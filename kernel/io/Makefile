name := io_test

obj-m+= $(name).o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean


intall:
	@sudo insmod $(name).ko

uninstall:
	@sudo rmmod $(name)
