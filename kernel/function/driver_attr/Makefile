ifneq ($(KERNELRELEASE),)
obj-m:=device.o driver.o
else
KDIR :=/lib/modules/$(shell uname -r)/build
PWD  :=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.symvers *.cmd  *.mod.c *.order

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

