#pragma once

/*
GrWindowManager - Interface
*/

#ifndef __GR_WINDOW_MANAGER_H__
#define __GR_WINDOW_MANAGER_H__

#include <string>

#include "rf_size.h"

namespace zootopia {

    class GrWindowManager {

    public:

        GrWindowManager() : _windowName("Initial Window"), _windowSize(RfSize(0,0)){};
        virtual ~GrWindowManager() {};

    public:

        virtual void initialize() = 0;
        virtual void createWindow(const RfSize& windowSize) = 0;
        virtual void show() = 0;
        virtual void swapBuffers() = 0;
        virtual void terminate() = 0;

        virtual int shouldClose() = 0;

        RfSize getWindowSize() { return _windowSize; };

    protected:

        std::string     _windowName;
        RfSize          _windowSize;

    };

}

#endif
