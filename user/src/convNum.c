/*******************************************************************************
 * Number conversion functions with error checking.
 ******************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "convNum.h"

                /* static prototypes #{{{ */
/* aborts program if anything convFails. change exit to a return if needed */
static void
convFail(const char *fname, const char *msg, const char *arg, const char *name);

/* converts arg to a long, based on flags. fname and name used in error */
static long long
convNum(const char *fname, const char *arg, int32_t flags, const char *name);

/* converts arg to a long, based on flags. fname and name used in error */
static unsigned long long
convUnsigned(const char *fname, const char *arg, register int32_t flags, const char *name);
/*#}}}*/

/******************************************************************************
 *                          Header Funtions
 *****************************************************************************/
void convFail(const char *fname, const char *msg, const char *arg, const char *name)/*#{{{*/
{
    fprintf(stderr, "%s error", fname);

    /* print original callers name if it was given */
    if(name != NULL){
        fprintf(stderr, " (intType %s)", name);}

    fprintf(stderr, ": %s\n", msg);

    /* print the argument that was invalid */
    if(arg != NULL && *arg != '\0'){
        fprintf(stderr, "\t     offending text: %s\n", arg);}

    exit(EXIT_FAILURE);
} /* end fnFail #}}}  */

long long convNum(const char *fname, const char *arg, register int32_t flags, /*#{{{*/
                 const char *name)
{
    register long long res = 0;
    register int32_t base = 0;
    char *endptr = NULL;

    if(arg == NULL || *arg == '\0'){
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nNull or empty string.\n");
            return 0;}
        else{
            convFail(fname, "null or empty string", arg, name);}
    }

    /* determine the base to be passed to strtoul. */
    base = (flags & CN_BASE_10) ? 10 :
           (flags & CN_BASE_16) ? 16 :
           (flags & CN_BASE_8)  ? 8  : 0;

    errno = 0; /* just in case errno was set from who knows where */
    res = strtoll(arg, &endptr, base); /* convert string to base */

    /* error check result */
    if(errno != 0){
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nConversion convFailed.\n");
            return 0;}
        else{
            convFail(fname, "strtoll() convFailed", arg, name);} /* end else */
    }

    if(*endptr != '\0'){
        if(flags & CN_NOEXIT_){
            errno = EINVAL;
            fprintf(stderr,"\nNonnumeric characters.\n");
            return 0;
        }
        else{
            convFail(fname, "nonnumeric characters", arg, name);}
    }

    if((flags & CN_NONEG) && res < 0){
        if(flags & CN_NOEXIT_){
            errno = EINVAL;
            fprintf(stderr,"\nNegetive value not allowed.\n");
            return 0;
        }
        else{
            convFail(fname, "negative value not allowed", arg, name);}
    }

    if((flags & CN_GT_Z) && res <= 0){
        if(flags & CN_NOEXIT_){
            errno = EINVAL;
            fprintf(stderr,"\nValue must be > 0.\n");
            return 0;
        }
        else{
            convFail(fname, "value must be > 0", arg, name);}
    }
    return res;
} /* end convNum #}}} */

unsigned long long convUnsigned(const char *fname, const char *arg,     /*#{{{*/
                               register int32_t flags, const char *name)
{
    register unsigned long long res = 0;
    register int32_t base = 0;
    char *endptr = NULL;

    if(arg == NULL || *arg == '\0'){
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nNull or empty string.\n");
            return 0;}
        else{
            convFail(fname, "null or empty string", arg, name);}
    }

    /* determine the base to be passed to strtoul. */
    base = (flags & CN_BASE_10) ? 10 :
           (flags & CN_BASE_16) ? 16 :
           (flags & CN_BASE_8)  ? 8  : 0;

    errno = 0; /* just in case errno was set from who knows where */
    res = strtoull(arg, &endptr, base); /* convert string to base */

    /* error check result */
    if(errno != 0){
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nConversion convFailed.\n");
            return 0;}
        else{
            convFail(fname, "strtoll() convFailed", arg, name);}
    }

    if(*endptr != '\0'){
        if(flags & CN_NOEXIT_){
            errno = EINVAL;
            fprintf(stderr,"\nNonnumeric characters.\n");
            return 0;}
        else{
            convFail(fname, "nonnumeric characters", arg, name);}
    }

    if((flags & CN_GT_Z) && res <= 0){
        if(flags & CN_NOEXIT_){
            errno = EINVAL;
            fprintf(stderr,"\nValue must be > 0.\n");
            return 0;}
        else{
            convFail(fname, "value must be > 0", arg, name);}
    }
    return res;
} /* end convUnsigned #}}} */

/******************************************************************************
 *                          Header Funtions
 *****************************************************************************/
long convLong(const char *arg, int32_t flags, const char *varName)/*#{{{*/
{
    register long long res = 0;

    res = convNum("convLong", arg, flags, varName);

    /* make sure the converted number is in the range of an long */
    if(res > LONG_MAX || res < LONG_MIN){
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nInteger was out of range\n");
            return 0;
        }
        else convFail("convLong", "integer out of range", arg, varName);
    }
    return (long) res;
} /* end convLong #}}} */

int convInt(const char *arg, int32_t flags, const char *varName)/*#{{{*/
{
    register long long res = 0;

    res = convNum("convInt", arg, flags, varName);

    if (errno)
        return 0;

    /* make sure the converted number is in the range of an int */
    if(res > INT_MAX || res < INT_MIN){
        if(flags & CN_NOEXIT_){
            errno = EINVAL;
            fprintf(stderr,"\nInteger was out of range\n");
            return 0;
        } else {
            convFail("convInt", "integer out of range", arg, varName);
        }
    }
    return (int) res;
} /* end getInt #}}}*/

int32_t conv32_t(const char *arg, int32_t flags, const char *varName)/*#{{{*/
{
    register long long res = 0;

    res = convNum("conv32_t", arg, flags, varName);

    /* make sure the converted number is in the range of an int32_t */
    if(res > INT32_MAX || res < INT32_MIN){
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nInteger was out of range\n");
            return 0;
        }
        else convFail("conv32_t", "integer out of range", arg, varName);
    }
    return (int32_t) res;
} /* end get32_t #}}} */

int64_t conv64_t(const char *arg, int32_t flags, const char *varName)/*#{{{*/
{
    register long long res = 0;

    res = convNum("conv64_t", arg, flags, varName);

    /* make sure the converted number is in the range of an int64_t */
    if(res > INT64_MAX || res < INT64_MIN){
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nInteger was out of range\n");
            return 0;
        }
        else convFail("conv64_t", "integer out of range", arg, varName);
   }
    return (int64_t) res;
} /* end get64_t #}}} */

uint32_t convU32_t(const char *arg, int32_t flags, const char *varName)/*#{{{*/
{
    register unsigned long long res = 0;

    res = convUnsigned("convU32_t", arg, flags, varName);

    /* make sure the converted number is in the range of an uint32_t */
    if(res > UINT32_MAX){ /* checks for >0 in convNum */
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nInteger was out of range\n");
            return 0;
        }
        else convFail("convU32_t", "integer out of range", arg, varName);
    }
    return (uint32_t) res;
} /* end get32_t #}}} */

uint64_t convU64_t(const char *arg, int32_t flags, const char *varName)/*#{{{*/
{
    register unsigned long long res = 0;

    res = convUnsigned("convU64_t", arg, flags, varName);

    /* make sure the converted number is in the range of an uint64_t */
    if(res > UINT64_MAX){ /* checks for unsigned in getNum */
        if(flags & CN_NOEXIT_){
            fprintf(stderr,"\nInteger was out of range\n");
            return 0;
        }
        else convFail("convU64_t", "integer out of range", arg, varName);
    }
    return (uint64_t) res;
} /* end get64_t #}}} */
