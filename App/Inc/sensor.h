#ifndef SENSOR_H_
#define SENSOR_H_
#include "vec3.h"

enum sensor_state {
    IDLE,
    NEW_DATA
};

struct sensor_values {
    struct vec3 acc;
    struct vec3 gyro;
    float temp;
};

void sensor_parse();
struct sensor_values * sensor_getValues();
void sensor_read();
void sensor_init();

#endif // SENSOR_H_