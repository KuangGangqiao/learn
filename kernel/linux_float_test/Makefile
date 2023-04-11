obj-m := test_float.o
KDIR := /lib/modules/$(shell uname -r)/build

FPU_CFLAGS += -mhard-float
FPU_CFLAGS += -msse -msse2
CFLAGS_test_float.o += $(FPU_CFLAGS)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

