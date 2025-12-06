#ifndef APPLICATION_TYPES_H
#define APPLICATION_TYPES_H


typedef unsigned char uint8;
typedef unsigned long uint32;

typedef signed char int8;
typedef signed long int32;

typedef float float32;


typedef enum {
    INCREASE,
    DECREASE
} Direction;


typedef struct {
    float32 min;
    float32 max;
} Limitation;


#endif