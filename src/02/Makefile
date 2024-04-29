PWD = $(shell pwd)
CURRENT = $(shell uname -r)
MODULES = /lib/modules
KDIR = $(MODULES)/$(CURRENT)/build
DEST = $(MODULES)/$(CURRENT)/misc
TARGET = ootm
obj-m := $(TARGET).o


build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


clean:
	@rm -f    *.o  .*.cmd  .*.cmd  .*.flags  *.mod.c  *.order
	@rm -f    .*.*.cmd  *~  *.*~  TODO.*
	@rm -rf   .tmp*  *.mod
	@rm -f    *.symvers  *.ko
