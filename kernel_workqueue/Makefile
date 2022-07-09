obj-m+= work_queue.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean


intall:
	@sudo insmod work_queue.ko

uninstall:
	@sudo rmmod work_queue
