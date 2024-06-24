
# ----------------------------------------------------------- #
#                  S T A T I C   D A T A                      #
# ----------------------------------------------------------- #


# TODO: implement enum approach instead of explicitely assign error values and write strings
# [see https://stackoverflow.com/questions/21849328/enum-data-type-seems-not-available-in-bash]


ERR_OK=0
ERR_DISAPPEAR=254       # error should occur but actually did not
ERR_CRIT=255            # 255 is the max allowed retcode


ERR_MAKE_BUILD=1
ERR_MAKE_DISTCLEAN=2
ERR_INSMOD=3
ERR_MODPROBE=4
ERR_CAT=5
ERR_MKNOD=6


ERR_STRINGS=(
    [$ERR_OK]="ERR_OK"
    [$ERR_MAKE_BUILD]="ERR_MAKE_BUILD"
    [$ERR_MAKE_DISTCLEAN]="ERR_MAKE_DISTCLEAN"
    [$ERR_INSMOD]="ERR_INSMOD"
    [$ERR_MODPROBE]="ERR_MODPROBE"
    [$ERR_CAT]="ERR_CAT"
    [$ERR_MKNOD]="ERR_MKNOD"
    [$ERR_DISAPPEAR]="ERR_DISAPPEAR"
    [$ERR_CRIT]="ERR_CRIT"
)


# ----------------------------------------------------------- #
#                 A P I   F U N C T I O N S                   #
# ----------------------------------------------------------- #


# CHECK COMMAND RESULT ($1) AND CALL CALLBACK ($2) WITH GIVEN ERRCODE ($3)
check_result()
{
    local result=$1
    if [ $result -ne 0 ]
    then
        echo "SHELL RETURNED ERROR: $1"
        $2 $3
    fi
}


# JUST EXIT FROM TEST SCRIPT WITHOUT ANY CLEANUP OPERATIONS
crcb_just_exit()
{
    exit $1
}


# EXIT ONLY IF THERE IS NO ERROR, BECAUSE ERROR OCCURANCE IS AN EXPECTED BEHAVIOR 
crcb_error_expected()
{
    if [ $1 -eq 0 ]
    then
        exit $ERR_DISAPPEAR
    fi
}
