#!/bin/bash


source "../../tests/errcode.sh"


PENDING_ERROR=$ERR_OK


IS_OOTM_INSERTED=false
IS_OOTM_CALL_INSERTED=false


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
    insmod ootm_call.ko

    # module insertion must cause error
    if [ $? == 0 ]     
    then 
        rc_create_retcode $FSM_ERRC $ERR_DISAPPEAR
        retcode=$?
        rmmod ootm_call    # module removal will succeed (restore system state)
        return $retcode
    fi
    
    insmod ootm.ko

    if [ $? != 0 ] 
    then 
        rc_create_retcode $FSM_ERRC $ERR_INSMOD
        return $?
    else
        IS_OOTM_INSERTED=true
    fi
    
    insmod ootm_call.ko

    if [ $? != 0 ] 
    then 
        rc_create_retcode $FSM_ERRC $ERR_INSMOD
        return $?
    else 
        rc_create_retcode $FSM_NEXT
        retcode=$?
        IS_OOTM_CALL_INSERTED=true
        return $retcode
    fi
}


function remove()
{
    if [ $IS_OOTM_INSERTED == true ]
    then
        rmmod ootm

        if [ $? == 0 ] 
        then 
            rc_create_retcode $FSM_ERRC $ERR_CRIT
            return $?
        fi
    fi
    
    if [ $IS_OOTM_CALL_INSERTED == true ]
    then
        rmmod ootm_call

        if [ $? != 0 ] 
        then 
            rc_create_retcode $FSM_ERRC $ERR_CRIT
            return $?
        else 
            IS_OOTM_CALL_INSERTED=false
        fi
    fi

    if [ $IS_OOTM_INSERTED == true ]
    then
        rmmod ootm

        if [ $? != 0 ] 
        then 
            rc_create_retcode $FSM_ERRC $ERR_CRIT
            return $?
        else 
            IS_OOTM_INSERTED=false
        fi
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
        rc_create_retcode $FSM_GOTO 3
        return $?
    fi

    rc_create_retcode $FSM_STOP $ERR_OK
    return $?
}


#                               0       1     2      3      4
declare -a fsm_test_functions=( prepare build insert remove clean )
