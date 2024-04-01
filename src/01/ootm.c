#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("<license>");
MODULE_AUTHOR("<name> <email>");


/*
    1. Necessary function which is called while loading module
    2. Logging via printk() API:
       [https://docs.kernel.org/core-api/printk-formats.html]
*/
static int 
__init 
ootm_init(void)
{
    size_t i, size;

    const char* bitDepth = IS_ENABLED(CONFIG_64BIT)
    ? "64-bit"
    : "32-bit"
    ;
    
    static const char* llvl[] =
    {
        KERN_EMERG,
        KERN_ALERT,
        KERN_CRIT, 
        KERN_ERR,     
        KERN_WARNING, 
        KERN_NOTICE,  
        KERN_INFO, 
        KERN_DEBUG   
    };

    // specify log level: printk(<lvl> <fmt>, <args>)
    printk(KERN_NOTICE "Hello, world! (OS bit depth: %s)\n", bitDepth);

    // print all kernel log levels   
    size = sizeof(llvl) / sizeof(llvl[0]);
    for (i = 0; i < size; ++i) {
        printk("log level: bin=%s, val=%ld\n", llvl[i], i);
    }

    // return value to indicate if init succeeded
    return 0;
}


/*
    Optional function which is called while removing module.
    If this function is abcent module becomes permanet after
    loading and might be removed only while reboot:
        $ insmod <module>.ko
        $ lsmod | grep <module>  // ... Used by: 0 [permanent] ...
*/ 
static void 
__exit
ootm_exit(void)
{
    printk("Exiting on: %pS\n", ootm_exit);    // using %pS - one of printk unique specifiers 
    printk("Goodbye, world!\n");
    
    // no return value so diagnostic is unavailable
    return;    
}


/*
    Necessary macros to register init and exit functions.
    To see hw they works read macros definition or execute:
        $ insmod <module>.ko
        $ readelf -s <module>.ko | grep FUNC
        $ rmmod <module>
*/
module_init(ootm_init);
module_exit(ootm_exit);
