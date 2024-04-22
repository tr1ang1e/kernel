#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <common/author.h>
#include <common/macros.h>
#include <common/fops/open/open.h>
#include <common/fops/read/read.h>


/* --------------------------------------------------------- */
/*                        M A C R O S                        */
/* --------------------------------------------------------- */


#define DEVICE_NAME             MODULE_NAME "_cdev"
#define DEVICE_CLASS_NAME       DEVICE_NAME "_class"
#define DEVICE_INSTANCE_NAME    MODULE_NAME
#define DEVICE_MINOR_FIRST      0
#define DEVICE_MINOR_COUNT      3


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


static struct class *devclass = { 0 };      // device class handler
static struct cdev hcdev = { 0 };           // char device instance


/* --------------------------------------------------------- */
/*             M O D U L E   M A N A G E M E N T             */
/* --------------------------------------------------------- */


static int 
__init 
ootm_main_init(void)
{
    GREETING();

    int      minor = DEVICE_MINOR_FIRST; 
    int      ret = 0;
    dev_t    dev = 0;


    // create character device region, which 
    // starts from (major + DEVICE_MINOR_FIRST) and 
    // might contain DEVICE_MINOR_COUNT instances
    if (0 != major)
    {
        // try to set specific major number
        dev = MKDEV(major, DEVICE_MINOR_FIRST);
        ret = register_chrdev_region(dev, DEVICE_MINOR_COUNT, DEVICE_NAME);
    }
    else
    {
        // get major number dynamically
        ret = alloc_chrdev_region(&dev, DEVICE_MINOR_FIRST, DEVICE_MINOR_COUNT, DEVICE_NAME);
        major = MAJOR(dev);
    }

    if (ret < 0)
    {
        printk(KERN_ERR "Error: *_chrdev_region\n");
        goto exit;
    }

    // initialize char device handler
    cdev_init(&hcdev, &fops);
    hcdev.owner = THIS_MODULE;

    // register device with given (major + initial minor) combination
    // and specify how much minor numbers are available for device
    ret = cdev_add(&hcdev, dev, DEVICE_MINOR_COUNT);
    if (ret < 0)
    {
        unregister_chrdev_region(dev, DEVICE_MINOR_COUNT);
        printk(KERN_ERR "Error: cdev_add\n");
        goto exit;
    }

    // create device class itself + class instances
    // it means that corresponding device nodes will be created automatically
    devclass = class_create(THIS_MODULE, DEVICE_CLASS_NAME);
    for ( ; minor < DEVICE_MINOR_COUNT; ++minor)
    {
        dev = MKDEV(major, minor);
        device_create
        (
            devclass,                                   // instance class
            NULL,                                       // [?] struct device* parent
            dev,                                        // device major + minor number
            NULL,                                       // [?] void* drvdata
            "%s_%d", DEVICE_INSTANCE_NAME, minor        // device node name = /dev/"%s_%d"
        );
    }

    printk(KERN_INFO DEVICE_NAME " installed %d:[%d-%d]\n", MAJOR(dev), DEVICE_MINOR_FIRST, MINOR(dev));
    
exit:

    return ret;
}


static void 
__exit 
ootm_main_exit(void)
{
    FAREWELL();

    int      minor = DEVICE_MINOR_FIRST;
    dev_t    dev = 0;

    // device destruction must be done
    // in reverse to init order:
    //      destroy device class instances
    //      destroy device class itself
    //      delete char device handler
    //      destroy character device region

    for ( ; minor < DEVICE_MINOR_COUNT; ++minor)
    {
        dev = MKDEV(major, minor);
        device_destroy(devclass, dev);
    }

    class_destroy(devclass);
    
    cdev_del(&hcdev);
    
    unregister_chrdev_region(MKDEV(major, DEVICE_MINOR_FIRST), DEVICE_MINOR_COUNT);
    
    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);
