#include <linux/export.h>


/* --------------------------------------------------------- */
/*             E X T E R N   F U N C T I O N S               */
/* --------------------------------------------------------- */


const char* get_greeting(void)
{   
    return "Hello, world!";
}


/*
    EXPORT_SYMBOL macro makes symbol, which is passed
    as an argument, accessible not only for module which
    was compiled together with this source file to a 
    common .ko object. But also for every module which
    is load after this one - because when this module
    is load, the symbol makes accessible as if it's one 
    of the kernel itslef symbols.

    $ nm <module>.ko | grep T    // get exported symbols
    [see man 1 nm]
    [see Module.symvers]
*/
EXPORT_SYMBOL(get_greeting);
