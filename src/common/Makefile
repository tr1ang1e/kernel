KDIR ?= /lib/modules/`uname -r`/build/


OBJS := 
OBJS += src/device/device.o
OBJS += src/fops/open/open.o
OBJS += src/fops/read/read.o


all: _shipped/common_bin.o_shipped clean


_shipped/common_bin.o_shipped: ${OBJS}
	ld -r -o common_bin.o_shipped ${OBJS}
	mkdir -p _shipped
	mv common_bin.o_shipped _shipped/


src/device/device.o:
	make -C ${KDIR} M=`pwd` src/device/device.o


src/fops/open/open.o:
	make -C ${KDIR} M=`pwd` src/fops/open/open.o


src/fops/read/read.o:
	make -C ${KDIR} M=`pwd` src/fops/read/read.o


clean:
	@rm -f   */.*.o.cmd */*/.*.o.cmd


distclean: 
	@rm -f   */*.o  */*/*.o  */*/*/*.o
	@rm -rf  ./_shipped/
