#!/bin/bash


# get major value automatically as it might
# be assigned to module also automatically
DEVICE=`cat /proc/devices | grep ootm_cdev`
TOKENS=( $DEVICE )
MAJOR=${TOKENS[0]}


# the number of nodes must be the same (or less, but not greater)
# than minor values registered is source code by *_chrdev_region()
mknod -m006 /dev/ootm0 c $MAJOR 0   
mknod -m006 /dev/ootm1 c $MAJOR 1
mknod -m006 /dev/ootm2 c $MAJOR 2   
