#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <common/author.h>
#include <common/macros.h>
#include <common/fops/open/open.h>
#include <common/fops/read/read.h>
#include <common/result.h>


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


static struct class *devclass = NULL;       // device class handler
static struct cdev hcdev = { 0 };           // char device instance


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static  void  ootm_create_cdev_region(ootm_result *result);
static  void  ootm_destroy_cdev_region(dev_t dev);

static  void  ootm_add_cdev(ootm_result *result);
static  void  ootm_delete_cdev();

static  void  ootm_create_device_nodes(ootm_result *result);
static  void  ootm_destroy_device_nodes();


/* --------------------------------------------------------- */
/*             M O D U L E   M A N A G E M E N T             */
/* --------------------------------------------------------- */


static int 
__init 
ootm_main_init(void)
{
    GREETING();

    dev_t          dev = 0;
    ootm_result    result = { 0 };

    ootm_create_cdev_region(&result);
    if (result.error < 0) 
    {
        printk(KERN_ERR "Error: %s\n", result.what);
        goto exit;
    }

    // keep global data which no longer will be changed
    dev = result.value_dev_t;
    major = MAJOR(dev);

    ootm_add_cdev(&result);
    if (result.error < 0)
    {
        printk(KERN_ERR "Error: %s\n", result.what);
        goto destroy_cdev_region;
    }

    ootm_create_device_nodes(&result);
    // TODO:
    // if ( <error> ) 
    // {
    //     ...
    //     ...
    //     goto delete_cdev;
    // }

    printk(KERN_INFO DEVICE_NAME " installed %d:[%d-%d]\n", MAJOR(dev), DEVICE_MINOR_FIRST, MINOR(dev));
    
    goto exit;


// Cleanup operations are started from here.
// They must be performed in reverse order.

delete_cdev:

    ootm_delete_cdev();

destroy_cdev_region:

    ootm_destroy_cdev_region(dev);

exit:

    return result.error;
}


static void 
__exit 
ootm_main_exit(void)
{
    FAREWELL();

    // Device destruction must be done
    // in reverse to init order:
    //      destroy device class instances and then class itself
    //      delete char device handler
    //      destroy character device region

    ootm_destroy_device_nodes();
    
    ootm_delete_cdev();

    ootm_destroy_cdev_region(MKDEV(major, DEVICE_MINOR_FIRST));
    
    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static void   
ootm_create_cdev_region(ootm_result *result)
{
    /*
        Create character device region: custom or auto

        [out]  ootm_result::value_dev_t     created device (major + DEVICE_MINOR_FIRST) if success
        [out]  ootm_result::error
        [out]  ootm_result::what
    */
    
    int      ret = 0; 
    dev_t    dev = 0;

    // create character device region, which 
    // starts from (major + DEVICE_MINOR_FIRST) and 
    // might contain DEVICE_MINOR_COUNT instances
    if (0 != major)
    {
        // try to set custom major number
        dev = MKDEV(major, DEVICE_MINOR_FIRST);
        ret = register_chrdev_region(dev, DEVICE_MINOR_COUNT, DEVICE_NAME);
    }
    else
    {
        // get major number dynamically
        ret = alloc_chrdev_region(&dev, DEVICE_MINOR_FIRST, DEVICE_MINOR_COUNT, DEVICE_NAME);
    }

    result->error = ret;
    result->what = "*_chrdev_region()";
    result->value_dev_t = dev;

    return;
}


static void
ootm_destroy_cdev_region(dev_t dev)
{
    unregister_chrdev_region(dev, DEVICE_MINOR_COUNT);

    return;
}


static void 
ootm_add_cdev(ootm_result *result)
{  
    /*
        Initialize and add character device

        [in]   ootm_result::value_dev_t     device to be initialized
        [out]  ootm_result::error
        [out]  ootm_result::what
    */

    // initialize char device handler
    cdev_init(&hcdev, &fops);
    hcdev.owner = THIS_MODULE;

    // register device with given (major + initial minor) combination
    // and specify how much minor numbers are available for device
    result->error = cdev_add(&hcdev, result->value_dev_t, DEVICE_MINOR_COUNT);
    result->what = "cdev_add()";
}


static void  
ootm_delete_cdev()
{
    cdev_del(&hcdev);
}


static void 
ootm_create_device_nodes(ootm_result *result)
{
    /*
        Create device class and it's instances
    */
    
    (void) result;  // temporarily unused

    int      minor = DEVICE_MINOR_FIRST; 
    dev_t    dev = 0;

    // Create device class itself
    devclass = class_create(THIS_MODULE, DEVICE_CLASS_NAME);
    
    // Create class instances.
    // Corresponding device nodes will be created automatically
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
}


static void 
ootm_destroy_device_nodes()
{
    for (int minor = DEVICE_MINOR_FIRST; minor < DEVICE_MINOR_COUNT; ++minor)
    {
        device_destroy(devclass, MKDEV(major, minor));
    }

    class_destroy(devclass);
}
