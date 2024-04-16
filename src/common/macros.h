#ifndef MACROS_H
#define MACROS_H


/* --------------------------------------------------------- */
/*                        M O D U L E                        */
/* --------------------------------------------------------- */


#define MODULE_NAME     "ootm"

#define GREETING()      printk("Hello from " MODULE_NAME "!\n");
#define FAREWELL()      printk("Goodbuy from " MODULE_NAME "!\n");


/* --------------------------------------------------------- */
/*                         D E B U G                         */
/* --------------------------------------------------------- */


// final value is computed in run-time
#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


/* --------------------------------------------------------- */
/*                      R E T C O D E S                      */
/* --------------------------------------------------------- */


#define EOF     0           // end of file
#define EOK     0           // success


#endif // MACROS_H
