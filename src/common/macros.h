#ifndef MACROS_H
#define MACROS_H


/* --------------------------------------------------------- */
/*                         D E B U G                         */
/* --------------------------------------------------------- */


// final value is computed in run-time
#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


// init and exit module trace messages
#define GREETING()      printk("Hello from " MODULE_NAME "!\n");
#define FAREWELL()      printk("Goodbuy from " MODULE_NAME "!\n");


/* --------------------------------------------------------- */
/*                      R E T C O D E S                      */
/* --------------------------------------------------------- */


#define EOF     0           // end of file
#define EOK     0           // success


#endif // MACROS_H
