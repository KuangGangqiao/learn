name := interrupt

obj-m+= $(name).o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean


intall:
	@sudo insmod $(name).ko irq=1 devname=myirq

uninstall:
	@sudo rmmod $(name)
