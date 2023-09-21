ifneq ($(KERNELRELEASE),)
obj-m:=fakemac_device.o fakemac_driver.o fakephy.o
else

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

install:
	sudo insmod fakephy.ko
	sudo insmod fakemac_device.ko
	sudo insmod fakemac_driver.ko

uninstall:
	sudo rmmod fakephy.ko
	sudo rmmod fakemac_driver.ko
	sudo rmmod fakemac_device.ko

endif

