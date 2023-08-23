ifneq ($(KERNELRELEASE),)
obj-m:=device.o driver.o
else

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

install:
	sudo insmod device.ko
	sudo insmod driver.ko
uninstall:
	sudo rmmod driver.ko
	sudo rmmod device.ko

run:
	sudo chmod 777 /sys/bus/platform/drivers/duang/peng
	sudo cat /sys/bus/platform/drivers/duang/peng
	sudo echo 1 > /sys/bus/platform/drivers/duang/peng

endif

