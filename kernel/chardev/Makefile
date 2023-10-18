name := cdev_driver

obj-m+= $(name).o

all:
	gcc app.c -o app
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	rm app
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean


install:
	@sudo insmod $(name).ko

uninstall:
	@sudo rmmod $(name)
