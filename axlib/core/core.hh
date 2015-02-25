#ifndef AXLIB_CORE_CORE_HH
#define AXLIB_CORE_CORE_HH

#include "axlib/core/core.h"

namespace axlib {

#include <stdint.h>

extern uint8_t BitFlip(uint8_t n);

template<class T>
T min(const T a, const T b)
{
    if (a < b) {
        return a;
    }
    return b;
}

template<class T>
T max(const T a, const T b)
{
    if (a >= b) {
        return a;
    }
    return b;
}

template<class T>
T abs(const T a)
{
    if (a < 0) {
        return -a;
    }
    return a;
}

} //namespace axlib

#endif //ifndef AXLIB_CORE_CORE_HH
