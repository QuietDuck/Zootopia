#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <cassert>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "zpd.h"

using namespace zootopia;

typedef enum {
    __pure__,
    __info__,
    __warn__,
    __error__
}__channel;

static void
__show_log(const __channel channel, const char *file, const int line, const char * func, const char *format, va_list args)
{

    // extract only file name from full path
    char filename[255] = { "\0", };
    const char *p = file;
    int i = 0;

    if (p) {

        while (*p != '\0') {

            p++;
            i++;
        };

        assert(i > 0);
        i = 0;

        while ((*p != '/') && (*p != '\\')) {
            p--;
            i++;
        }

        assert(i > 0);

        i = 0;

        while (*(++p) != '\0') {
            filename[i++] = *p;
        }
    }

    switch (channel) {

    case __info__:
        printf("[INFO] %s, line:%d, %s()\n", filename, line, func);
        break;

    case __warn__:
        printf("[WARN] %s, line:%d, %s()\n", filename, line, func);
        break;

    case __error__:
        printf("[ERROR] %s, line:%d, %s()\n", filename, line, func);
        break;
    }

    vprintf(format, args);

    printf("\n");
}


void
dbg::__log_to_inform_channel_pure(const char *format, ...)
{

    va_list args;

    va_start(args, format);

    __show_log(__pure__, null, 0, null, format, args);

    va_end(args);

}



void
dbg::__log_to_inform_channel(const char *file, const int line, const char * func, const char *format, ...)
{


    va_list args;

    va_start(args, format);

    __show_log(__info__, file, line, func, format, args);

    va_end(args);

}

void
dbg::__log_to_warning_channel(const char *file, const int line, const char * func, const char *format, ...)
{
    va_list args;

    va_start(args, format);

    __show_log(__warn__, file, line, func, format, args);

    va_end(args);
}


void
dbg::__log_to_error_channel(const char *file, const int line, const char * func, const char *format, ...)
{

    va_list args;

    va_start(args, format);

    __show_log(__error__, file, line, func, format, args);

    va_end(args);

}


void
dbg::__assert(bool cond)
{
    assert(cond);
}


uint32
dbg::getTimeMS(void)
{
    return ((uint32)(((long long)clock()) * 1000 / CLOCKS_PER_SEC));
}



/*
+---------------------------------------------------------------------------------------
|
|	EOF
|
+---------------------------------------------------------------------------------------
*/

