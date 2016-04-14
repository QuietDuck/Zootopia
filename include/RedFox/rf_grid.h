#pragma once

/*
RfGrid - Interface
*/

#ifndef __RF_GRID_H__
#define __RF_GRID_H__

#include "rf_camera.h"

namespace zootopia {

    class RfGrid {

    public:
        
        RfGrid() {};
        virtual ~RfGrid() {};

    public:

        virtual void draw() = 0;
        virtual void destroy() = 0;

    };

}

#endif