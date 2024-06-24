#!/bin/bash


source "../../tests/error.sh"			
 

function run_test()
{
    make distclean
    
    local result=
    local node=

    make
    check_result $? crcb_just_exit $ERR_MAKE_BUILD

	insmod ootm.ko
    check_result $? crcb_just_exit $ERR_INSMOD

    minors=(0 1 2)
    for minor in ${minors[*]}
    do
        node="/dev/ootm_$minor"
        cat $node
        check_result $? crcb_just_exit $ERR_CAT        
    done

    rmmod ootm
    check_result $? crcb_just_exit $ERR_CRIT

    make distclean
    check_result $? crcb_just_exit $ERR_MAKE_DISTCLEAN

    exit $ERR_OK
}


run_test
