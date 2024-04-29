#!/bin/bash


source "../../tests/error.sh"


run_test()
{
    make build
    check_result $? crcb_just_exit $ERR_MAKE_BUILD

    insmod ootm.ko temp=36.6
    check_result $? crcb_just_exit $ERR_INSMOD

    cat /sys/module/ootm/parameters/fan
    check_result $? crcb_just_exit $ERR_CAT

    cat /sys/module/ootm/parameters/temp
    check_result $? crcb_just_exit $ERR_CAT

    cat /sys/module/ootm/parameters/pins
    check_result $? crcb_just_exit $ERR_CAT

    rmmod ootm
    check_result $? crcb_just_exit $ERR_CRIT

    make clean
    check_result $? crcb_just_exit $ERR_MAKE_DISTCLEAN

    exit $ERR_OK
}


run_test
