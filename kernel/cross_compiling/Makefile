#CROSS_COMPILE:= arm-linux-
CROSS_COMPILE:=
ARCH:= arm
CC:= $(CROSS_COMPILE)gcc
LD:= $(CROSS_COMPILE)ld
obj-m := jlsemi.o
KERNELDIR := /usr/src/linux-3.18.97

PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -f *.o
	rm -f *.symvers
	rm -f *.order
	rm -f *.ko
	rm -f *.mod.c
