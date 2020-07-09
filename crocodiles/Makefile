#KERNELDIR=/home/student/linux-2.6.27-rc9
KERNELDIR=/home/student/dist/linux-2.6.27
PWD=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm *.ko *.o *.mod.c *.order *.symvers
