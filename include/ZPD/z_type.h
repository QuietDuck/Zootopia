#pragma once

#ifndef __Z_TYPE_H__
#define __Z_TYPE_H__

#include <float.h>
#include <limits.h>

#include "z_compose.h"

namespace zootopia {

    typedef signed char		 		int8;

    typedef unsigned char		 	uint8;

    typedef signed short		 	int16;

    typedef unsigned short			uint16;

    typedef signed int		 		int32;

    typedef unsigned int		 	uint32;

    typedef float					xtime;

#ifdef _WIN32
#  		ifdef _M_X64 // 64bit processor
    typedef unsigned long long	 	uptr;		///big enough size to keep pointer
#		else
    typedef unsigned long		 	uptr;		///big enough size to keep pointer	
#		endif					
#else
    typedef unsigned long		 		uptr;		///big enough size to keep pointer	
#endif

    typedef enum { XFAIL = 0, XSUCCESS = 1 } XRESULT;

    // limits

#define INT8_MAX_VALUE				CHAR_MAX
#define INT8_MIN_VALUE				CHAR_MIN

#define INT32_MAX_VALUE				INT_MAX
#define INT32_MIN_VALUE				INT_MIN

#define INT16_MAX_VALUE				SHRT_MAX
#define INT16_MIN_VALUE				SHRT_MIN

#define UINT8_MAX_VALUE				UCHAR_MAX
#define UINT16_MAX_VALUE			USHRT_MAX
#define UINT32_MAX_VALUE			UINT_MAX

#define SCALAR_MAX_VALUE			FLT_MAX
#define SCALAR_MIN_VALUE			FLT_MIN

}

#endif