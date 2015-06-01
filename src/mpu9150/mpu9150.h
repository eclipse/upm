/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Based on InvenSense MPU-6050 register map document rev. 2.0, 5/19/2011 (RM-MPU-6000A-00)
 * 8/24/2011 by Jeff Rowberg <jeff@rowberg.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <mraa/i2c.h>

#define MPU6050_ADDRESS_AD0_LOW             0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH            0x69 // address pin high (VCC)
#define ADDR                                MPU6050_ADDRESS_AD0_LOW // device address

// registers address
#define MPU6050_CLOCK_PLL_XGYRO             0x01
#define MPU6050_GYRO_FS_250                 0x00
#define MPU6050_ACCEL_FS_2                  0x00
#define MPU6050_RA_INT_PIN_CFG              0x37

#define MPU6050_RA_ACCEL_XOUT_H             0x3B
#define MPU6050_RA_ACCEL_XOUT_L             0x3C
#define MPU6050_RA_ACCEL_YOUT_H             0x3D
#define MPU6050_RA_ACCEL_YOUT_L             0x3E
#define MPU6050_RA_ACCEL_ZOUT_H             0x3F
#define MPU6050_RA_ACCEL_ZOUT_L             0x40
#define MPU6050_RA_TEMP_OUT_H               0x41
#define MPU6050_RA_TEMP_OUT_L               0x42
#define MPU6050_RA_GYRO_XOUT_H              0x43
#define MPU6050_RA_GYRO_XOUT_L              0x44
#define MPU6050_RA_GYRO_YOUT_H              0x45
#define MPU6050_RA_GYRO_YOUT_L              0x46
#define MPU6050_RA_GYRO_ZOUT_H              0x47
#define MPU6050_RA_GYRO_ZOUT_L              0x48

#define MPU6050_RA_CONFIG                   0x1A
#define MPU6050_CFG_DLPF_CFG_BIT            2
#define MPU6050_CFG_DLPF_CFG_LENGTH         3

#define MPU6050_RA_GYRO_CONFIG              0x1B
#define MPU6050_GCONFIG_FS_SEL_BIT          4
#define MPU6050_GCONFIG_FS_SEL_LENGTH       2

#define MPU6050_RA_ACCEL_CONFIG             0x1C
#define MPU6050_ACONFIG_AFS_SEL_BIT         4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH      2

// magnotometer
#define MPU9150_RA_MAG_ADDRESS              0x0C
#define MPU9150_RA_MAG_XOUT_L               0x03

#define MPU6050_RA_PWR_MGMT_1               0x6B
#define MPU6050_PWR1_CLKSEL_BIT             2
#define MPU6050_PWR1_CLKSEL_LENGTH          3
#define MPU6050_PWR1_SLEEP_BIT              6

#define MPU6050_RA_INT_PIN_CFG              0x37

// temperature
#define MPU6050_PWR1_TEMP_DIS_BIT           3
#define MPU6050_RA_WHO_AM_I                 0x75
#define MPU6050_WHO_AM_I_BIT                6
#define MPU6050_WHO_AM_I_LENGTH             6

#define SMOOTH_TIMES                        10.0

#define HIGH               1
#define LOW                0

namespace upm {

struct Vector3DRaw {
    uint16_t axisX;
    uint16_t axisY;
    uint16_t axisZ;
};

struct Vector3D {
    double axisX;
    double axisY;
    double axisZ;
};

struct AxisData {
    Vector3DRaw rawData;
    Vector3D    sumData;
    Vector3D    data;
};

/**
 * @brief mpu9150 accelerometer library
 * @defgroup mpu9150 libupm-mpu9150
 * @ingroup seeed i2c accelerometer compass
 */
/**
 * @library mpu9150
 * @sensor mpu9150
 * @comname mpu9150 accelerometer library
 * @type accelerometer compass
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_IMU_9DOF_v1.0
 * @con i2c
 *
 * @brief API for MPU9150 chip (Accelerometer, Gyro and Magnometer Sensor)
 *
 * This file defines the MPU9150 interface for libmpu9150
 *
 * @image html mpu9150.jpg
 * @snippet mpu9150.cxx Interesting
 */
class MPU9150 {
    public:
        /**
         * Instanciates a MPU9150 object
         *
         * @param bus number of used bus
         * @param devAddr addres of used i2c device
         */
        MPU9150 (int bus=0, int devAddr=0x68);

        /**
         * MPU9150 object destructor, basicaly it close i2c connection.
         */
        ~MPU9150 ();

        /**
         * Initiate MPU9150 chips
         */
        mraa_result_t initSensor ();

        /**
         * Get identity of the device
         */
        uint8_t getDeviceID ();

        /**
         * Get the Accelerometer, Gyro and Compass data from the chip and
         * save it in private section.
         */
        mraa_result_t getData ();

        /**
         * @param data structure with 3 axis (x,y,z)
         */
        mraa_result_t getAcceleromter (Vector3D * data);

        /**
         * @param data structure with 3 axis (x,y,z)
         */
        mraa_result_t getGyro (Vector3D * data);

        /**
         * @param data structure with 3 axis (x,y,z)
         */
        mraa_result_t getMagnometer (Vector3D * data);

        /**
         * Read on die temperature from the chip
         */
        float getTemperature ();

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }

    private:
        std::string m_name;

        int m_i2cAddr;
        int m_bus;
        mraa_i2c_context m_i2Ctx;

        AxisData axisMagnetomer;
        AxisData axisAcceleromter;
        AxisData axisGyroscope;

        uint16_t i2cReadReg_N (int reg, unsigned int len, uint8_t * buffer);
        mraa_result_t i2cWriteReg (uint8_t reg, uint8_t value);
        int updateRegBits (uint8_t reg, uint8_t bitStart,
                                    uint8_t length, uint16_t data);
        uint8_t getRegBits (uint8_t reg, uint8_t bitStart,
                                    uint8_t length, uint8_t * data);
};

}
