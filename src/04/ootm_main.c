#include <linux/init.h>
#include <linux/module.h>
#include "extern.h"


MODULE_LICENSE("<license>");
MODULE_AUTHOR("<name> <email>");


static int 
__init 
ootm_main_init(void)
{
    printk("%s from ootm_main. Extern func = [%p]\n", 
           get_greeting(), 
           (void*)get_greeting);

    return 0;
}


static void 
__exit 
ootm_main_exit(void)
{
    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);

