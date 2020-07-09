# procs_info makefile
#

KERNELDIR=/usr/src/linux-headers-2.6.24-19-generic/
PWD=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm *.o *.ko *.order *.symvers *.mod.c
