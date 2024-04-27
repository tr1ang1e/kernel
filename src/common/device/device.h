#ifndef DEVICE_H
#define DEVICE_H


#include <linux/device.h>


/* --------------------------------------------------------- */
/*                        M A C R O S                        */
/* --------------------------------------------------------- */


#define MODULE_NAME             "ootm"
#define DEVICE_NAME             MODULE_NAME "_cdev"
#define DEVICE_CLASS_NAME       DEVICE_NAME "_class"
#define DEVICE_INSTANCE_NAME    MODULE_NAME
#define DEVICE_MINOR_FIRST      0
#define DEVICE_MINOR_COUNT      3


/* --------------------------------------------------------- */
/*                A P I   F U N C T I O N S                  */
/* --------------------------------------------------------- */


/// @brief 
/// @param dev 
void ootm_dev_keep_dev(dev_t dev);


/// @brief 
/// @return 
dev_t ootm_dev_peek_dev();


#endif // DEVICE_H
