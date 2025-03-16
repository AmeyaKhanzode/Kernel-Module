obj-m := kern_module.o

KBUILD_EXTRA_SYMBOLS := /home/ameya/Kernel-Module/Module.symvers

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

