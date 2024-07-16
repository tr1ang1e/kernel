#!/bin/bash


source "../../tests/errcode.sh"


PENDING_ERROR=$ERR_OK


function prepare()
{
    make clean

    rc_create_retcode $FSM_NEXT
    return $?
}


function build()
{
    make build

    if [ $? != 0 ] 
    then rc_create_retcode $FSM_ERRC $ERR_MAKE_BUILD
    else rc_create_retcode $FSM_NEXT
    fi
    return $?
}


function insert()
{
    insmod ootm.ko

    if [ $? != 0 ] 
    then rc_create_retcode $FSM_ERRC $ERR_INSMOD
    else rc_create_retcode $FSM_NEXT
    fi
    return $?
}


function remove()
{
    rmmod ootm

    if [ $? != 0 ] 
    then rc_create_retcode $FSM_ERRC $ERR_CRIT
    else rc_create_retcode $FSM_NEXT
    fi
    return $?
}


function clean()
{
    make clean

    if [ $PENDING_ERROR != $ERR_OK ]
    then
        rc_create_retcode $FSM_STOP $PENDING_ERROR
        return $?
    fi

    if [ $2 == $FSM_ERRC ]
    then
        PENDING_ERROR=$3
        rc_create_retcode $FSM_GOTO 3
        return $?
    fi

    rc_create_retcode $FSM_STOP $ERR_OK
    return $?
}


#                               0       1     2      3      4
declare -a fsm_test_functions=( prepare build insert remove clean )
