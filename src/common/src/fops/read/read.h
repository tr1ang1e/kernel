#ifndef READ_H
#define READ_H


#include <linux/fs.h>
#include <linux/uaccess.h>
#include <common/src/basic/macros.h>


/// @brief 
/// @param file 
/// @param buf 
/// @param count 
/// @param ppos 
/// @return 
ssize_t fops_read(struct file* file, char* buf, size_t count, loff_t* ppos);


#endif // READ_H
