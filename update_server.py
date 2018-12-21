#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# Copyright (c) 2009-2012 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
import json
import optparse  # pylint: disable=deprecated-module
import os
import re
import shutil
import signal
import socket
import subprocess
import sys
import tempfile
import threading
import time
import types
from logging import handlers
import portend
import cherrypy
# pylint: disable=no-name-in-module
from cherrypy import _cplogging as cplogging
from cherrypy.process import plugins # pylint: disable=import-error

import autoupdate 
import cherrypy_ext
import common_util
import devserver_constants
import log_util


# Module-local log function.
def _Log(message, *args):
    return log_util.LogWithTag('UPDATE SERVER', message, *args)

try:
    import psutil
except ImportError:
    # Ignore psutil import failure. This is for backwards compatibility, so
    # "cros flash" can still update duts with build without psutil installed.
    # The reason is that, during cros flash, local update server code is copied over
    # to DUT, and update server will be running inside DUT to stage the build.
    _Log('Python module psutil is not installed, update server load data will not be '
        'collected')
    psutil = None
except OSError as e:
    # Ignore error like following. psutil may not work properly in builder. Ignore
    # the error as load information of update server is not used in builder.
    # OSError: [Errno 2] No such file or directory: '/dev/pts/0'
    _Log('psutil is failed to be imported, error: %s. update server load data will '
        'not be collected.', e)
    psutil = None


CACHED_ENTRIES = 12

TELEMETRY_FOLDER = 'telemetry_src'
TELEMETRY_DEPS = ['dep-telemetry_dep.tar.bz2',
                  'dep-page_cycler_dep.tar.bz2',
                  'dep-chrome_test.tar.bz2',
                  'dep-perf_data_dep.tar.bz2']

# Sets up global to share between classes.
updater = None

# Log rotation parameters.  These settings correspond to twice a day once
# update server is started, with about two weeks (28 backup files) of old logs
# kept for backup.
#
# For more, see the documentation in standard python library for
# logging.handlers.TimedRotatingFileHandler
_LOG_ROTATION_TIME = 'H'
_LOG_ROTATION_INTERVAL = 12 # hours
_LOG_ROTATION_BACKUP = 28 # backup counts

# Number of seconds between the collection of disk and network IO counters.
STATS_INTERVAL = 10.0

# Auto-update parameters

# Error msg for missing key in CrOS auto-update.
KEY_ERROR_MSG = 'Key Error in RPC: %s= is required'

# Command of running auto-update.
AUTO_UPDATE_CMD = '/usr/bin/python -u %s -d %s -b %s --static_dir %s'

class DevServerError(Exception):
  """Exception class used by this module."""


def require_psutil():
    """Decorator for functions require psutil to run."""
    def deco_require_psutil(func):
        """Wrapper of the decorator function.

        Args:
        func: function to be called.
        """
        def func_require_psutil(*args, **kwargs):
            """Decorator for functions require psutil to run.

            If psutil is not installed, skip calling the function.

            Args:
                *args: arguments for function to be called.
                **kwargs: keyword arguments for function to be called.
            """
            if psutil:
                return func(*args, **kwargs)
            else:
                _Log('Python module psutil is not installed. Function call %s is '
                    'skipped.' % func)
        return func_require_psutil
    return deco_require_psutil

def _GetUpdateTimestampHandler(static_dir):
    """Returns a handler to update directory staged.timestamp.

    This handler resets the stage.timestamp whenever static content is accessed.

    Args:
        static_dir: Directory from which static content is being staged.

    Returns:
        A cherrypy handler to update the timestamp of accessed content.
    """
    def UpdateTimestampHandler():
        if not '404' in cherrypy.response.status:
            build_match = re.match(devserver_constants.STAGED_BUILD_REGEX,
                                    cherrypy.request.path_info)
        if build_match:
            build_dir = os.path.join(static_dir, build_match.group('build'))
            file_name = os.path.join(build_dir, 'staged.timestamp')
            # Easiest python version of |touch file_name|
            with file(file_name, 'a'):
                os.utime(file_name, None)        
    return UpdateTimestampHandler

def _GetConfig(options):
    """Returns the configuration for the update server."""

    socket_host = '::'
    # Fall back to IPv4 when python is not configured with IPv6.
    if not socket.has_ipv6:
        socket_host = '0.0.0.0'

    # Adds the UpdateTimestampHandler to cherrypy's tools. This tools executes
    # on the on_end_resource hook. This hook is called once processing is
    # complete and the response is ready to be returned.
    cherrypy.tools.update_timestamp = cherrypy.Tool(
        'on_end_resource', _GetUpdateTimestampHandler(options.static_dir))

    base_config = {
        'global': {
            'server.log_request_headers': True,
            'server.protocol_version': 'HTTP/1.1',
            'server.socket_host': socket_host,
            'server.socket_port': int(options.port),
            'response.timeout': 6000,
            'request.show_tracebacks': True,
            'server.socket_timeout': 60,
            'server.thread_pool': 2,
            'engine.autoreload.on': False,
        },
        '/update': {
            # Gets rid of cherrypy parsing post file for args.
            'request.process_request_body': False,
            'response.timeout': 10000,
        },
        # Sets up the static dir for file hosting.
        '/static': {
            'tools.staticdir.dir': options.static_dir,
            'tools.staticdir.on': True,
            'response.timeout': 10000,
            'tools.update_timestamp.on': True,
        },
    }
    if options.production:
        base_config['global'].update({'server.thread_pool': 150})

    return base_config

def MakeLogHandler(logfile):
    """Create a LogHandler instance used to log all messages."""
    hdlr_cls = handlers.TimedRotatingFileHandler
    hdlr = hdlr_cls(logfile, when=_LOG_ROTATION_TIME,
                    interval=_LOG_ROTATION_INTERVAL,
                    backupCount=_LOG_ROTATION_BACKUP)
    hdlr.setFormatter(cplogging.logfmt)
    return hdlr

def _CleanCache(cache_dir, wipe):
    """Wipes any excess cached items in the cache_dir.

    Args:
        cache_dir: the directory we are wiping from.
        wipe: If True, wipe all the contents -- not just the excess.
    """
    if wipe:
        # Clear the cache and exit on error.
        cmd = 'rm -rf %s/*' % cache_dir
        if os.system(cmd) != 0:
            _Log('Failed to clear the cache with %s' % cmd)
            sys.exit(1)
    else:
        # Clear all but the last N cached updates
        cmd = ('cd %s; ls -tr | head --lines=-%d | xargs rm -rf' %
            (cache_dir, CACHED_ENTRIES))
        if os.system(cmd) != 0:
            _Log('Failed to clean up old delta cache files with %s' % cmd)
            sys.exit(1)

def _IsExposed(name):
  """Returns True iff |name| has an `exposed' attribute and it is set."""
  return hasattr(name, 'exposed') and name.exposed

def _FindExposedMethods(root, prefix, unlisted=None):
  """Finds exposed CherryPy methods.

  Args:
    root: the root object for searching
    prefix: slash-joined chain of members leading to current object
    unlisted: URLs to be excluded regardless of their exposed status

  Returns:
    List of exposed URLs that are not unlisted.
  """
  method_list = []
  for member in sorted(root.__class__.__dict__.keys()):
    prefixed_member = prefix + '/' + member if prefix else member
    if unlisted and prefixed_member in unlisted:
      continue
    member_obj = root.__class__.__dict__[member]
    if _IsExposed(member_obj):
      if type(member_obj) == types.FunctionType:
        method_list.append(prefixed_member)
      else:
        method_list += _FindExposedMethods(
            member_obj, prefixed_member, unlisted)
  return method_list

def _GetRecursiveMemberObject(root, member_list):
  """Returns an object corresponding to a nested member list.

  Args:
    root: the root object to search
    member_list: list of nested members to search

  Returns:
    An object corresponding to the member name list; None otherwise.
  """
  for member in member_list:
    next_root = root.__class__.__dict__.get(member)
    if not next_root:
      return None
    root = next_root
  return root



def _GetExposedMethod(root, nested_member, ignored=None):
  """Returns a CherryPy-exposed method, if such exists.

  Args:
    root: the root object for searching
    nested_member: a slash-joined path to the nested member
    ignored: method paths to be ignored

  Returns:
    A function object corresponding to the path defined by |member_list| from
    the |root| object, if the function is exposed and not ignored; None
    otherwise.
  """
  method = (not (ignored and nested_member in ignored) and
            _GetRecursiveMemberObject(root, nested_member.split('/')))
  if method and type(method) == types.FunctionType and _IsExposed(method):
    return method


class DevServerRoot(object):
    """The Root Class for the Dev Server.

    CherryPy works as follows:
    For each method in this class, cherrpy interprets root/path
    as a call to an instance of DevServerRoot->method_name.  For example,
    a call to http://myhost/build will call build.  CherryPy automatically
    parses http args and places them as keyword arguments in each method.
    For paths http://myhost/update/dir1/dir2, you can use *args so that
    cherrypy uses the update method and puts the extra paths in args.
    """
    # Method names that should not be listed on the index page.
    _UNLISTED_METHODS = ['index']


    # Number of threads that update server is staging images.
    _staging_thread_count = 0
    # Lock used to lock increasing/decreasing count.
    _staging_thread_count_lock = threading.Lock()
    @require_psutil()
    def _refresh_io_stats(self):
        """A call running in a thread to update IO stats periodically."""
        prev_disk_io_counters = psutil.disk_io_counters()
        prev_network_io_counters = psutil.net_io_counters()
        prev_read_time = time.time()
        while True:
            time.sleep(STATS_INTERVAL)
            now = time.time()
            interval = now - prev_read_time
            prev_read_time = now
            # Disk IO is for all disks.
            disk_io_counters = psutil.disk_io_counters()
            network_io_counters = psutil.net_io_counters()

            self.disk_read_bytes_per_sec = (
                disk_io_counters.read_bytes -
                prev_disk_io_counters.read_bytes)/interval
            self.disk_write_bytes_per_sec = (
                disk_io_counters.write_bytes -
                prev_disk_io_counters.write_bytes)/interval
            prev_disk_io_counters = disk_io_counters

            self.network_sent_bytes_per_sec = (
                network_io_counters.bytes_sent -
                prev_network_io_counters.bytes_sent)/interval
            self.network_recv_bytes_per_sec = (
                network_io_counters.bytes_recv -
                prev_network_io_counters.bytes_recv)/interval
            prev_network_io_counters = network_io_counters

    @require_psutil()
    def _start_io_stat_thread(self):
        """Start the thread to collect IO stats."""
        thread = threading.Thread(target=self._refresh_io_stats)
        thread.daemon = True
        thread.start()

    def __init__(self):
        self._builder = None
        self._telemetry_lock_dict = common_util.LockDict()

        # Cache of disk IO stats, a thread refresh the stats every 10 seconds.
        # lock is not used for these variables as the only thread writes to these
        # variables is _refresh_io_stats.
        self.disk_read_bytes_per_sec = 0
        self.disk_write_bytes_per_sec = 0
        # Cache of network IO stats.
        self.network_sent_bytes_per_sec = 0
        self.network_recv_bytes_per_sec = 0
        self._start_io_stat_thread()

    @cherrypy.expose
    def index(self):
        """Presents a welcome message and documentation links."""
        return ('Welcome to the Update Server!<br>\n'
                '<br>\n'
                'Here are the available methods, click for documentation:<br>\n'
                '<br>\n'
                '%s' %
                '<br>\n'.join(
                    [('<a href=doc/%s>%s</a>' % (name, name))
                        for name in _FindExposedMethods(
                            self, '', unlisted=self._UNLISTED_METHODS)]))

    @cherrypy.expose
    def doc(self, *args):
        """Shows the documentation for available methods / URLs.

        Example:
            http://myhost/doc/update
        """
        name = '/'.join(args)
        method = _GetExposedMethod(self, name)
        if not method:
            raise DevServerError("No exposed method named `%s'" % name)
        if not method.__doc__:
            raise DevServerError("No documentation for exposed method `%s'" % name)
        return '<pre>\n%s</pre>' % method.__doc__


    @cherrypy.expose
    def update(self, *args):
        """Handles an update check from a slinux client.

        The HTTP request should contain the standard Omaha-style XML blob. The URL
        line may contain an additional intermediate path to the update payload.

        This request can be handled in one of 4 ways, depending on the devsever
        settings and intermediate path.

        1. No intermediate path
        If no intermediate path is given, the default behavior is to generate an
        update payload from the latest test image locally built for the board
        specified in the xml. Devserver serves the generated payload.

        2. Path explicitly invokes XBuddy
        If there is a path given, it can explicitly invoke xbuddy by prefixing it
        with 'xbuddy'. This path is then used to acquire an image binary for the
        update server to generate an update payload from. Devserver then serves this
        payload.

        3. Path is left for the update server to interpret.
        If the path given doesn't explicitly invoke xbuddy, update server will attempt
        to generate a payload from the test image in that directory and serve it.

        4. The update server is in a 'forced' mode. TO BE DEPRECATED
        This comes from the usage of --forced_payload or --image when starting the
        update server. No matter what path (or no path) gets passed in, update server will
        serve the update payload (--forced_payload) or generate an update payload
        from the image (--image).

        Examples:
        1. No intermediate path
        update_engine_client --omaha_url=http://myhost/update
        This generates an update payload from the latest test image locally built
        for the board specified in the xml.

        2. Explicitly invoke xbuddy
        update_engine_client --omaha_url=
        http://myhost/update/xbuddy/remote/board/version/dev
        This would go to GS to download the dev image for the board, from which
        the update server would generate a payload to serve.

        3. Give a path for update server to interpret
        update_engine_client --omaha_url=http://myhost/update/some/random/path
        This would attempt, in order to:
            a) Generate an update from a test image binary if found in
            static_dir/some/random/path.
            b) Serve an update payload found in static_dir/some/random/path.
            c) Hope that some/random/path takes the form "board/version" and
            and attempt to download an update payload for that board/version
            from GS.
        """
        label = '/'.join(args)
        body_length = int(cherrypy.request.headers.get('Content-Length', 0))
        data = cherrypy.request.rfile.read(body_length)

        return updater.HandleUpdatePing(data, label)
            
def _AddUpdateOptions(parser):
  group = optparse.OptionGroup(
      parser, 'Autoupdate Options', 'These options can be used to change '
      'how the update server either generates or serve update payloads. Please '
      'note that all of these option affect how a payload is generated and so '
      'do not work in archive-only mode.')
  group.add_option('--board',
                   help='By default the update server will create an update '
                   'payload from the latest image built for the board '
                   'a device that is requesting an update has. When we '
                   'pre-generate an update (see below) and we do not specify '
                   'another update_type option like image or payload, the '
                   'update server needs to know the board to generate the latest '
                   'image for. This is that board.')
  group.add_option('--critical_update',
                   action='store_true', default=False,
                   help='Present update payload as critical')
  group.add_option('--image',
                   metavar='FILE',
                   help='Generate and serve an update using this image to any '
                   'device that requests an update.')
  group.add_option('--payload',
                   metavar='PATH',
                   help='use the update payload from specified directory '
                   '(update.gz).')
  group.add_option('-p', '--pregenerate_update',
                   action='store_true', default=False,
                   help='pre-generate the update payload before accepting '
                   'update requests. Useful to help debug payload generation '
                   'issues quickly. Also if an update payload will take a '
                   'long time to generate, a client may timeout if you do not'
                   'pregenerate the update.')
  group.add_option('--src_image',
                   metavar='PATH', default='',
                   help='If specified, delta updates will be generated using '
                   'this image as the source image. Delta updates are when '
                   'you are updating from a "source image" to a another '
                   'image.')
  group.add_option('--private_key',
                   metavar='PATH', default=None,
                   help='path to the private key in pem format. If this is set '
                   'the update server will generate update payloads that are '
                   'signed with this key.')
  group.add_option('--max_updates',
                   metavar='NUM', default=-1, type='int',
                   help='maximum number of update checks handled positively '
                        '(default: unlimited)')
  group.add_option('--private_key_for_metadata_hash_signature',
                   metavar='PATH', default=None,
                   help='path to the private key in pem format. If this is set '
                   'the update server will sign the metadata hash with the given '
                   'key and transmit in the Omaha-style XML response.')
  group.add_option('--public_key',
                   metavar='PATH', default=None,
                   help='path to the public key in pem format. If this is set '
                   'the update server will transmit a base64 encoded version of '
                   'the content in the Omaha-style XML response.')
  group.add_option('--remote_payload',
                   action='store_true', default=False,
                   help='Payload is being served from a remote machine. With '
                   'this setting enabled, this update server instance serves as '
                   'just an Omaha server instance. In this mode, the '
                   'update server enforces a few extra components of the Omaha '
                   'protocol, such as hardware class, being sent.')
  group.add_option('-u', '--urlbase',
                   metavar='URL',
                   help='base URL for update images, other than the '
                   'update server. Use in conjunction with remote_payload.')   
  group.add_option('--production',
                   action='store_true', default=False,
                   help='have the update server use production values when '
                   'starting up. This includes using more threads and '
                   'performing less logging.')   
  group.add_option('--logfile',
                   metavar='PATH',
                   help='log output to this file instead of stdout') 
  group.add_option('--host_log',
                   action='store_true', default=False,
                   help='record history of host update events (/api/hostlog)')
  group.add_option('--clear_cache',
                   action='store_true', default=False,
                   help='At startup, removes all cached entries from the'
                   'update server\'s cache.')                   
  group.add_option('--exit',
                   action='store_true',
                   help='do not start the server (yet pregenerate/clear cache)')                                                                      
  parser.add_option_group(group)

def main():
    usage = '\n\n'.join(['usage: prog [options]'])
    parser = optparse.OptionParser(usage=usage)
    # get directory that the update server is run from
    devserver_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
    default_static_dir = '%s/static' % devserver_dir
    parser.add_option('--static_dir',
                        metavar='PATH',
                        default=default_static_dir,
                        help='writable static directory')
    parser.add_option('--port',
                        default=8080, type='int',
                        help=('port for the dev server to use; if zero, binds to '
                            'an arbitrary available port (default: 8080)'))
    _AddUpdateOptions(parser)
    (options, _) = parser.parse_args()

    # Handle options that must be set globally in cherrypy.  Do this
    # work up front, because calls to _Log() below depend on this
    # initialization.
    if options.production:
        cherrypy.config.update({'environment': 'production'})
    if not options.logfile:
        cherrypy.config.update({'log.screen': True})
    else:
        cherrypy.config.update({'log.error_file': '',
                                'log.access_file': ''})
        hdlr = MakeLogHandler(options.logfile)
        # Pylint can't seem to process these two calls properly
        # pylint: disable=E1101
        cherrypy.log.access_log.addHandler(hdlr)
        cherrypy.log.error_log.addHandler(hdlr)
        # pylint: enable=E1101

    # set static_dir, from which everything will be served
    options.static_dir = os.path.realpath(options.static_dir)

    cache_dir = os.path.join(options.static_dir, 'cache')
    # If our update server is only supposed to serve payloads, we shouldn't be
    # mucking with the cache at all. If the update server hadn't previously
    # generated a cache and is expected, the caller is using it wrong.
    if os.path.exists(cache_dir):
        _CleanCache(cache_dir, options.clear_cache)
    else:
        os.makedirs(cache_dir)

    _Log('Using cache directory %s' % cache_dir)
    _Log('Serving from %s' % options.static_dir)
    # We allow global use here to share with cherrypy classes.
    # pylint: disable=W0603
    global updater
    updater = autoupdate.Autoupdate(
        static_dir=options.static_dir,
        urlbase=options.urlbase,
        forced_image=options.image,
        payload_path=options.payload,
        proxy_port=None,
        src_image=options.src_image,
        board=options.board,
        copy_to_static_root=not options.exit,
        private_key=options.private_key,
        private_key_for_metadata_hash_signature=(
            options.private_key_for_metadata_hash_signature),
        public_key=options.public_key,
        critical_update=options.critical_update,
        remote_payload=options.remote_payload,
        max_updates=options.max_updates,
        host_log=options.host_log,
    )

    if options.pregenerate_update:
        updater.PreGenerateUpdate()

    if options.exit:
        return
    
    # Patch CherryPy to support binding to any available port (--port=0).
    portend.free('::1', options.port, timeout=5)

    cherrypy.quickstart(DevServerRoot(), config=_GetConfig(options))

if __name__ == '__main__':
    main()
