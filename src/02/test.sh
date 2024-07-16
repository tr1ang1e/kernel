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
    insmod ootm.ko temp=36.6

    if [ $? != 0 ] 
    then rc_create_retcode $FSM_ERRC $ERR_INSMOD
    else rc_create_retcode $FSM_NEXT
    fi
    return $?
}


function read_opts()
{
    rc=true

    while :
    do
        opt_fan=$(cat /sys/module/ootm/parameters/fan)
        if [ $? != 0 ]
        then
            rc=false
            break
        fi

        opt_temp=$(cat /sys/module/ootm/parameters/temp)
        if [ $? != 0 ]
        then
            rc=false
            break
        fi

        opt_pins=$(cat /sys/module/ootm/parameters/pins)
        if [ $? != 0 ]
        then
            rc=false
            break
        fi

        break
    done

    if [ $rc != true ]
    then
        rc_create_retcode $FSM_ERRC $ERR_CAT
        return $?
    fi

    opt_all=$(echo $opt_fan $opt_temp $opt_pins)
    if [[ $opt_all != "N 36.6 41,42,43" ]]
    then
        rc_create_retcode $FSM_ERRC $ERR_VALUE
        return $?
    fi

    rc_create_retcode $FSM_NEXT
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
        rc_create_retcode $FSM_GOTO 4
        return $?
    fi

    rc_create_retcode $FSM_STOP $ERR_OK
    return $?
}


#                               0       1     2      3         4      5
declare -a fsm_test_functions=( prepare build insert read_opts remove clean )
