#include "open.h"


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


static int device_open_counter = 0;


/* --------------------------------------------------------- */
/*                A P I   F U N C T I O N S                  */
/* --------------------------------------------------------- */


int 
fops_open_exclusive(struct inode* inode, struct file* file)
{
    if (0 != device_open_counter)
    {
        return -EBUSY;
    }

    device_open_counter = 1;

    return EOK;
}


int 
fops_release_exclusive(struct inode* inode, struct file* file)
{
    device_open_counter = 0;

    return EOK;
}
