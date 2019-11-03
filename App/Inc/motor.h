#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_L_DIR 1
#define MOTOR_R_DIR 1

//PWM limit at which the motor spins reliably
#define LOWER_LIMIT 35

void motor_init();
int32_t motor_remap(float error, float err_max);
void motor_setSpeed(int32_t speed);
#endif
