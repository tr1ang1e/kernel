#include <linux/init.h>
#include <linux/miscdevice.h>
#include <common/include/headers.h>


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


static int minor = UNSET_INT;
module_param(minor, int, S_IRUGO);


static const struct file_operations fops =
{
    .owner    = THIS_MODULE,
    .read     = fops_read
};


static struct miscdevice misc_dev =
{
    .name     = "ootm_misc",
    .fops     = &fops
};


/* --------------------------------------------------------- */
/*             M O D U L E   M A N A G E M E N T             */
/* --------------------------------------------------------- */


static int 
__init 
ootm_main_init(void)
{
    GREETING();

    int rc = 0;

    misc_dev.minor = (minor == UNSET_INT) ? MISC_DYNAMIC_MINOR : minor;

    rc = misc_register(&misc_dev);
    if (0 != rc)  {
        printk(KERN_ERR "Error: %s\n", "failed to misc_register()");
    }
    
    return rc;
}


static void 
__exit 
ootm_main_exit(void)
{
    FAREWELL();

    misc_deregister(&misc_dev);

    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);
