#include <stdint.h>
#include <math.h>
#include "angle.h"
#include "sensor.h"
#include "vec3.h"

const float FILTER_COEFF = 0.98;
const float RAD_TO_DEG = 180/M_PI;
const float DEG_TO_RAD = M_PI/180;

static float fast_atan2(float x, float y);
static float angle_filter(float curr_ang, float acc_ang, float gyro, float dt);

static struct vec3 angles = {0.0, 0.0, 0.0};

/* Read sensor data and calculate new angles*/
struct vec3 * angle_update(uint32_t tick) {

    /* Calculate time since last measurement*/
    const float TICKS_PER_SECOND = 1000.0;
    static uint32_t last_tick = 0;
    struct sensor_values * raw_data = sensor_getValues();
    float dt_s = (tick - last_tick) / TICKS_PER_SECOND;
    last_tick = tick;

    /* Calculate angles according to accelerometers*/
    float acc_x = fast_atan2(raw_data->acc.y, raw_data->acc.z) * RAD_TO_DEG;
    float acc_y = fast_atan2(raw_data->acc.x, raw_data->acc.z) * RAD_TO_DEG;
    float acc_z = fast_atan2(raw_data->acc.x, raw_data->acc.y) * RAD_TO_DEG;

    /* Smooth Accelerometer data with Gyros*/
    angles.x = angle_filter(angles.x, acc_x, -raw_data->gyro.x, dt_s);
    angles.y = angle_filter(angles.y, acc_y, raw_data->gyro.y, dt_s);
    angles.z = angle_filter(angles.z, acc_z, -raw_data->gyro.z, dt_s);

    return &angles;
}

/*generic complementary filter*/
static float angle_filter(float curr_ang, float acc_ang, float gyro, float dt) {
    return (FILTER_COEFF * (curr_ang + gyro * dt) + (1-FILTER_COEFF) * acc_ang);
}

/*
 *fast arctan approximation, read here for details:
 *https://math.stackexchange.com/questions/1098487/atan2-faster-approximation/1105038
 */
float fast_atan2(float x, float y) {
    if(x != 0 && y != 0) {
        float a = fmin(fabsf(x), fabsf(y)) / fmax(fabsf(x), fabsf(y));
        float s = a*a;
        float r = ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a;

        if (fabsf(y) > fabsf(x)) r = M_PI/2 - r;
        if (x < 0) r = M_PI - r;
        if (y < 0) r = -r;
        
        return r;
    }

    if(x == 0) {
        if(y > 0) return M_PI/2;
        if(y < 0) return -M_PI/2;
    }

    return 0;
}
