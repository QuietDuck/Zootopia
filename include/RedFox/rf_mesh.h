#pragma once

/*
RfMesh - Interface
*/

#ifndef __RF_MESH_H__
#define __RF_MESH_H__

#include "rf_shader.h"

namespace zootopia {

    class RfMesh {

    public:

        RfMesh() {};
        virtual ~RfMesh() {};

    public:

        virtual void draw() = 0;

    };

}

#endif