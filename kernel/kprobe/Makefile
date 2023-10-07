name := kprobe_example

obj-m+= $(name).o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean


install:
	@sudo insmod $(name).ko

uninstall:
	@sudo rmmod $(name)
