#include <linux/init.h>
#include <linux/miscdevice.h>
#include <common/include/headers.h>
#include "ootm_main.h"


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static int fops_open_(struct inode* inode, struct file* file);
static int fops_release_(struct inode* inode, struct file* file);
static ssize_t fops_read_(struct file* file, char* dest, size_t dsize, loff_t* ppos);
static ssize_t fops_write_(struct file* file, const char* source, size_t ssize, loff_t* ppos);


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


#define DATA_NOT_COPIED     0
#define DATA_COPIED         1


static const struct file_operations fops =
{
    .owner    = THIS_MODULE,
    .open     = fops_open_,
    .release  = fops_release_,
    .read     = fops_read_,
    .write    = fops_write_
};


static struct miscdevice misc_dev =
{
    .minor    = MISC_DYNAMIC_MINOR,
    .name     = DEVICE_NODE,
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

    int rc = misc_register(&misc_dev);
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


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static int 
fops_open_(struct inode* inode, struct file* file) 
{
    printk(KERN_INFO "fops_open_: inode=%p, file=%p\n", inode, file);

    ootm_data* data;
    
    file->private_data = kmalloc(sizeof(ootm_data), GFP_KERNEL);
    if (NULL == file->private_data) {
        printk(KERN_ERR "Error: %s\n", "failed to kmalloc()");
        return -ENOMEM;
    }

    data = (ootm_data*)file->private_data;
    data->status = DATA_NOT_COPIED;
    strcpy(data->buf, "none");

    return EOK;
}


static int fops_release_(struct inode* inode, struct file* file) 
{
    printk(KERN_INFO "fops_close_: inode=%p, file=%p\n", inode, file);

    kfree(file->private_data);

    return EOK;
}


static ssize_t 
fops_read_(struct file* file, char* dest, size_t dsize, loff_t* ppos)
{   
    printk(KERN_INFO "fops_read_: file=%p\n", file);
    
    ssize_t result;
    ootm_data* data = (ootm_data*)file->private_data;
    ssize_t len = strlen(data->buf) + 1; 

    do
    {
        if (DATA_COPIED == data->status) 
        {
            data->status = DATA_NOT_COPIED;
            result = EOF;
            break;
        }

        if (dsize < len)
        {
            printk(KERN_ERR "read: invalid destination buffer size\n");
            result = -EINVAL;
            break;
        }

        unsigned long not_copied = copy_to_user(dest, data->buf, len);
        
        if (0 != not_copied) 
        {
            printk(KERN_ERR "read: failed to copy full data\n");
            result = -EINVAL;
            break;
        }

        result = len;

    } while (0);

    return result;
}


static ssize_t 
fops_write_(struct file* file, const char* source, size_t ssize, loff_t* ppos)
{
    printk(KERN_INFO "fops_write_: file=%p\n", file);

    ssize_t result;
    ootm_data* data = (ootm_data*)file->private_data;
    ssize_t len = (ssize < BUF_SIZE) ? ssize : (BUF_SIZE - 1);

    do
    {
        unsigned long not_copied = copy_from_user(data->buf, source, len);
        if (0 != not_copied)
        {
            printk(KERN_ERR "write: failed to copy full data\n");
            result = -EINVAL;
            break;
        }

        data->buf[len] = '\0';
        result = len;

    } while (0);

    return result;
}
