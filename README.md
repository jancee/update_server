### run

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/delta_generator
export PATH=$PATH:$PWD/delta_generator:$PWD/chromite/bin
#delta update .eg:
sudo python update_server.py  \
    --image=/home/parallels/develop/seeed-update-server/update_server/armhf-rootfs-debian-stretch-new.tar \
    --src_image=/home/parallels/develop/seeed-update-server/update_server/armhf-rootfs-debian-stretch-old.tar \
    --board=beaglebone \
    --host_log \
    --private_key=/home/parallels/develop/seeed-update-server/update_server/keys/playload_private_key.pem \
    --private_key_for_metadata_hash_signature=/home/parallels/develop/seeed-update-server/update_server/keys/playload_private_key.pem \
    -p \
    --exit
```
