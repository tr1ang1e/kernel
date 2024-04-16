#include <linux/init.h>
#include <common/author.h>
#include <common/macros.h>


/* --------------------------------------------------------- */
/*                 S T A T I C   D A T A                     */
/* --------------------------------------------------------- */


/* --------------------------------------------------------- */
/*             M O D U L E   M A N A G E M E N T             */
/* --------------------------------------------------------- */


static int 
__init 
ootm_main_init(void)
{
    GREETING();
    return 0;
}


static void 
__exit 
ootm_main_exit(void)
{
    FAREWELL();
    return;
}


module_init(ootm_main_init);
module_exit(ootm_main_exit);
