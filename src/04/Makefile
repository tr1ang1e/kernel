CONFIG_MODULE_SIG=n


MODULES 	:=  /lib/modules/
KERN 		:=  ${shell uname -r}
KDIR 		:=  ${MODULES}/${KERN}/build/
DEST 		:=  ${MODULES}/${KERN}/kernel/custom/


build: modules clean


modules:
	make -C ${KDIR} M=${PWD} modules


clean:
	@rm -f    *.o  .*.cmd  .*.cmd  .*.flags  *.mod.c  *.order
	@rm -f    .*.*.cmd  *~  *.*~  TODO.*  .*.d
	@rm -rf   .tmp*  *.mod


distclean:
	@rm -f 	  *.ko  *.symvers


install:
	@if [ ! -d ${DEST} ]; then mkdir ${DEST}; fi
	@cp ootm.ko ${DEST} && depmod -a


uninstall:
	@rm ${DEST}/ootm.ko && depmod -a
	@if [ -d ${DEST} ] && [ ! "$(ls -A ${DEST})" ]; then rm -rf ${DEST}; fi
