#pragma once

#ifndef __BC_CORE_H__
#define __BC_CORE_H__

#include <vector>

#include "rf_compositor.h"
#include "bc_resource_manager.h"
#include "gr_window_manager.h"
#include "gr_event_manager.h"

namespace zootopia {

    class BcCore {

    private:

        BcCore();
        ~BcCore();

    public:

        enum BackendType {
            kOpenGL,
            kDirectX
        };

        static BcCore*  getInstance();

        void initialize(BackendType backend);
        void terminate();
        void run();

        void compositorSetUp();

        BackendType getBackendType() const {
            return _backendType;
        }

    private:

        BackendType         _backendType;

        BcResourceManager*  _resourceManager;

        GrWindowManager*    _windowManager;
        GrEventManager*     _eventManager;

        RfCompositor*       _compositor;
    };

}

#endif