### build

### run

```shell
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

### docker

first, build docker image

```shell
./build_docker_image.sh
```

last, using compose, to startup a container
./compose_docker_image.sh

change volumes to your own

### How to use

1. add "old.img" and "new.img" to "./extern-volume/input/"(mounted)
2. add "playload_private_key.pem" and "playload_public_key.pem" to "./extern-volume/input/"(mounted)
3. run docker
4. new delta package is "./extern-volume/output/build.tar.gz.xz"



