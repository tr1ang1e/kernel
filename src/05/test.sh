#!/bin/bash


source "../../tests/errcode.sh"


PENDING_ERROR=$ERR_OK


function prepare()
{
    make distclean

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


function makenodes()
{
    local major=
    local node=

    major=`cat /sys/module/ootm/parameters/major`
    if [ $? != 0 ] 
    then
        rc_create_retcode $FSM_ERRC $ERR_GETDATA
        return $?
    fi

    minors=(0 1 2)
    for minor in ${minors[*]}
    do
        node="/dev/ootm$minor"
        
        mknod -m006 /dev/ootm${minor} c $major ${minor}
        if [ $? != 0 ] 
        then
            rc_create_retcode $FSM_ERRC $ERR_MKNOD
            return $?
        fi

        cat $node
        if [ $? != 0 ] 
        then
            rc_create_retcode $FSM_ERRC $ERR_CAT
            return $?
        fi
    done

    rc_create_retcode $FSM_NEXT
    return $?
}


function remove()
{
    rm /dev/ootm*

    if [ $? != 0 ]
    then
        rc_create_retcode $FSM_ERRC $ERR_CRIT
        return $?
    fi
    
    rmmod ootm
		     
    if [ $? != 0 ]
    then
        rc_create_retcode $FSM_ERRC $ERR_CRIT
        return $?
    fi

    rc_create_retcode $FSM_NEXT
    return $?
}


function clean()
{
    make distclean

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
declare -a fsm_test_functions=( prepare build insert makenodes remove clean )
