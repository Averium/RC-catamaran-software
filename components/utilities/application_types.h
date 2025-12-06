#ifndef APPLICATION_TYPES_H
#define APPLICATION_TYPES_H


typedef unsigned char uint8;
typedef unsigned long uint32;

typedef signed char int8;
typedef signed long int32;

typedef float float32;


typedef enum {
    ERROR_OK,
    ERROR_INVALID_OBJECT_POINTER,
    ERROR_FAILED_ESP_CALL,
    ERROR_OUT_OF_RANGE_FUNCTION_ARGUMENT,
    ERROR_INVALID_FUNCTION_ARGUMENT,
    N_ERRORS,
} ApplicationErrorType;


typedef enum {
    CALIBRATION_FINISHED,
    CALIBRATION_ONGOING,
    CALIBRATION_NOT_STARTED,
    CALIBRATION_FAILED,
} ApplicationCalibrationState;


typedef struct {
    float32 min;
    float32 max;
} Limitation;


#endif