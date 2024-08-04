#ifndef OOTM_MAIN_H
#define OOTM_MAIN_H


// This file is just to provide interface for
// test program which should use the valid
// ioctl command code and correct argument type


typedef struct 
{
    char buf[256];
} ootm_ioctl_buf_t;


typedef enum
{
    OOTM_IOCTL_CMD_GET_STR = 1
} ootm_ioctl_cmd_e;


#define IOCTL_SUCCESS       0
#define IOCTL_MAGIC         'h'
#define IOCTL_GET_STRING    _IOR(IOCTL_MAGIC, OOTM_IOCTL_CMD_GET_STR, ootm_ioctl_buf_t)


#endif // OOTM_MAIN_H
