/******************************************************************************
 * filename: utility_sys.h
 *
 * Useful macros and definitions.
 *
 * Written by: James Ross
 *****************************************************************************/

#ifndef _UTL_SYS_H_
#define _UTL_SYS_H_

//#define _POSIX_C_SOURCE 199309

#define FAILURE -1
#define SUCCESS 0

#define RW_END 0

#define P_RD 0 // value for a pipe read fd in pipefd[2]
#define P_WR 1 // value for a pip write fd in pipefd[2]


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>
#include <errno.h>
#include <pthread.h>

#ifndef __USE_MISC
#define __USE_MISC 1 // provide usleep 
#endif
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define NDEBUG 1
#include <assert.h>

#include "convNum.h"       /* str to int functions */
#include "err_handle.h" /* general error printing */

/* GNU compiler native to hint for branch prediction in user space */
#define _usrLikely(x)      __builtin_expect(!!(x), 1)
#define _usrUnlikely(x)    __builtin_expect(!!(x), 0)

/* malloc family allocation macros */
#define CALLOC(type)             ((type*) calloc(1, sizeof(type)))
#define CALLOC_VOID(nmemb, size) ((void*) calloc(nmemb, size))
#define CALLOC_ARRAY(type, num)  ((type*) calloc(num, sizeof(type)))


#define my_strdup(dest, src, len)                                              \
{                                                                              \
    dest = CALLOC_ARRAY(char, len);                                            \
    if (dest) memcpy(dest, src, len);                                          \
} /* end my_strdup */

/*******************************************************************************  
 * buff is set to '\0' for nByte before read occures. 
 *  If nothing is read, buff[0] = '\0', retBytes == 0.
 *  - fd    == int   , File descriptor used for allocInputBuff()
 *  - buff  == char* , Buffer to be filled with character data from fd.
 *  - nbyte == size_t, number of bytes to read 
 *  - retBytes == ssize_t, number of bytes read from file
 *                   (typically the size of buffer array) 
 ******************************************************************************/
#define READ_INPUT(fd, buff, nByte, retBytes)                                  \
{                                                                              \
    assert(buff != NULL);                                                      \
    memset(buff, '\0', nByte);                                                 \
    if(_usrUnlikely((retBytes = read(fd, (void*) buff, nByte)) == FAILURE))    \
        errExit("READ_INPUT, read() failure");                                 \
} /* end READ_INPUT */

/****************************************************************************** 
 * Copy a variable ammount of characters from a buffer based on a given 
 * position. Places a null value at end of inBuff
 * 
 * Place resulting string in resStr based on a given conditional. 
 * resStr is cleared by resLen to '\0' values before being written to.
 *
 * To make sure conditional was met and copied check the value before the first
 * '\0' to ensure it is one of your delimiters, otherwise 
 * -end of buffer- was reached.
 *
 * If data in inbuff is a string, the end of the string is found when the last
 * two index of resStr is '\0'.
 *
 * EOF is reached when the resulting string has more than 1 terminating '\0'
 * value.
 *
 * - fd     == int  , File descriptor corresponding to inBuf.
 * - inBuf  == char*, copy from buff. Must not be NULL, leave room for '\0'
 * - bfPl   == int, the current location inside inBuf.
 * - nbyte  == size_t, number of bytes to read
 * - resStr == char*, buffer to copy to.
 * - resLen == size_t, length of resStr for conidional overflow stop point 
 * - conditional == The conditionals desired in the copy process.
 *                Example: inBuf[i] != ' ' && inBuf[i] != '\n' 
 ******************************************************************************/
#define PARSE_BUFF(fd, inBuf, bfPl, resStr, resLen, conditional)\
{                                                                              \
    int _TM_ = 0;                                                              \
    assert(resStr != NULL && inBuf != NULL);                                   \
                                                                               \
    memset(resStr, '\0', resLen);                                              \
    for(_TM_ = 0; conditional && _TM_ < resLen-1; ++_TM_)                      \
    {                                                                          \
        resStr[_TM_] = inBuf[bfPl];                                            \
        ++bfPl;                  /* increase buff placement */                 \
    } /* end for */                                                            \
    resStr[_TM_] = '\0';                                                       \
} /* end PARSE_BUFF */

/* Clears STDIN using read() */
#define RD_CLR_STDIN() {                                                        \
    char __ch = {'\0'};                                                        \
    while(read(STDIN_FILENO, (void*)&__ch, 1) && __ch != '\n' && __ch != EOF); \
} /* end RD_CLR_STDIN */

/*******************************************************************************
 * TODO: Adjust this macro or make an alternate that can call a function with
 *       variable arguments, rather than just one argument. (i.e. free(pntr);)
 *
 * TODO: Find a system or compiler macro, or something, to see if C99 is
 *       supported. If it is not, do not define the macro at all. Programmer
 *       should be aware of it, and for now do not use the macro unless c99 is
 *       supported.
 *
 * Vectorizes the function funct.
 *
 * Will execute every argument into the function.
 * funct can only take 1 argument.
 *
 * -Type is the type of pointer used. (VA_ARGS could be void for example.). 
 * -... is a variable argument list.
 *
 * NOTE: Modified to only be used for free
 ******************************************************************************/
#define APPLY_FUNCT(type, funct, ...)                                          \
{                                                                             \
    void *stopper = (int[]){0};                                                \
    type **apply_list = (type*[]){__VA_ARGS__, stopper};                       \
    int __i_;                                                                  \
                                                                               \
    for (__i_ = 0; apply_list[__i_] != stopper; ++__i_) {                      \
        if (apply_list[__i_])                                                  \
            (funct)(apply_list[__i_]);                                          \
    }                                                                          \
} /* end apply_funct */
    
/* Apply free to every pointer given in the argument list using apply_funct() */
#define FREE_ALL(...) APPLY_FUNCT(void, free, __VA_ARGS__)

/******************************************************************************* 
 * Subtract two timespec structures and place them in a resulting timespec
 * struct.
 *
 * All passed values are pointers of struct timespec.
 ******************************************************************************/
#define _NANO_1SEC 1000000000
#define TIMESPEC_SUB(toSubPtr, subByPtr, resRetPtr)                            \
{                                                                              \
    assert((toSubPtr) != NULL && (subByPtr) != NULL && (resRetPtr) != NULL);   \
                                                                               \
    (resRetPtr) -> tv_sec  = ((toSubPtr) -> tv_sec) - ((subByPtr) -> tv_sec);  \
    (resRetPtr) -> tv_nsec = ((toSubPtr) -> tv_nsec) - ((subByPtr) -> tv_nsec);\
                                                                               \
    /* If nano seconds is negetive, need to adjust for carry by adding 1       \
     * second to nano until nano is no longer negetive or seconds is zero. */  \
    while(0 > ((resRetPtr) -> tv_nsec) && 0 < ((resRetPtr) -> tv_sec)){        \
        --((resRetPtr) -> tv_sec);                                             \
        (resRetPtr) -> tv_nsec = ((resRetPtr) -> tv_nsec) + _NANO_1SEC;        \
    }                                                                          \
} /* end TIMESPEC_SUB */

#endif /************ EOF **************/
