#include "pid.h"

#include <math.h>

const float MIN_ERROR = 1.25;

float pid_compute(pidc_t *values, float measure, uint32_t dt, const float TICKS_PER_SECOND) {

    if (dt == 0.0 || TICKS_PER_SECOND == 0) {
        return 0;
    }
    /*calculate current error*/
    float error = values->set_point - measure;

    float dt_s = dt / TICKS_PER_SECOND;

    /*calulate integral term*/
    values->err_sum += values->ki * error * dt_s;

    /*calulate differential term*/
    float err_diff = (measure - values->last_measure) / dt_s;
    values->last_measure = measure;

    if (-MIN_ERROR < error && error < MIN_ERROR) {
        return 0;
    }

    /*calculate controller output*/
    float output = values->direction * values->kp * error + values->err_sum - values->kd * err_diff;

    /*clamp output values*/
    if (output > values->outMax) output = values -> outMax;
    if (output < values->outMin) output = values -> outMin;

    return output;
}
