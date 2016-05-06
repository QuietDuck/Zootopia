#pragma once

/*
RfSpotLightGL - Class
*/

#ifndef __RF_SPOT_LIGHT_GL_H__
#define __RF_SPOT_LIGHT_GL_H__

#include <GL/glew.h>

#include "rf_spot_light.h"

namespace zootopia {

    class RfSpotLightGL : public RfSpotLight {

        friend class RfLightManagerGL;

    public:

        explicit RfSpotLightGL(const RfPoint3& position, const RfVector3& direction, const RfColor& color);
        ~RfSpotLightGL();

    public:

        void setPosition(const RfPoint3& position) override;
        void setDirection(const RfVector3& direction) override;
        void setColor(const RfColor& color) override;
        void setCutOff(const RfScalar cutOff, const RfScalar outerCutOff) override;
        void setProperties(const RfScalar linear, const RfScalar quadratic) override;


    private:

        void    _setIndex(uint32 index) { _values.index = index; }
        uint32  _getIndex() const { return _values.index; }

        void    _update(const GLintptr offset, const GLsizeiptr size, const GLvoid* data);

    };

}

#endif