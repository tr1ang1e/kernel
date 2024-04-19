#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>


MODULE_LICENSE("<license>");
MODULE_AUTHOR("<name> <email>");


/* --------------------------------------------------------- */
/*            M O D U L E   P A R A M E T E R S              */
/* --------------------------------------------------------- */


/*
    Module paramaters must be registered 
    via one of special macros:

    <type> <name> = <default_value>;
    module_param(<name>, <type>, <perm>);
        name:   var name from code (the param will have the same)
        type:   integer types or 'charp' (C-string)
        perm:   permissions for /sys/module/<module>/parameters/<name>

    char <name>[<size>] = "<default_value>";
    module_param_string(<param>, <name>, <size>, <perm>);
        param:  parameter name as it will be exported outside
        name:   var name from code (the param will have the same)
        size:   const value of the char array sze
        perm:   permissions for /sys/module/<module>/parameters/<name>    

    <type> <name>[] = { <default_values> };
    int size = sizeof(<name>) / sizeof(<name>[0]); 
    module_param_array(<name>, <type>, &size, <perm>)
        name:   var name from code (the param will have the same)
        type:   integer types or 'charp' (C-string)
        size:   pointer to var which keeps array size:
                    - will be modified if less values are passed
                    - will cause error or default value if more values are passed 
        perm:   permissions for /sys/module/<module>/parameters/<name>

    [: modprobe.d :]
*/


// If fans should be turned on
static bool fan = false;
module_param(fan, bool, S_IRUGO);


// Tempretature upon reaching which it is necessary to turn fans on
#define TEMPLEN 5
static char temperature[TEMPLEN] = "40.0";
module_param_string(temp, temperature, TEMPLEN, S_IRUGO);


// GPIO pins which fans are connected to
static int pins[] = {41, 42, 43};
static int pinsnum = sizeof(pins) / sizeof(pins[0]);
module_param_array(pins, int, &pinsnum, S_IRUGO);


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static void ootm_printk_options(void);
static void ootm_printk_kernel_config(void);
static void ootm_printk_module_refcount(void);


/* --------------------------------------------------------- */
/*        M O D U L E   I N I T   a n d    E X I T           */
/* --------------------------------------------------------- */


static int 
__init 
ootm_init(void)
{
    printk("Hello, world!\n");

    ootm_printk_options();
    ootm_printk_kernel_config();
    ootm_printk_module_refcount();
    
    return 0;
}


static void 
__exit
ootm_exit(void)
{
    printk("Goodbye, world!\n");
    return;    
}


module_init(ootm_init);
module_exit(ootm_exit);


/* --------------------------------------------------------- */
/*             S T A T I C   F U N C T I O N S               */
/* --------------------------------------------------------- */


static void 
ootm_printk_options(void)
{
    printk("Following options are set:\n");
    printk("\tfan=%s\n", fan ? "on" : "off");
    if (fan)
    {
        printk("\ttemp=%s'C\n", temperature);
        for (int i = 0; i < pinsnum; ++i)
        {
            if (pins[i])
            {
                printk("\tgpio=%d\n", pins[i]);    
            }
        }
    }
}


static void 
ootm_printk_kernel_config(void)
{
    // run-time
    const char* bitDepth = IS_ENABLED(CONFIG_64BIT)
        ? "64-bit"
        : "32-bit"
        ;
    printk("OS bit depth: %s\n", bitDepth);

    // compile-time
    #if defined(CONFIG_SMP)
        printk("CONFIG_SMP=%d\n", CONFIG_SMP);
    #endif
}


static void 
ootm_printk_module_refcount(void)
{
    // THIS_MODULE is defined as (&__this_module)
    
    printk("Module reference counter: ");
    printk("\tinitial value = %u", module_refcount(THIS_MODULE));
    
    if (try_module_get(THIS_MODULE))
    {
        printk("\tafter get = %u\n", module_refcount(THIS_MODULE));
        module_put(THIS_MODULE);
        printk("\tafter put = %u\n", module_refcount(THIS_MODULE));
    }
    else
    {
        printk("\nFailed to try_module_get()\n");
    }
}
