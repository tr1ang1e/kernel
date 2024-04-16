#include "read.h"


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


static const char str[] = "Hello, world!\n";

static int size = sizeof(str);


/* --------------------------------------------------------- */
/*                A P I   F U N C T I O N S                  */
/* --------------------------------------------------------- */


ssize_t 
fops_read(struct file* file, char* dest, size_t dsize, loff_t* ppos)
{
    if (dsize < size)
    {
        // operation can't be performed
        return -EINVAL;
    }
    
    if (*ppos != 0)
    {
        // already sent data which we wanted to send
        printk(KERN_INFO "read: EOF\n");
        return EOF;
    }

    unsigned long not_copied = copy_to_user(dest, str, size);
    if (0 != not_copied)
    {
        // won't do another try
        printk(KERN_ERR "Error: copy_to_user\n");
        return -EINVAL;
    }
    else
    {
        printk(KERN_INFO "read: %d\n", size);
    }

    // set position
    *ppos = size;
    
    // positive value means success
    return size;
}
