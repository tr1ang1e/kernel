#include "device.h"


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


static dev_t device = -1;


/* --------------------------------------------------------- */
/*                A P I   F U N C T I O N S                  */
/* --------------------------------------------------------- */


void 
ootm_dev_keep_dev(dev_t dev)
{
    if (unlikely(-1 != device))
    {
        return;
    }

    device = dev;
    return;
}


dev_t 
ootm_dev_peek_dev()
{
    return device;
}
