#!/bin/bash


source "errcode.sh"


# ----------------------------------------------------------- #
#                  S T A T I C   D A T A                      #
# ----------------------------------------------------------- #


currentDir=`pwd`
sourcesDirRelative="$currentDir/../src"
testName="test.sh"

successTests=()
failureTests=()

shopt -s nullglob       # prevent empty ../src/*/ result specific loop behavior
shopt -u dotglob        # exclude hidden directories from being itarated over

RED='\033[0;31m'
BLUE='\033[0;34m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RESTORE='\033[0m'


# ----------------------------------------------------------- #
#              S T A T I C   F U N C T I O N S                #
# ----------------------------------------------------------- #


function print_help()
{
    echo "Usage:"
    echo "  ./run.sh            run all tests inside ROOT/src/*/ directories"
    echo "  ./run.sh <dir>      run single test inside ROOT/src/<dir>/"
}


function run_all_tests()
{
    echo -e "Start tests\n"
    
    cd $sourcesDirRelative
    sourcesDirAbsolute=`pwd`
    sourcesDirs=()
    arr=()

    for dir in "$sourcesDirAbsolute/*/"
    do      
        sourcesDir+=$dir
    done

    for dir in ${sourcesDir[*]};
    do
        testFile="$dir/$testName" 
        if [ -f $testFile ];
        then
            run_test $dir
            result=$?
            test_if_critical $result $dir
            push_result $result $dir
        fi 
    done

    print_results
    cd 
}


function run_single_test()
{
    cd $sourcesDirRelative
    sourcesDirAbsolute=`pwd`

    testDir=$sourcesDirAbsolute/$1
    testFile="$testDir/$testName"
    
    if [ -f $testFile ];
    then run_test $testDir
    else 
        echo -e ${YELLOW}
        echo "No $testDir dir or file $testDir/$testName does not exist"
        echo -e ${RESTORE}
    fi 

    cd
}


function run_test()
{
    tmp=`pwd`
    cd $1 
    print_test_begin
    
    source $testName

    test_result=$ERR_OK
        
    stop=false
    idx=0
    prev_idx=$idx
    count=${#fsm_test_functions[@]}
    
    result_type=$ERR_CODE
    result_value=$ERR_OK

    while [ $stop != true ] && [ $idx != $count ]
    do

        echo -e "${BLUE}${fsm_test_functions[$idx]}${RESTORE}"

        ${fsm_test_functions[$idx]} $prev_idx $result_type $result_value
        result=$?
        prev_idx=$idx

        rc_retrieve_type $result
        result_type=$?

        rc_retrieve_value $result
        result_value=$?

        if [ $result_type == $ERR_CODE ]
        then
            
            # If current FSM function returns ERR_CODE constant
            # it means that critical error occured so that entire 
            # test flow must be stopped. 
            #
            # To indicate not critical error use:
            #       rc_create_retcode $FSM_ERRC $ERR_<error>

            test_result=$ERR_CRIT
            break;
        
        else

            case $result_type in

                $FSM_NEXT)
                    idx=$((idx+1))
                    ;;

                $FSM_GOTO)
                    idx=$result_value
                    ;;

                $FSM_STOP)
                    test_result=$result_value
                    stop=true
                    ;;

                $FSM_ERRC)
                    if [ $result_value == $ERR_CRIT ] 
                    then
                        test_result=$ERR_CRIT
                        break
                    fi
                    idx=$((count-1))
                    ;;

                *)
                    echo "unknown fsm signal"
                    test_result=$ERR_CRIT
                    break
                    ;;
        
            esac
        
        fi

    done

    print_test_end $test_result
    cd $tmp
    return $test_result    
}


function test_if_critical()
{
    if [ $1 -eq $ERR_CRIT ]
    then
        echo -e "${RED}CRITICAL ERROR ... $2${RESTORE}"
        echo -e "${YELLOW}Check testing log and restore OS state${RESTORE}"
        exit $ERR_OK
    fi
}


function push_result()
{
    if [ $1 -eq $ERR_OK ];
    then   
        successTests+=" $2"
    else    
        failureTests+=" $2"
    fi
}


function print_results()
{
    echo -e "${GREEN}[ SUCCESS ]"
    for test in $successTests
    do
        echo "   $test"
    done
    echo -e ${RESTORE}


    echo -e "${RED}[ FAILURE ]"
    for test in $failureTests
    do
        echo "   $test"
    done
    echo -e ${RESTORE}
}


function print_test_begin()
{
    echo -e "${YELLOW}[ TEST ] ........ `pwd`${RESTORE}"
}


function print_test_end()
{
    local result=

    if [ $1 -eq $ERR_OK ]
    then
        result="${GREEN} ok ${RESTORE}"
        echo -e "[ $result ]\n"
    else
        result="${RED}fail${RESTORE}"
        errcode="${RED}${ERR_STRINGS[$1]}${RESTORE}"
        echo -e "[ $result ] ........ ${errcode}\n"
    fi
}


# ----------------------------------------------------------- #
#                          M A I N                            #
# ----------------------------------------------------------- #


case $# in

    1)
        if [ "$1" = "--help" ]
        then 
            print_help
            exit 0
        fi

        if [ "$1" = "all" ]
            then  run_all_tests
            else  run_single_test $1
        fi

        ;;

    *)
        echo "Wrong args number"
        print_help
        ;;

esac
