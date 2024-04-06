#include <linux/init.h>
#include <linux/module.h>
#include <vdso/limits.h>


MODULE_LICENSE("<license>");
MODULE_AUTHOR("<name> <email>");


/* --------------------------------------------------------- */
/*        M O D U L E   I N I T   a n d    E X I T           */
/* --------------------------------------------------------- */


static void 
asm_sum(long long a, long long b)
{
    bool         overflow;
    long long    sum;

    /*
        Inline assembly syntax:

        asm [qualifiers]
        (
            AssemblerTemplate       // asm code itself in "", instructions are separated by "\n"
            : OutputOperands        // variables to be modified 
            : InputOperands         // expressions to be read
            : Clobbers              // registers or other values are changed beyond those listed as outputs 
            : GotoLabels            // asm can jump only to C goto and only if they are listed here 
        );
    
        The total number of input + output + goto operands is limited to 30
        For details see: https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
        To understand following example search for:
            > ‘r’ and ‘m’ 
            > =@cc
    */

    asm 
    (
        "adds %0 , %2 , %3" 
        : "=r" (sum), "=@ccvs" (overflow) 
        : "r" (a), "r" (b)
    );

    if (overflow)    printk("Signed overflow deteted\n");
    else             printk("Result: %lld+%lld=%lld\n", a, b, sum);
}


static int 
__init 
ootm_init(void)
{ 
    asm_sum(1, 2);
    asm_sum(LLONG_MAX, 2);

    return -1;
}


module_init(ootm_init);
