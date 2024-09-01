#ifndef DEVICE_H
#define DEVICE_H


#include <linux/device.h>


/* --------------------------------------------------------- */
/*                        M A C R O S                        */
/* --------------------------------------------------------- */


#define MODULE_NAME             "ootm"                      // [: module_name :]
#define DEVICE_NAME             MODULE_NAME "_cdev"         // [: device_name :]
#define DEVICE_CLASS_NAME       DEVICE_NAME "_class"
#define DEVICE_INSTANCE_NAME    MODULE_NAME
#define DEVICE_MINOR_FIRST      0
#define DEVICE_MINOR_COUNT      3


/* --------------------------------------------------------- */
/*                A P I   F U N C T I O N S                  */
/* --------------------------------------------------------- */


/// @brief Save dev_t value made of <MAJOR> + <MINOR_0> values
/// @param dev - value to be saved
void ootm_dev_keep_dev(dev_t dev);


/// @brief Get previously saved dev_t value
/// @return defv_t value
dev_t ootm_dev_peek_dev(void);


#endif // DEVICE_H
