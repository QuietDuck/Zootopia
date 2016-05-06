#pragma once

/*
RfDirLightGL - Class
*/

#ifndef __RF_DIR_LIGHT_GL_H__
#define __RF_DIR_LIGHT_GL_H__

#include <GL/glew.h>

#include "rf_dir_light.h"

#include "rf_point3.h"

namespace zootopia {

    class RfDirLightGL : public RfDirLight {

        friend class RfLightManagerGL;

    public:

        explicit RfDirLightGL(const RfVector3& direction, const RfColor& color);
        ~RfDirLightGL();

    public:

        void setDirection(const RfVector3& direction) override;
        void setColor(const RfColor& color) override;

    private:

        void    _setIndex(uint32 index) { _values.index = index; }
        uint32  _getIndex() const { return _values.index; }

        void    _update(const GLintptr offset, const GLsizeiptr size, const GLvoid* data);
        

    };

}

#endif
