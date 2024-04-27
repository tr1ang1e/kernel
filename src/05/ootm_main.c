#include <linux/cdev.h>
#include <linux/init.h>
#include <common/author.h>
#include <common/macros.h>
#include <common/fops/open/open.h>
#include <common/fops/read/read.h>


/* --------------------------------------------------------- */
/*                        M A C R O S                        */
/* --------------------------------------------------------- */


#define MODULE_NAME     "ootm"
#define DEVICE_NAME     MODULE_NAME "_cdev"
#define DEVICE_FIRST    0
#define DEVICE_COUNT    3


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


static int major = 0;
module_param(major, int, S_IRUGO);


static struct file_operations fops =
{
    .owner    = THIS_MODULE,
    .open     = fops_open,
    .release  = fops_release,
    .read     = fops_read
};


static struct cdev hcdev = { 0 };    // cdev handler


static dev_t dev = 0;    // device major and minor  


/* --------------------------------------------------------- */
/*             M O D U L E   M A N A G E M E N T             */
/* --------------------------------------------------------- */


static int 
__init 
ootm_main_init(void)
{
    GREETING();

    int ret = 0;

    if (0 != major)
    {
        // try to set specific major number
        dev = MKDEV(major, DEVICE_FIRST);
        ret = register_chrdev_region(dev, DEVICE_COUNT, DEVICE_NAME);
    }
    else
    {
        // get major number dynamically
        ret = alloc_chrdev_region(&dev, DEVICE_FIRST, DEVICE_COUNT, DEVICE_NAME);
        major = MAJOR(dev);
    }

    if (ret < 0)
    {
        printk(KERN_ERR "Error: *_chrdev_region\n");
        goto exit;
    }

    cdev_init(&hcdev, &fops);
    hcdev.owner = THIS_MODULE;

    ret = cdev_add(&hcdev, dev, DEVICE_COUNT);
    if (ret < 0)
    {
        unregister_chrdev_region(dev, DEVICE_COUNT);
        printk(KERN_ERR "Error: cdev_add\n");
        goto exit;
    }

    printk(KERN_INFO DEVICE_NAME " installed %d:%d\n", MAJOR(dev), MINOR(dev));
    
exit:

    return ret;
}


static void 
__exit 
ootm_main_exit(void)
{
    FAREWELL();

    cdev_del(&hcdev);
    unregister_chrdev_region(dev, DEVICE_COUNT);
    
    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);
