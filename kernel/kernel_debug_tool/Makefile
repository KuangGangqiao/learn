OBJ_KERNEL=linux-4.14.191
OBJ_PATH=

all:
	make -C ${OBJ_KERNEL}/ -j4

unpack:
	unzip -d ./ ${OBJ_KERNEL}.zip

pack:
	make -C ${OBJ_KERNEL}/ clean
	zip -r ${OBJ_KERNEL}.zip ${OBJ_KERNEL}/

setup_env:
	apt-get install qemu-system-x86
start:
	qemu-system-x86_64 -kernel ${OBJ_PATH}${OBJ_KERNEL}/arch/x86_64/boot/bzImage \
	-hda ./busybox-1.32.0/rootfs.img  -append "root=/dev/sda console=ttyS0" -nographic
