/*******************************************************************************
 *
 *  Functions declared in this header file will handle various error cases
 *  including closing the process, printing error messages, and interpreting
 *  errno. It also produces a core dump file when appropriate.
 *
 *   filename: err_handle.h
 *
 ******************************************************************************/

#ifndef _ERR_HANDLE_H
#define _ERR_HANDLE_H

#include <sys/types.h>
#include <inttypes.h>
#include <errno.h>

/* this macro stops -Wall from producing a warning when exiting from a
   function that is non-void. */
#ifdef __GNUC__
    #define NORETURN __attribute__((__noreturn__))
#else
    #define NORETURN /* defines for compilation */
#endif

/* functions below pass arguments like printf(3), except errnumExit(), whose
   first argument is the error number (obtained without errno).

   functions flush stdout unless otherwise specified. */

/* does not terminate, errno for errnum */
void err_msg(const char *fstring, ...);

/* does not terminate, does not print errno information */
void noerr_msg(const char *fstring, ...);

/* does not terminate, uses errnum instead of errno */
void errnum_msg(int32_t errnum, const char *fstring, ...);

/* uses exit() to terminate, errno for errnum */
void errExit(const char *fstring, ...) NORETURN;

/* uses _exit() to terminate, errno for errnum, does not flush stdout */
void err_exit(const char *fstring, ...) NORETURN;

/* does not use errno, instead uses errnum which should correspond to a errno
   number. Usefull in threaded applications aswell. uses exit() */
void errnumExit(int32_t errnum, const char *fstring, ...) NORETURN;

/* there was an error without a corresponding error number.
   uses exit() */
void noerrExit(const char *fstring, ...) NORETURN;

/* there was an error without a corresponding error number.
   uses _exit(), does not flush stdout */
void noerr_exit(const char *fstring, ...) NORETURN;
#endif
