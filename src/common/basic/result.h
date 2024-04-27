#ifndef RESULT_H
#define RESULT_H


#include <linux/device.h>
#include <linux/types.h>


// [?] In newer kernel versions as well as in old answers on the Internet
// the assumption that Linux required sizeof(void*) == sizeof(long) is made. 
// I have ve failed to find any official doc specfying it. But nevertheless
typedef long    intptr_t;


// Type is used to simplify handling error codes when return value
// not only indicates if error occured but also contain meaningful data.
// 
// It also might be used in call "chains", when meaningful output value 
// of previous function call should be passed as input value into the
// following function call. In this case no need to retreive this value,
// as it might be passed inside this type instance. But not recommended.
typedef struct
{
    // intptr is suitable for storing both pointer and signed values. As 
    // errors in linux kernel are always less than zero, intptr_t is used
    bool           error;      // indicates if error occured
    intptr_t       result;     // return code
    const char*    message;    // possibility to return message
    
    // To avoid type conversions, it is better 
    // to store result values in the exact type
    union 
    {
        int        value_int;
        dev_t      value_dev_t;
        void*      value_ptr;
    };
} ootm_result;


#endif // RESULT_H
