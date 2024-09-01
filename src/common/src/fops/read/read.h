#ifndef READ_H
#define READ_H


#include <linux/fs.h>
#include <linux/uaccess.h>
#include <common/src/basic/macros.h>


/// @brief Callback used from user-space to read data from device
/// @param file - file structure associated with file descriptor got when open() device
/// @param buf - buffer for data to be copied to
/// @param count - expected bytes number to be copied to buffer
/// @param ppos - pointer to keep 
/// @return - actually copied bytes if success, negative error code otherwise
/// If not all planned bytes are copied, the function doesn't do attempt 
/// to copy remaining bytes and just returns error code as no bytes are copied.
ssize_t fops_read(struct file* file, char* buf, size_t count, loff_t* ppos);


#endif // READ_H
