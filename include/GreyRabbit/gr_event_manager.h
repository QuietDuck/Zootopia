#pragma once

/*
GrEventManager - Interface
*/

#ifndef __GR_EVENT_MANAGER__
#define __GR_EVENT_MANAGER__

namespace zootopia {

    class GrEventManager {

    public:
        
        GrEventManager() {};
        virtual ~GrEventManager() {};

    public:

        virtual void initialize() = 0;

        virtual void pollEvents() = 0;
        virtual void waitEvents() = 0;

        virtual void calculateDeltaTime() = 0;

    };

}

#endif