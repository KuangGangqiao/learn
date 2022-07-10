all:
	make -C linux-4.14.191/ -j4

unpack:
	unzip -d ./ linux-4.14.191.zip

pack:
	make -C linux-4.14.191/ clean
	zip -r linux-4.14.191.zip linux-4.14.191/

start:
	qemu-system-x86_64 -kernel ./linux-4.14.191/arch/x86_64/boot/bzImage \
	-hda ./busybox-1.32.0/rootfs.img  -append "root=/dev/sda console=ttyS0" -nographic
