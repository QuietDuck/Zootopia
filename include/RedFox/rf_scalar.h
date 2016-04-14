#pragma once

#ifndef __RF_SCALAR_H__
#define __RF_SCALAR_H__

#include <math.h>
#include <float.h>

#define RF_SCALAR_IS_DOUBLE 0
#undef RF_SCALAR_IS_FLOAT
#define RF_SCALAR_IS_FLOAT  1

#if RF_SCALAR_IS_FLOAT

typedef float RfScalar;

#define RF_Scalar1                  1.0f
#define RF_ScalarHalf               0.5f
#define RF_ScalarSqrt2              1.41421356f
#define RF_ScalarPI                 3.14159265f
#define RF_ScalarTanPIOver8         0.414213562f
#define RF_ScalarRoot2Over2         0.707106781f
#define RF_ScalarMax                3.402823466e+38f

#define RfScalarFloorToScalar(x)    floorf(x)
#define RfScalarCeilToScalar(x)     ceilf(x)
#define RfScalarRoundToScalar(x)    floorf((x) + 0.5f)

#define RfScalarFloorToInt(x)       (int)floorf(x)
#define RfScalarCeilToInt(x)        (int)ceilf(x)
#define RfScalarRoundToInt(x)       (int)floorf((x) + 0.5f)

#define RfScalarAbs(x)              fabsf(x)
#define RfScalarCopySign(x, y)      copysignf(x, y)
#define RfScalarMod(x, y)           fmodf(x,y)
#define RfScalarFraction(x)         fmodf(x, 1.0f)
#define RfScalarSqrt(x)             sqrtf(x)
#define RfScalarPow(b, e)           powf(x)

#define RfScalarSin(radians)        sinf(radians)
#define RfScalarCos(radians)        cosf(radians)
#define RfScalarTan(radians)        tanf(radians)
#define RfScalarASin(val)           asinf(val)
#define RfScalarACos(val)           acosf(val)
#define RfScalarATan2(y, x)         atan2f(y,x)
#define RfScalarExp(x)              expf(x)
#define RfScalarLog(x)              logf(x)
#define RfScalarLog2(x)             log2f(x)

#else // RF_SCALAR_IS_DOUBLE

typedef double RfScalar;

#define RF_Scalar1                  1.0
#define RF_ScalarHalf               0.5
#define RF_ScalarSqrt2              1.414213562373095
#define RF_ScalarPI                 3.141592653589793
#define RF_ScalarTanPIOver8         0.4142135623731
#define RF_ScalarRoot2Over2         0.70710678118655
#define RF_ScalarMax                1.7976931348623157+308
#define RF_ScalarInfinity           RF_DoubleInfinity
#define RF_ScalarNegativeInfinity   RF_DoubleNegativeInfinity
#define RF_ScalarNaN                RF_DoubleNaN

#define RfFixedToScalar(x)          RfFixedToDouble(x)
#define RfScalarToFixed(x)          RfDoubleToFixed(x)

#define RfScalarFloorToScalar(x)    floor(x)
#define RfScalarCeilToScalar(x)     ceil(x)
#define RfScalarRoundToScalar(x)    floor((x) + 0.5)

#define RfScalarFloorToInt(x)       (int)floor(x)
#define RfScalarCeilToInt(x)        (int)ceil(x)
#define RfScalarRoundToInt(x)       (int)floor((x) + 0.5)

#define RfScalarAbs(x)              abs(x)
#define RfScalarCopySign(x, y)      copysign(x, y)
#define RfScalarMod(x, y)           fmod(x,y)
#define RfScalarFraction(x)         fmod(x, 1.0)
#define RfScalarSqrt(x)             sqrt(x)
#define RfScalarPow(b, e)           pow(b, e)

#define RfScalarSin(radians)        sin(radians)
#define RfScalarCos(radians)        cos(radians)
#define RfScalarTan(radians)        tan(radians)
#define RfScalarASin(val)           asin(val)
#define RfScalarACos(val)           acos(val)
#define RfScalarATan2(y, x)         atan2(y,x)
#define RfScalarExp(x)              exp(x)
#define RfScalarLog(x)              log(x)
#define RfScalarLog2(x)             log2(x)

#endif

// TYPECAST
#define RfIntToScalar(x)        static_cast<RfScalar>(x)
#define RfScalarTruncToInt(x)   static_cast<int>(x)

#define RfScalarToFloat(x)      static_cast<float>(x)
#define RfFloatToScalar(x)      static_cast<RfScalar>(x)
#define RfScalarToDouble(x)     static_cast<double>(x)
#define RfDoubleToScalar(x)     static_cast<RfScalar>(x)

#define RF_ScalarMin            (-RF_ScalarMax)


#endif
