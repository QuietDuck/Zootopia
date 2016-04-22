#pragma once

/*
RfNoncopyable - Type
*/

#ifndef __RF_NONCOPYABLE_H__
#define __RF_NONCOPYABLE_H__

namespace zootopia {

    class RfNoncopyable {
    protected:
        RfNoncopyable() {}
        ~RfNoncopyable() {}

    private:
        RfNoncopyable(const RfNoncopyable&) {}
        RfNoncopyable& operator=(const RfNoncopyable&) {}
    };
}

#endif

