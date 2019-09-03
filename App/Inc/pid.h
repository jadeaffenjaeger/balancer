#ifndef PID_H_
#define PID_H_ 
#include <stdint.h>

typedef struct {
    float kp;
    float ki;
    float kd;

    float outMax;
    float outMin;
    float direction;

    float last_measure;
    float err_sum;
    float set_point;
} pidc_t;

float pid_compute(pidc_t *values, float measure, uint32_t dt, const float TICKS_PER_SECOND);
#endif