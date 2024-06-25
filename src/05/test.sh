#!/bin/bash


source "../../tests/error.sh"


function prepare_test()
{
    make distclean
}


function run_test()
{    
    local result=
    local major=
    local node=

    make
    check_result $? crcb_just_exit $ERR_MAKE_BUILD

	insmod ootm.ko
    check_result $? crcb_just_exit $ERR_INSMOD

    major=`cat /sys/module/ootm/parameters/major`
    check_result $? crcb_just_exit $ERR_CAT

    minors=(0 1 2)
    for minor in ${minors[*]}
    do
        node="/dev/ootm$minor"
        
        mknod -m006 /dev/ootm${minor} c $major ${minor}
        check_result $? crcb_just_exit $ERR_MKNOD

        cat $node
        check_result $? crcb_just_exit $ERR_CAT        
    done

	rm /dev/ootm*			     
    check_result $? crcb_just_exit $ERR_CRIT

    rmmod ootm
    check_result $? crcb_just_exit $ERR_CRIT

    make distclean
    check_result $? crcb_just_exit $ERR_MAKE_DISTCLEAN

    exit $ERR_OK
}


prepare_test
run_test
