#ifndef COMMON_H
#define COMMON_H


/* --------------------------------------------------------- */
/*                        M A C R O S                        */
/* --------------------------------------------------------- */


// final value is computed in run-time
#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


#endif // COMMON_H
