#include "i2c.h"
#include "sensor.h"
#include "mpu6050.h"

static enum sensor_state state = IDLE;
static struct sensor_values values;
static uint8_t buffer[14];
const static struct vec3 gyro_bias = {27.751366, -0.833710, -2.247824};

/* Read sensor data over I2C*/
struct sensor_values * sensor_update() {
    HAL_I2C_Mem_Read_DMA(&hi2c1, MPU6050_ADDRESS_AD0_LOW << 1, MPU6050_RA_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &buffer, sizeof(buffer));
    sensor_parse();
    return &values;
}

/* Turn sensor output into g and deg/s */
void sensor_parse() {
    values.acc.x = ((float) (int16_t) (buffer[0]  << 8 | buffer[1]))    / 16384.0;
    values.acc.y = ((float) (int16_t) (buffer[2]  << 8 | buffer[3]))    / 16384.0;
    values.acc.z = ((float) (int16_t) (buffer[4]  << 8 | buffer[5]))    / 16384.0;

    values.temp = ((float) (int16_t) (buffer[6]  << 8 | buffer[7]))     / 340.0 + 36.53;

    values.gyro.x = ((float) (int16_t) (buffer[8]  << 8 | buffer[9]))    / 131.0;
    values.gyro.y = ((float) (int16_t) (buffer[10]  << 8 | buffer[11]))  / 131.0;
    values.gyro.z = ((float) (int16_t) (buffer[12]  << 8 | buffer[13]))  / 131.0;

    /* Compensate for Gyro drift*/
    values.gyro.x -= gyro_bias.x;
    values.gyro.y -= gyro_bias.y;
    values.gyro.z -= gyro_bias.z;
}

/* Power on sensor and do initial setup*/
void sensor_init() {
    uint8_t buf = MPU6050_CLOCK_PLL_XGYRO;
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS_AD0_LOW << 1, MPU6050_RA_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &buf, 1, 100) != HAL_OK) {
        Error_Handler();
    }
}

/* Print out average over Gyro readings (board needs to be held still to get drift rate)*/
void sensor_calibrate() {
    double gx = 0.0;
    double gy = 0.0;
    double gz = 0.0;

    for(uint32_t i = 0; i < 1000; i++) {
        sensor_update();
        gx += values.gyro.x;
        gy += values.gyro.y;
        gz += values.gyro.z;
        
        HAL_Delay(10);
    }

    gx /= 1000.0;
    gy /= 1000.0;
    gz /= 1000.0;

    char strbuffer[256];
    snprintf(strbuffer, 256, 
        "Gyro Bias Values: %f,%f,%f\r\n", 
        gx,
        gy,
        gz
    );

    CDC_Transmit_FS(strbuffer, strlen(strbuffer));
}
