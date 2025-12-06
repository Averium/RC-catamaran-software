#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H


#include "application_types.h"


#define MS_TO_S 0.001f
#define S_TO_MS 1000.0f
#define US_TO_S 0.000001f
#define S_TO_US 1000000.0f

#define US_TO_MS (MS_TO_S)
#define MS_TO_US (S_TO_MS)


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