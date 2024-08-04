#!/bin/bash


source "../../tests/errcode.sh"


PENDING_ERROR=$ERR_OK


NODE="/dev/ioctl"


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
    majors=(200 201 202 203 204 205)
    for major in ${majors[*]}
    do
        insmod ootm.ko major=$major

        if [ $? == 0 ] 
        then
            rc_create_retcode $FSM_NEXT
            return $?
        fi
    done

    rc_create_retcode $FSM_ERRC $ERR_INSMOD
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

    minor=200

    mknod -m006 $NODE c $major $minor
    if [ $? != 0 ] 
    then
        rc_create_retcode $FSM_ERRC $ERR_MKNOD
        return $?
    fi

    rc_create_retcode $FSM_NEXT
    return $?
}


function ioctl()
{
    text=`./ioctl_test.out`

    if [ $? != 0 ] || [ "$text" != "Hello, ioctl!" ]
    then
        rc_create_retcode $FSM_ERRC $ERR_EXEC
        return $?
    fi

    rc_create_retcode $FSM_NEXT
    return $?
}


function remove()
{
    rm $NODE

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
        rc_create_retcode $FSM_GOTO 5
        return $?
    fi

    rc_create_retcode $FSM_STOP $ERR_OK
    return $?
}


#                               0       1     2      3         4     5      6
declare -a fsm_test_functions=( prepare build insert makenodes ioctl remove clean )
