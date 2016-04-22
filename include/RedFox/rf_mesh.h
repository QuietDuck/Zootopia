#pragma once

/*
RfMesh - Interface
*/

#ifndef __RF_MESH_H__
#define __RF_MESH_H__

#include "rf_noncopyable.h"

namespace zootopia {

    class RfMesh : private RfNoncopyable {

    public:

        RfMesh() {};
        virtual ~RfMesh() {};

    public:

        virtual void draw() = 0;

    };

}

#endif