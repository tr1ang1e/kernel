
# ----------------------------------------------------------- #
#                  S T A T I C   D A T A                      #
# ----------------------------------------------------------- #


# TODO: implement enum approach instead of explicitely assign error values and write strings
# [see https://stackoverflow.com/questions/21849328/enum-data-type-seems-not-available-in-bash]

# Bash routines are able to return only integers in range 0..255 and
# moreover global variables managing is sophisticated. So for errors
# handling and fsm managing purposes packing two values into the one
# is pritty convinient. It's done as follows:
# 
# return type       bits 7..4       000 = error, 001..111 = fsm signal
# return value      bits 4..0       error value or fsm signal data (range 0..31)


# --------------------- E R R O R --------------------------- #


ERR_CODE=0

# Available error codes are only 0..31
# Reserve following ranges:
#     0 .. 7   ->  special error codes
#     8 .. 31  ->  regular error codes

# SPECIAL ERROR CODES
ERR_OK=0                # not actually an error
ERR_DISAPPEAR=1         # error should occur but actually did not
ERR_CRIT=2              # critical error and immidiate stop
# =3
# =4
# =5
# =6
# =7

# REGULAR ERROR CODES
ERR_MAKE_BUILD=8
ERR_MAKE_DISTCLEAN=9
ERR_INSMOD=10
ERR_MODPROBE=11
ERR_CALL=12
ERR_MKNOD=13
ERR_VALUE=14
ERR_CAT=15
ERR_GETDATA=16
ERR_EXEC=17

# SPECIAL AND REGUAR ERROR STRING REPRESENTATIONS
ERR_STRINGS=(
    [$ERR_OK]="ERR_OK"
    [$ERR_DISAPPEAR]="ERR_DISAPPEAR"
    [$ERR_CRIT]="ERR_CRIT"
    [$ERR_MAKE_BUILD]="ERR_MAKE_BUILD"
    [$ERR_MAKE_DISTCLEAN]="ERR_MAKE_DISTCLEAN"
    [$ERR_INSMOD]="ERR_INSMOD"
    [$ERR_MODPROBE]="ERR_MODPROBE"
    [$ERR_MKNOD]="ERR_MKNOD"
    [$ERR_VALUE]="ERR_VALUE"
    [$ERR_CAT]="ERR_CAT"
    [$ERR_GETDATA]="ERR_GETDATA"
    [$ERR_EXEC]="ERR_EXEC"
)


# ----------------------- F S M ----------------------------- #


FSM_NEXT=1
FSM_STOP=2
FSM_GOTO=3      # goto function by its index in functions array
FSM_ERRC=4      # all errors are the same as for ERR_CODE
# =5
# =6 
# =7


# ----------------------------------------------------------- #
#                 A P I   F U N C T I O N S                   #
# ----------------------------------------------------------- #


function rc_retrieve_type()
{
    type=$1
    type=$((type>>5))
    return $type
}


function rc_retrieve_value()
{
    value=$1
    value=$((value&31))
    return $value
}


function rc_create_retcode()
{
    type=$1
    type=$((type<<5))

    value=0
    if [ $# -eq 2 ]
    then
        value=$2
    fi

    retcode=$((type+value))
    return $retcode
}
