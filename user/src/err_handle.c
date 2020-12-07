/*******************************************************************************
 * Level 1 cache similator project
 * ECE 4/585
 * Mark Faust
 * 12/3/2017
 * Portland State University, Portland OR
 * written and property of : James Ross
 ******************************************************************************/


/*
    implements all functions declared in err_handle.h

    filename: err_handle.c
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

#include "err_handle.h"
#include "ename_array.inc" /* holds error code names for printing */

/* this macro stops -Wall from producing a warning when exiting from a
   function that is non-void. */
#ifdef __GNUC__
__attribute__((__noreturn__))
#endif

#ifndef _EH_FLAGS_
#define _EH_FLAGS_
    #define EXIT_EH        0x1 /* use exit() on terminate */
    #define _EXIT_EH       0x2 /* use _exit() on terminate */
    #define USE_ERRNUM_EH  0x4 /* place errnum information in stderr output */
    #define STDFLUSH_EH    0x8 /* flush stdio before printing msg to stderr */
#endif


                /* static functions */

                /* static prototypes #{{{ */
/* terminate the program, if appropriate produce a core dump file with abort()
   else  mainExit = false (_exit), mainExit = true (exit()) */
static void terminate(register int32_t exitType);

/* Outputs error messages. Flags include, USE_ERRNUM_EH | STDFLUSH_EH */
static void outputErr(int32_t flags, int32_t errnum, const char* format, va_list ap);
/*#}}}*/

void terminate(register int32_t exitType)/*#{{{*/
{
    register char *cdEnv = NULL;

    /* check for EF_DUMPCORE env variable. If it is set, produce a core dump
       file and exit by calling the abort() system call, otherwise exit based
       on exitType */
    cdEnv = getenv("EF_DUMPCORE");

    (cdEnv != NULL && *cdEnv != '\0') ? abort()
                                      : (exitType & EXIT_EH) ? exit(EXIT_FAILURE)
                                      : _exit(EXIT_FAILURE);
} /* end terminate #}}} */

void outputErr(int32_t flags, int32_t errnum, const char* fstring,/*#{{{*/
               va_list argList)
{
#define _BUFF_SIZE_ 500

    char finalMsg[_BUFF_SIZE_] = {'\0'};   /* final msg to send to stderr */
    char callMsg[_BUFF_SIZE_]  = {'\0'};   /* msg from original call */
    char errStr[_BUFF_SIZE_]   = {'\0'};   /* string from error information */

    /* place the fstring and its relevent argList into callMsg. */
    vsnprintf(callMsg, _BUFF_SIZE_, fstring, argList);

    /* if flag USE_ERRNUM_EH was used, fill errstr with the appropriate error
       information. if flag USE_ERRNUM_EH was used check range on errnum,
       if its out of range of errno constants insert UNKNOWNERR. */
    if(flags & USE_ERRNUM_EH)
    {
        snprintf(errStr, _BUFF_SIZE_, "[%s %s]",
             ((errnum > 0 && errnum <= MAX_ENAME) ? ename[errnum] : "?UNKWN?"),
             strerror(errnum));
    } /* end if */
    else
    {
        snprintf(errStr, _BUFF_SIZE_, "[NONUM]");
    } /* end else */

    /* combine the final callMsg and final errStr to create the finalMsg to
       display in stderr. */
    snprintf(finalMsg, _BUFF_SIZE_, "ERROR: %s %s\n", errStr, callMsg);

    if(flags & STDFLUSH_EH){
        fflush(stdout);}

    /* print finalMsg to stderr */
    fputs(finalMsg, stderr);
    fflush(stderr); /* in case stderr is not currently line buffered */
#undef _BUFF_SIZE_
} /* end outputErr #}}} */


                /* header functions */

void err_msg(const char *fstring, ...)/*#{{{*/
{
    va_list vargList;
    register int32_t sverr = errno;

    va_start(vargList, fstring);
    outputErr((USE_ERRNUM_EH | STDFLUSH_EH), sverr, fstring, vargList);
    va_end(vargList);

    /* since errMsg does not terminate, it resets errno at the end in case
       errno was triggered inside any functions used with outputErr() */
    errno = sverr;
} /* end errMsg #}}} */

void noerr_msg(const char *fstring, ...)/*#{{{*/
{
    va_list vargList;

    va_start(vargList, fstring);
    outputErr(STDFLUSH_EH, 0, fstring, vargList);
    va_end(vargList);
} /* end noerr_msg #}}} */

void errnum_msg(int32_t errnum, const char *fstring, ...){/*#{{{*/
    va_list vargList;

    va_start(vargList, fstring);
    outputErr((USE_ERRNUM_EH | STDFLUSH_EH), errnum, fstring, vargList);
    va_end(vargList);
}// end errnum_msg #}}}

void errExit(const char *fstring, ...)/*#{{{*/
{
    va_list vargList;

    va_start(vargList, fstring);
    outputErr((USE_ERRNUM_EH | STDFLUSH_EH), errno, fstring, vargList);
    va_end(vargList);

    terminate(EXIT_EH);
} /* end errExit #}}} */

void err_exit(const char *fstring, ...)/*#{{{*/
{
    va_list vargList;

    va_start(vargList, fstring);
    outputErr(USE_ERRNUM_EH, errno, fstring, vargList);
    va_end(vargList);

    terminate(_EXIT_EH);
} /* end err_exit #}}} */

void errnumExit(register int32_t errnum, const char *fstring, ...)/*#{{{*/
{
    va_list vargList;

    va_start(vargList, fstring);
    outputErr((USE_ERRNUM_EH | STDFLUSH_EH), errnum, fstring, vargList);
    va_end(vargList);

    terminate(EXIT_EH);
} /* end err_exit #}}} */


void noerrExit(const char *fstring, ...)/*#{{{*/
{
    va_list vargList;

    va_start(vargList, fstring);
    outputErr(STDFLUSH_EH, 0, fstring, vargList);
    va_end(vargList);

    terminate(EXIT_EH);
} /* end noerrExit #}}} */

void noerr_exit(const char *fstring, ...)/*#{{{*/
{
    va_list vargList;

    va_start(vargList, fstring);
    outputErr(0, 0, fstring, vargList);
    va_end(vargList);

    terminate(EXIT_EH);
} /* end noerr_exit #}}} */

