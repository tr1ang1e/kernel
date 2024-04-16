#ifndef OPEN_H
#define OPEN_H


#include <linux/fs.h>
#include <common/macros.h>


int fops_open(struct inode* inode, struct file* file);

int fops_release(struct inode* inode, struct file* file);


#endif // OPEN_H
