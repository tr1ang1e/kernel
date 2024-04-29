#!/bin/bash


source "../../tests/error.sh"


run_test()
{
    make build
    check_result $? crcb_just_exit $ERR_MAKE_BUILD

    insmod ootm.ko
    check_result $? crcb_just_exit $ERR_INSMOD

    rmmod ootm
    check_result $? crcb_just_exit $ERR_CRIT

    make clean
    check_result $? crcb_just_exit $ERR_MAKE_DISTCLEAN

    exit $ERR_OK
}


run_test
