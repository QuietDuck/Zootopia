#pragma once

/*
RfPointLightGL - Class
*/

#ifndef __RF_POINT_LIGHT_GL_H__
#define __RF_POINT_LIGHT_GL_H__

#include "rf_light.h"
#include "rf_light_manager_gl.h"
#include "rf_point_light.h"

#include "rf_scalar.h"
#include "rf_point4.h"

namespace zootopia {

    class RfPointLightGL : public RfPointLight {

        friend class RfLightManagerGL;

    public:

        explicit RfPointLightGL(const RfColor& color, const RfPoint3& position);
        ~RfPointLightGL();

    public:

        void setPosition(const RfPoint3& position) override;
        void setColor(const RfColor& color) override;
        void setProperties(const RfScalar linear, const RfScalar quadratic) override;

    private:

        void    _setIndex(uint32 index) { _values.index = index; }
        uint32  _getIndex() const { return _values.index; }

        void    _calculateRadius();
        void    _update(const GLintptr offset, const GLsizeiptr size, const GLvoid* data);

        RfScalar    _constant;
        RfScalar    _threshold;
        RfScalar    _maxBrightness;
        
    };

}

#endif