#!/bin/bash


source "error.sh"


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
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RESTORE='\033[0m'


# ----------------------------------------------------------- #
#              S T A T I C   F U N C T I O N S                #
# ----------------------------------------------------------- #


run_test()
{
    tmp=`pwd`
    cd $1 
    
    print_test_begin
    eval "./$testName"
    result=$?
    print_test_end $result

    cd $tmp
    return $result
}


test_if_critical()
{
    if [ $1 -eq $ERR_CRIT ]
    then
        echo -e "${RED}CRITICAL ERROR ... $2${RESTORE}"
        echo -e "${YELLOW}Check testing log and restore OS state${RESTORE}"
        exit $ERR_OK
    fi
}


push_result()
{
    if [ $1 -eq 0 ];
    then   
        successTests+=" $2"
    else    
        failureTests+=" $2"
    fi
}


print_results()
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


print_test_begin()
{
    echo -e "${YELLOW}[ TEST ] ........ `pwd`${RESTORE}"
}


print_test_end()
{
    local result=

    if [ $1 -eq 0 ]
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


run_all_tests()
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


run_all_tests
