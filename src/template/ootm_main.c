#include <linux/init.h>
#include <linux/module.h>
#include <common/macros.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("tr1angle <tr1angle.goog1e@gmail.com>");


static int 
__init 
ootm_main_init(void)
{
    printk("Hello from %s!\n", FILENAME);
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
