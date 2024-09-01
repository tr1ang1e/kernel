#ifndef OPEN_H
#define OPEN_H


#include <linux/fs.h>
#include <common/src/basic/macros.h>


/// @brief Callback to be called when open() is called from user-space
/// @param inode - inode of device file
/// @param file - file structure associated with file descriptor got when open() device
/// @return EOK
int fops_open_exclusive(struct inode* inode, struct file* file);


/// @brief Callback to be called when close() is called from user-space
/// @param inode - inode of device file
/// @param file - file structure associated with file descriptor got when open() device
/// @return EOK
int fops_release_exclusive(struct inode* inode, struct file* file);


#endif // OPEN_H
