#include "stm32f1xx_hal.h"
#include "motor.h"
#include <stdint.h>
#include <math.h>

extern TIM_HandleTypeDef htim2;

const uint32_t MOTOR_RIGHT_MIN = 350;
const uint32_t MOTOR_LEFT_MIN = 300;
const uint32_t MOTOR_MAX = 1000;

void motor_init()
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    motor_setSpeed(MOTOR_LEFT, 0);
    motor_setSpeed(MOTOR_RIGHT, 0);
}

int32_t motor_remap(motor_t side, float error, float err_max)
{
    if (error == 0) {
        return 0;
    }

    uint32_t motor_min_pwm;
    if (side == MOTOR_RIGHT) {
        motor_min_pwm = MOTOR_RIGHT_MIN;
    } else {
        motor_min_pwm = MOTOR_LEFT_MIN;
    }

    /* Rescale error to motor dynamic range*/
    float output = error * ((float)(MOTOR_MAX - motor_min_pwm)/err_max);

    if (error < 0) {
        return ((int32_t) output) - motor_min_pwm;
    } else {
        return ((int32_t) output) + motor_min_pwm;
    }

}

// Write speed to PWM outputs
void motor_setSpeed(motor_t side, int32_t output)
{
    if (output >= 0)
    {
        if (side == MOTOR_RIGHT) {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, output);
        } else {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, output);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
        }
    }
    else 
    {
        output = -output;
        if (side == MOTOR_RIGHT) {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, output);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
        } else {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, output);
        }
    }
}
