/**
 *      @file  terr.h
 *     @brief
 *
 * Detailed description starts here.
 *
 *    @author  Paweł Szynkiewicz (PSz), pszynk@gmail.com
 *
 *  @internal
 *    Created  01.11.2013 15:11:21
 *   Revision  $REVISION$
 *   Compiler  gcc
 *    Company  Politechnika Warszawska
 *  Copyright  Copyright (c) 2013, Paweł Szynkiewicz
 *
 *=====================================================================================
 */
#ifndef MYERR_H
#define MYERR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/*
 *#define ERR(...) do{globErr=errno; _WARN(__VA_ARGS__); exit(-1);}while(0)
 *#define WARN(...) do{globErr=errno; _WARN(__VA_ARGS__);}while(0)
 *#define WARNX(...) do{globErr=0; _WARN(__VA_ARGS__);}while(0)
 */
// functions for color output in tty & no-color in pipes
/*
 *extern int (*red)(const char *fmt, ...);
 *extern int (*_WARN)(const char *fmt, ...);
 *extern int (*green)(const char *fmt, ...);
 */



#ifndef MYERR_COLOR_ON
#define MYERR_COLOR_ON 1
#endif

#define __MYERR_STD 2

#define __MYERR_ERROR_PREFIX "ERROR: "
#define __MYERR_WARNING_PREFIX "WARNING: "

#define __MYERR_COL_RED  "\x1b[31m"
#define __MYERR_COL_GREEN  "\x1b[32m"
#define __MYERR_COL_BLUE "\x1b[34m"
#define __MYERR_COL_DEFAULT  "\x1b[0m"


#if MYERR_COLOR_ON > 0
#define __MYERR_SET_COLOR(color) do{ myerr_set_color(color); } while(0)
#else
#define __MYERR_SET_COLOR(color) do{ if(0) {myerr_set_color(color); }} while(0)
#endif

#define MYERR_ERR(eval, args...) do { \
    __MYERR_MSG(__MYERR_ERROR_PREFIX, __MYERR_COL_RED, 1, args); \
    exit(eval); \
}while(0)

#define MYERR_WARN(args...) do { \
    __MYERR_MSG(__MYERR_WARNING_PREFIX, __MYERR_COL_GREEN, 1, args); \
}while(0)

#define MYERR_WARNX(args...) do { \
    __MYERR_MSG(__MYERR_WARNING_PREFIX, __MYERR_COL_GREEN, 0, args); \
}while(0)



#define __MYERR_MSG(prefix, color, iferrno, fmt, args...) \
    do{ \
        __MYERR_SET_COLOR(color); \
        myerr_msg(iferrno, \
                prefix \
                "%s (%s, line %d)\n\t%s : " \
                fmt, \
                __func__, __FILE__, __LINE__, __TIME__, \
##args); \
        __MYERR_SET_COLOR(__MYERR_COL_DEFAULT); \
    } while(0)

static inline void myerr_set_color(const char *color)
{
    if (isatty(__MYERR_STD)) {
        fprintf(stderr, "%s", color);
    }
};

static inline void myerr_msg(int iferrno, const char *fmt, ...){
    va_list ar;
    va_start(ar, fmt);
    vfprintf(stderr, fmt, ar);
    va_end(ar);
    fprintf(stderr, "\n");
    if(iferrno) {
        if(errno) {
            fprintf(stderr, "\tERRNO [%d]: %s\n", errno, strerror(errno));
        }
    }
};


#endif /* end of include guard: MYERR_H */
