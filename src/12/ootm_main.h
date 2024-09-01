#ifndef OOTM_MAIN_H
#define OOTM_MAIN_H


// This file is just to provide interface 
// for test program to use the valid data


#define DEVICE_NODE     "ootm"
#define BUF_SIZE         256

typedef struct 
{
    char buf[BUF_SIZE];
    int status;
} ootm_data;


#endif // OOTM_MAIN_H
