#include <linux/init.h>
#include <linux/version.h>
#include <common/include/headers.h>
#include "ootm_main.h"


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static int fops_open_(struct inode* inode, struct file* file);
static int fops_release_(struct inode* inode, struct file* file);
static long fops_ioctl_(struct file *f, unsigned int cmd, unsigned long arg);


/* --------------------------------------------------------- */
/*               M O D U L E   P A R A M S                   */
/* --------------------------------------------------------- */


static int major = 200;    // default value is necessary - can't get it automatically in this approach
module_param(major, int, S_IRUGO);


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


static const struct file_operations fops =
{
    .owner           =  THIS_MODULE,
    .open            =  fops_open_,
    .release         =  fops_release_,
    .read            =  fops_read,
    .unlocked_ioctl  =  fops_ioctl_
};


static const char str[] = "Hello, ioctl!";
static int size = sizeof(str);


/* --------------------------------------------------------- */
/*             M O D U L E   M A N A G E M E N T             */
/* --------------------------------------------------------- */


static int 
__init 
ootm_main_init(void)
{
    GREETING();

    int ret = register_chrdev(major, DEVICE_NAME, &fops);
    if (ret < 0) {
      printk(KERN_ERR "Can not register char device. Major=%u\n", major);
    }

    return ret;
}


static void 
__exit 
ootm_main_exit(void)
{
    FAREWELL();

    unregister_chrdev(major, DEVICE_NAME);

    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static int 
fops_open_(struct inode* inode, struct file* file) 
{
   printk(KERN_INFO "fops_open_ = %u:%u\n", major, iminor(inode));
   return 0;
}


static int fops_release_(struct inode* inode, struct file* file) 
{
   printk(KERN_INFO "fops_close_ = %u:%u\n", major, iminor(inode));
   return 0;
}


static long 
fops_ioctl_(struct file *f, unsigned int cmd, unsigned long arg)
{
    int      rc;
    void    *dest;
    long     result = IOCTL_SUCCESS;
     
    if (_IOC_TYPE(cmd) != IOCTL_MAGIC) 
    {
        printk(KERN_ERR "Error: _IOC_TYPE is wrong\n");
        result = -EINVAL;
        goto exit;
    }
    
    switch (cmd)
    {
    case IOCTL_GET_STRING:
    {
        dest = (void *)arg;
        rc = copy_to_user(dest, str, size);
        if (rc != 0) 
        {
            printk(KERN_ERR "Error: copy_to_user\n");
            result = -EFAULT;
            goto exit;
        } 
    }
    break; 

    default:
    {
        printk(KERN_ERR "Error: unknown _IOC_NR or incorrect _IOC_SIZE\n");
        result = -ENOTTY;
        goto exit;
    }
    }

exit:

    return result;
}
