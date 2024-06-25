#!/bin/bash


source "../../tests/error.sh"


function prepare_test()
{
    make distclean
}


function run_test()
{
    local result=

    make
    check_result $? crcb_just_exit $ERR_MAKE_BUILD

    insmod ootm_call.ko
    result=$?
    check_result $result crcb_error_expected $result

	insmod ootm.ko
    check_result $? crcb_just_exit $ERR_INSMOD

    insmod ootm_call.ko
    check_result $? crcb_just_exit $ERR_INSMOD

    rmmod ootm 
    result=$?
    check_result $result crcb_error_expected $result
    
    rmmod ootm_call
    check_result $? crcb_just_exit $ERR_CRIT
    
    rmmod ootm
    check_result $? crcb_just_exit $ERR_CRIT

    make distclean
    check_result $? crcb_just_exit $ERR_MAKE_DISTCLEAN

    exit $ERR_OK
}


prepare_test
run_test
