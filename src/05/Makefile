CONFIG_MODULE_SIG=n


MODULES 	:=  /lib/modules/
KERN 		:=  ${shell uname -r}
KDIR 		:=  ${MODULES}/${KERN}/build/
DEST 		:=  ${MODULES}/${KERN}/kernel/custom/
STATIC	    :=  /root/kernel/src/common/


export KDIR


default: build


rebuild: fullclean build


fullclean: distclean
	make -C ${STATIC} distclean


build: static modules clean


static:
	make -C ${STATIC}
	cp ${STATIC}/_shipped/common_bin.o_shipped .
	touch .common_bin.o.cmd


modules:
	make -C ${KDIR} M=${PWD} modules


clean:
	@rm -f    *.o  .*.cmd  .*.cmd  .*.flags  *.mod.c  *.order
	@rm -f    .*.*.cmd  *~  *.*~  TODO.*  .*.d
	@rm -rf   .tmp*  *.mod
	@rm -f    *.o_shipped
	@rm -f    *.symvers


distclean:
	@rm -f 	  *.ko


install:
	@if [ ! -d ${DEST} ]; then mkdir ${DEST}; fi
	@cp ootm.ko ${DEST} && depmod -a
	@echo "'modprobe ${MODULE_OOTM}' to load module"


uninstall:
	@rm ${DEST}/ootm.ko && depmod -a
	@if [ -d ${DEST} ] && [ ! "$(ls -A ${DEST})" ]; then rm -rf ${DEST}; fi
	@echo "'modprobe -r ${MODULE_OOTM}' to unload module"
