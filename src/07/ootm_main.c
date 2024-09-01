#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <common/include/headers.h>


/* --------------------------------------------------------- */
/*               M O D U L E   P A R A M S                   */
/* --------------------------------------------------------- */


static int major = 0;
module_param(major, int, S_IRUGO);


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


static struct file_operations fops =
{
    .owner    = THIS_MODULE,
    .open     = fops_open_exclusive,
    .release  = fops_release_exclusive,
    .read     = fops_read
};


static struct class *devclass = NULL;       // device class handler
static struct cdev hcdev = { 0 };           // char device instance


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static  void  ootm_dev_create_cdev_region(ootm_result *result);                 // Create character device region: custom or auto
static  void  ootm_dev_destroy_cdev_region(dev_t dev, int minor_count);         // Destriy character device region

static  void  ootm_dev_add_cdev(dev_t dev, ootm_result *result);                // Initialize and add character device
static  void  ootm_dev_delete_cdev();                                           // Delete character device

static  void  ootm_dev_create_device_nodes(dev_t dev, ootm_result *result);     // Create device class and it's instances
static  void  ootm_dev_destroy_device_nodes(dev_t dev);                         // Destriy device class and it's instances


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

    ootm_dev_create_cdev_region(&result);
    if (result.error) 
    {
        printk(KERN_ERR "Error: %s\n", result.message);
        goto exit;
    }

    // keep global data which no longer will be changed
    dev = result.value_dev_t;
    ootm_dev_keep_dev(dev);

    // keep module parameter value
    major = MAJOR(dev);

    ootm_dev_add_cdev(dev, &result);
    if (result.error)
    {
        printk(KERN_ERR "Error: %s\n", result.message);
        goto destroy_cdev_region;
    }

    ootm_dev_create_device_nodes(dev, &result);
    if (result.error)
    {
        printk(KERN_ERR "Error: %s\n", result.message);
        goto delete_cdev;
    }


// Initialization is done
// No more actions required, skip cleanup

    printk(KERN_INFO DEVICE_NAME " installed %d:[%d-%d]\n", MAJOR(dev), DEVICE_MINOR_FIRST, MINOR(dev));
    goto exit;


// Cleanup operations are started from here.
// They must be performed in reverse order.

delete_cdev:

    ootm_dev_delete_cdev();

destroy_cdev_region:

    ootm_dev_destroy_cdev_region(dev, DEVICE_MINOR_COUNT);


// Exit point
// Both success and failure exit here

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

    ootm_dev_destroy_device_nodes(ootm_dev_peek_dev());
    
    ootm_dev_delete_cdev();

    ootm_dev_destroy_cdev_region(ootm_dev_peek_dev(), DEVICE_MINOR_COUNT);
    
    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static void   
ootm_dev_create_cdev_region(ootm_result *result)
{   
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

    if (unlikely(ret < 0))
    {
        result->error = ERR_KERN_CALL;
        result->result = ret;
        result->message = "*_chrdev_region()";
    }
    else
    {
        result->error = false;
        result->value_dev_t = dev;
    }

    return;
}


static void
ootm_dev_destroy_cdev_region(dev_t dev, int minor_count)
{
    unregister_chrdev_region(dev, minor_count);

    return;
}


static void 
ootm_dev_add_cdev(dev_t dev, ootm_result *result)
{  
    int ret = 0;

    // initialize char device handler
    cdev_init(&hcdev, &fops);
    hcdev.owner = THIS_MODULE;

    // register device with given (major + initial minor) combination
    // and specify how much minor numbers are available for device
    ret = cdev_add(&hcdev, dev, DEVICE_MINOR_COUNT);

    if (unlikely(ret < 0))
    {
        result->error = ERR_KERN_CALL;
        result->result = ret;
        result->message = "cdev_add()";
    }
    else
    {
        result->error = false;
    }

    return;
}


static void  
ootm_dev_delete_cdev()
{
    cdev_del(&hcdev);
}


static void 
ootm_dev_create_device_nodes(dev_t dev, ootm_result *result)
{
    result->error = false;
    
    int               major = MAJOR(dev);
    int               minor = DEVICE_MINOR_FIRST; 
    struct device    *instance = NULL;
    
    // Create device class itself
    devclass = class_create(THIS_MODULE, DEVICE_CLASS_NAME);
    if (unlikely(IS_ERR(devclass)))
    {
        result->error = ERR_KERN_CALL;
        result->result = (intptr_t) devclass;
        result->message = "class_create()";
        goto exit;
    }
    
    // Create class instances.
    // Corresponding device nodes will be created automatically
    for ( ; minor < DEVICE_MINOR_COUNT; ++minor)
    {
        instance = device_create (devclass, 
                                  NULL, 
                                  MKDEV(major, minor), 
                                  NULL,
                                 "%s_%d", DEVICE_INSTANCE_NAME, minor   // device node name = /dev/"%s_%d" 
        );

        if (unlikely(IS_ERR(instance)))
        {
            // destroy already created devices
            while (minor != DEVICE_MINOR_FIRST)
            {
                --minor;
                device_destroy(devclass, MKDEV(major, minor));
            }

            // destroy already created class
            class_destroy(devclass);
            devclass = NULL;

            result->error = ERR_KERN_CALL;
            result->result = PTR_ERR(instance);
            result->message = "device_create()";
            goto exit;
        }
    }

exit:

    return;
}


static void 
ootm_dev_destroy_device_nodes(dev_t dev)
{
    int major = MAJOR(dev);
    
    for (int minor = DEVICE_MINOR_FIRST; minor < DEVICE_MINOR_COUNT; ++minor)
    {
        device_destroy(devclass, MKDEV(major, minor));
    }

    class_destroy(devclass);
}
