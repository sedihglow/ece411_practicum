/*******************************************************************************
 * Level 1 cache similator project
 * ECE 4/585
 * Mark Faust
 * 12/3/2017
 * Portland State University, Portland OR
 * written and property of : James Ross
 *
 *   filename: convNum.h
    Turns a string of characters into a their numerical equivilance using
    strtoul family and error checking the results.
 ******************************************************************************/



#ifndef _CON_NUM_H
#define _CONV_NUM_H
                    /* definitions */
#define CN_NONEG    0x1      /* Value must be >= 0 */
#define CN_GT_Z     0x2      /* Value must be > 0  */

                            /* By default base is passed in as 0 (strtol(3)) */
#define CN_BASE_10  0x4     /* Value is expressed in decimal */
#define CN_BASE_8   0x8     /* Value is expressed in octal */
#define CN_BASE_16  0x10    /* Value is expressed in hexadecimal */

                            /* By default, process uses exit() on error */
#define CN_NOEXIT_  0x20    /* Return 0, set errno on conversion error */

#include <sys/types.h>
#include <inttypes.h>

/* ERROR HANDLE NOTE: If using the GN_NOEXIT_ flag, error is checked when
   function returns a 0 AND arg != '0' */

                    /* public prototypes */

/* turn arg into a long based on flags. name used in error message */
long convLong(const char *arg, int flags, const char *varName);

/* turn arg into a int based on flags. name used in error message */
int convInt(const char *arg, int flags, const char *varName);

/* turn arg into a int32_t based on flags. name used in error message */
int32_t conv32_t(const char *arg, int32_t flags, const char *varName);

/* turn arg into a int64_t based on flags. name used in error message */
int64_t conv64_t(const char *arg, int32_t flags, const char *varName);

/* turn arg into a uint32_t based on flags. name used in error message */
uint32_t convU32_t(const char *arg, int32_t flags, const char *varName);

/* turn arg into a uint64_t based on flags. name used in error message */
uint64_t convU64_t(const char *arg, int32_t flags, const char *varName);
#endif
