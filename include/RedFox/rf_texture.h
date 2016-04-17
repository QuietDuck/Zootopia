#pragma once

#ifndef __RF_TEXTURE_H__
#define __RF_TEXTURE_H__

#include <string>

#include "rf_size.h"

namespace zootopia {

    class RfTexture {

    public:

        RfTexture() {};
        virtual ~RfTexture() {};

    public:

        enum Type {
            kDiffuse,
            kSpecular,
            kNormal,
            kHeight
        };

        Type getType() const { return _type; }
        RfSize getSize() const { return _size; }

    protected:

        Type    _type;
        RfSize  _size;

    };

}

#endif
