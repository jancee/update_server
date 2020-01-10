#!/bin/bash

# clean cache
rm -rf /opt/extern-volume/output/*
rm -rf /opt/extern-volume/tmp/*

# Build Patcher
python update_server.py \
    --src_image=/opt/extern-volume/input/old.img \
    --image=/opt/extern-volume/input/new.img \
    --board=beaglebone \
    --host_log \
    --private_key=/opt/extern-volume/input/playload_private_key.pem \
    --private_key_for_metadata_hash_signature=/opt/extern-volume/input/playload_private_key.pem \
    -p --clear_cache --exit --production

# Check Build Error
if [ "$?" != "0" ]; then
    exit 2
fi

# Find Build Cache Document
document_name="none"
for file in /opt/static/cache/*_*
do
    if [ -d $file ]
    then
        document_name=$file
    fi
done

# Check Build Cache Health
if [ -f ${document_name}"/metadata_hash" ] && 
    [ -f ${document_name}"/update.gz" ] && 
    [ -f ${document_name}"/update.meta" ]
then
    echo "==Generate OK=="
else
    exit 3
fi

# tar&gz&xz files, and output the package
# cd $document_name && tar cvzf /opt/extern-volume/tmp/build.tar.gz . && rm -rf $document_name
# xz -9 -k -e -z /opt/extern-volume/tmp/build.tar.gz
# mv /opt/extern-volume/tmp/build.tar.gz.xz /opt/extern-volume/output/ && rm -rf /opt/extern-volume/tmp/*
mv ${document_name}/metadata_hash /opt/extern-volume/output/
mv ${document_name}/update.gz /opt/extern-volume/output/
mv ${document_name}/update.meta /opt/extern-volume/output/

# mount img, and copy config to output
MOUNT_OFFSET=$(fdisk -l /opt/extern-volume/input/new.img | sed -n 11p | cut -d' ' -f3 | awk '{printf "%.f",int($0)*512}')
echo MountOffset${MOUNT_OFFSET}
mkdir -p /img-mnt/old /img-mnt/new
mount -o loop,offset=${MOUNT_OFFSET},ro /opt/extern-volume/input/old.img /img-mnt/old
mount -o loop,offset=${MOUNT_OFFSET},ro /opt/extern-volume/input/new.img /img-mnt/new
cp /img-mnt/old/etc/lsb-release /opt/extern-volume/output/old-lsb-release
cp /img-mnt/new/etc/lsb-release /opt/extern-volume/output/new-lsb-release

# console打印ok，表示成功
echo "==Finished OK=="
exit 0




