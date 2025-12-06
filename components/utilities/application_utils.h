#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H


#include "application_types.h"


inline void saturate_in_place(float32* value, const Limitation limit) {
    if (*value < limit.min) { *value = limit.min; }
    if (*value > limit.max) { *value = limit.max; }
}


inline float32 saturate(float32 value, const Limitation limit) {
    if (value < limit.min) { return limit.min; }
    if (value > limit.max) { return limit.max; }
    return value;
}


#endif