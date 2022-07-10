dd if=/dev/zero of=rootfs.img bs=1M count=10
mkfs.ext4 rootfs.img
sudo mount -t ext4 -o loop rootfs.img ./fs
sudo make install CONFIG_PREFIX=./fs
cd fs
sudo mkdir proc dev etc home mnt
sudo cp -r ../examples/bootfloppy/etc/* etc/
cd ..
sudo chmod -R 777 fs/
