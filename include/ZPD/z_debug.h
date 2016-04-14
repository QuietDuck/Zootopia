#pragma once

#ifndef __Z_DEBUG_H__
#define __Z_DEBUG_H__

#include "z_config.h"
#include "z_compose.h"
#include "z_type.h"

namespace zootopia {

#define __xfilename__		__FILE__
#define __xlinenumber__		__LINE__
#define __xfuncname__		__FUNCTION__

#ifdef _DEBUG
#define ZASSERT(cond) 		\
	do{\
		const bool ____res = (((uptr)(cond)) != 0); \
		if(! ____res){\
			zootopia::dbg::__log_to_error_channel(__xfilename__, __xlinenumber__ , __xfuncname__, "aborted expression: %s", # cond);\
			zootopia::dbg::__assert(____res); \
		}\
	}while(0)

#define ZLOG_P(...)		zootopia::dbg::__log_to_inform_channel_pure( __VA_ARGS__)
#define ZLOG_I(...)		zootopia::dbg::__log_to_inform_channel(__xfilename__, __xlinenumber__, __xfuncname__, __VA_ARGS__)
#define ZLOG_W(...)		zootopia::dbg::__log_to_warning_channel(__xfilename__, __xlinenumber__, __xfuncname__, __VA_ARGS__)
#define ZLOG_E(...)		zootopia::dbg::__log_to_error_channel(__xfilename__, __xlinenumber__, __xfuncname__, __VA_ARGS__)
#define ZLOG_T(msg)		zootopia::dbg::__timelog __xdbg_time_logger (__xfilename__, __xlinenumber__, __xfuncname__, msg)
#define ZABORT(msg)		ZASSERT((msg) == null)
#else
#define XASSERT(...)
#define XLOG_P(...)
#define XLOG_I(...)
#define XLOG_W(...)
#define XLOG_E(...)
#define XLOG_T(...)
#endif

#define ZABORT_NOT_IMPLEMENTED() \
	ZASSERT("Not implemented yet." == null)

#if PRINT_CTOR_AND_DTOR
    // CTOR, DTOR LOG
#else

#endif

    namespace dbg {

        /**
        Do not directly use functions or class below, use above macro instead.
        **/

        void
            __assert(bool cond);

        void
            __log_to_inform_channel_pure(const char *format, ...);

        void
            __log_to_inform_channel(const char *file, const int line, const char * func, const char *format, ...);

        void
            __log_to_warning_channel(const char *file, const int line, const char * func, const char *format, ...);

        void
            __log_to_error_channel(const char *file, const int line, const char * func, const char *format, ...);


        uint32
            getTimeMS(void);


        class __timelog
        {
        private:
            __timelog() {}
        public:
            __timelog(const char *file_, const int line_, const char * func_, char *msg_)
                :file(file_), line(line_), msg(msg_), func(func_)
            {
                time = getTimeMS();
            };

            ~__timelog()
            {
                const int dt = getTimeMS() - time;
                __log_to_inform_channel(file, line, func, ">> %d(ms) %s\n", dt, msg);
            }

        private:
            const char * file;
            int line;
            const char * func;
            const char * msg;
            uint32 time;
        };

    }

#ifdef _DEBUG
#define ZASSERT_ONCOMPILE(condition, errmsg)	do { struct ____compiletime_err_checker { uint32 errmsg:!!(condition); };} while(0)
#else
#define ZASSERT_ONCOMPILE(condition,errmsg)
#endif

}

#endif

