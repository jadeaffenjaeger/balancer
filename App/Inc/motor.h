#ifndef MOTOR_H_
#define MOTOR_H_

typedef enum {
    MOTOR_LEFT,
    MOTOR_RIGHT
} motor_t;

void motor_init();
int32_t motor_remap(motor_t side, float error, float err_max);
void motor_setSpeed(motor_t side, int32_t output);
#endif
