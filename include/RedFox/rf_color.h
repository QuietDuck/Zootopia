#pragma once

/*
RfColor - Type
*/

#ifndef __RF_COLOR_H__
#define __RF_COLOR_H__

#include "zpd.h"

namespace zootopia {

    typedef enum {

        RfColorFormat_Invalid = 0,
        RfColorFormat_RGB_565,
        RfColorFormat_RGB_888,
        RfColorFormat_RGBA_8888,
        RfColorFormat_RGBA_8888_PRE,
        RfColorFormat_ARGB_8888,
        RfColorFormat_ARGB_8888_PRE,
        RfColorFormat_A8,

    } RfColorFormat;

    typedef struct RfColor {

        RfColorFormat format;
        uint32 packedColor;

        inline RfColor() :
            format(RfColorFormat_Invalid),
            packedColor(0)
        {}

        inline RfColor(RfColorFormat format_, uint32 packedColor_) :
            format(format_),
            packedColor(packedColor_)
        {}

        inline const bool operator == (const RfColor & rhs) {

            return (format EQ rhs.format AND packedColor EQ rhs.packedColor);
        }

        inline const bool operator != (const RfColor & rhs) {

            return (format NEQ rhs.format OR packedColor NEQ rhs.packedColor);
        }


        inline RfColor & operator = (const RfColor & rhs) {

            format = rhs.format;
            packedColor = rhs.packedColor;

            return *this;
        }

        inline uint32 r() const {

            switch (format) {

            case RfColorFormat_RGBA_8888:
            case RfColorFormat_RGBA_8888_PRE:
                return ((packedColor & 0xff000000) >> 24);
            case RfColorFormat_ARGB_8888:
            case RfColorFormat_ARGB_8888_PRE:
                return ((packedColor & 0x00ff0000) >> 16);
            case RfColorFormat_RGB_565:
                return ((packedColor & 0x0000f800) >> 11);
            case RfColorFormat_A8:
                ZABORT("A8 does not have red");
                return -1;
            default:
                ZABORT("invalid color");
                return -1;
            }
        }


        inline uint32 g() const {

            switch (format) {

            case RfColorFormat_RGBA_8888:
            case RfColorFormat_RGBA_8888_PRE:
                return ((packedColor & 0x00ff0000) >> 16);
            case RfColorFormat_ARGB_8888:
            case RfColorFormat_ARGB_8888_PRE:
                return ((packedColor & 0x0000ff00) >> 8);
            case RfColorFormat_RGB_565:
                return ((packedColor & 0x000007E0) >> 5);
            case RfColorFormat_A8:
                ZABORT("A8 does not have red");
                return 0;
            default:
                ZABORT("invalid color");
                return 0;
            }
        }


        inline uint32 b() const {

            switch (format) {

            case RfColorFormat_RGBA_8888:
            case RfColorFormat_RGBA_8888_PRE:
                return ((packedColor & 0x0000ff00) >> 8);
            case RfColorFormat_ARGB_8888:
            case RfColorFormat_ARGB_8888_PRE:
                return ((packedColor & 0x000000ff));
            case RfColorFormat_RGB_565:
                return (packedColor & 0x0000001f);
            case RfColorFormat_A8:
                ZABORT("A8 does not have red");
                return 0;
            default:
                ZABORT("invalid color");
                return 0;
            }
        }

        inline uint32 a() const {

            switch (format) {

            case RfColorFormat_RGBA_8888:
            case RfColorFormat_RGBA_8888_PRE:
            case RfColorFormat_A8:
                return (packedColor & 0x000000ff);
            case RfColorFormat_ARGB_8888:
            case RfColorFormat_ARGB_8888_PRE:
                return ((packedColor & 0xff000000) >> 24);
            case RfColorFormat_RGB_565:
                ZABORT("RGB565 does not have alpha value");
                return 0;
            default:
                ZABORT("invalid color");
                return 0;
            }
        }

        static inline uint32 make_packed_color_rgba8888(const uint32 r, const uint32 g, const uint32 b, const uint32 a) {

            ZASSERT(r <= 255 AND g <= 255 AND b <= 255 AND a <= 255);

            const uint32 packedColor = ((r << 24) | (g << 16) | (b << 8) | (a));

            return packedColor;
        }

        static inline RfColor make_RGBA32(const uint32 r, const uint32 g, const uint32 b, const uint32 a) {

            const uint32 packedColor = make_packed_color_rgba8888(r, g, b, a);
            return RfColor(RfColorFormat_RGBA_8888, packedColor);
        }

        static inline RfColor make_RGBA32_PRE(uint32 r, uint32 g, uint32 b, uint32 a) {

            ZASSERT(r <= 255 AND g <= 255 AND b <= 255 AND a <= 255);
            const uint32 packedColor = (((r * a) << 24) | ((g * a) << 16) | ((b * a) << 8) | (a << 0));
            return RfColor(RfColorFormat_RGBA_8888_PRE, packedColor);
        }

        static inline RfColor make_ARGB32(uint32 a, uint32 r, uint32 g, uint32 b) {

            ZASSERT(a <= 255 AND r <= 255 AND g <= 255 AND b <= 255);
            const uint32 packedColor = ((a << 24) | (r << 16) | (g << 8) | (b << 0));
            return RfColor(RfColorFormat_ARGB_8888, packedColor);
        }

        static inline RfColor make_ARGB32_PRE(uint32 a, uint32 r, uint32 g, uint32 b) {

            ZASSERT(a <= 255 AND r <= 255 AND g <= 255 AND b <= 255);
            const uint32 packedColor = ((a << 24) | (r << 16) | (g << 8) | (b << 0));
            return RfColor(RfColorFormat_ARGB_8888_PRE, packedColor);
        }

        static inline RfColor make_RGB565(uint32 r, uint32 g, uint32 b) {

            ZASSERT(r <= 31 AND g <= 63 AND b <= 31);
            const uint32 packedColor = ((r << 11) | (g << 5) | (b << 0));
            return RfColor(RfColorFormat_RGB_565, packedColor);
        }

        static inline RfColor make_A8(uint32 a) {

            ZASSERT(a <= 255);
            return RfColor(RfColorFormat_A8, a);
        }


    } RfColor;

}

#endif

