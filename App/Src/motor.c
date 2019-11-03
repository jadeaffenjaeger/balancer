#include "stm32f1xx_hal.h"
#include "motor.h"
#include <stdint.h>
#include <math.h>

extern TIM_HandleTypeDef htim2;

const uint32_t MOTOR_MIN = 100;
const uint32_t MOTOR_MAX = 1000;
const float DEADBAND = 1.5;

void motor_init()
{

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    motor_setSpeed(0);
}

int32_t motor_remap(float error, float err_max)
{
    // Turn motors off when error is sufficiently small
    if (-DEADBAND < error && error < DEADBAND)
    {
        return 0;
    }

    // Scale to -1 ... 1
    error /= err_max;

    float output = error * (float)(MOTOR_MAX - MOTOR_MIN);
    //output = round(output);

    if (error < 0) {
        return ((int32_t) output) - MOTOR_MIN;
    } else {
        return ((int32_t) output) + MOTOR_MIN;
    }

}

// Write speed to PWM outputs
void motor_setSpeed(int32_t output)
{
    if (output >= 0)
    {
        // 0 to CH1 and CH3
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);

        // output to CH2 and CH4
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, output);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, output);
    }
    else
    {
        // Inverted output to CH1 and CH3
        output = -output;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, output);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, output);

        // 0 to CH2 and CH4
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    }
}
