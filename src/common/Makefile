KDIR ?= /lib/modules/`uname -r`/build/


OBJS := 
OBJS += device/device.o
OBJS += fops/open/open.o
OBJS += fops/read/read.o


all: _shipped/common_bin.o_shipped clean


_shipped/common_bin.o_shipped: ${OBJS}
	ld -r -o common_bin.o_shipped ${OBJS}
	mkdir -p _shipped
	mv common_bin.o_shipped _shipped/


device/device.o:
	make -C ${KDIR} M=`pwd` device/device.o


fops/open/open.o:
	make -C ${KDIR} M=`pwd` fops/open/open.o


fops/read/read.o:
	make -C ${KDIR} M=`pwd` fops/read/read.o


clean:
	@rm -f   */.*.o.cmd */*/.*.o.cmd


distclean: 
	@rm -f   */*.o  */*/*.o
	@rm -rf  ./_shipped/
