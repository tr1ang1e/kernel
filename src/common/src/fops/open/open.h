#ifndef OPEN_H
#define OPEN_H


#include <linux/fs.h>
#include <common/src/basic/macros.h>


/// @brief 
/// @param inode 
/// @param file 
/// @return 
int fops_open(struct inode* inode, struct file* file);


/// @brief 
/// @param inode 
/// @param file 
/// @return 
int fops_release(struct inode* inode, struct file* file);


#endif // OPEN_H
