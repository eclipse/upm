/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This driver was rewritten based on the original driver written by:
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 *
 * The MIT License
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

<<<<<<< HEAD
#include <string>
#include <mraa/i2c.hpp>
#include <math.h>
#include "iTemperatureSensor.h"

#define ADDR               0x77 // device address

// registers address
#define BMP085_ULTRALOWPOWER 0
#define BMP085_STANDARD      1
#define BMP085_HIGHRES       2
#define BMP085_ULTRAHIGHRES  3
#define BMP085_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define BMP085_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define BMP085_CAL_AC3           0xAE  // R   Calibration data (16 bits)
#define BMP085_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define BMP085_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define BMP085_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define BMP085_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define BMP085_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define BMP085_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define BMP085_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define BMP085_CAL_MD            0xBE  // R   Calibration data (16 bits)

#define BMP085_CONTROL           0xF4
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD       0x2E
#define BMP085_READPRESSURECMD   0x34

#define HIGH               1
#define LOW                0

namespace upm {

/**
 * @brief Bosch BMP & GY65 Atmospheric Pressure Sensor library
 * @defgroup bmpx8x libupm-bmpx8x
 * @ingroup seeed adafruit sparkfun i2c pressure
 */

/**
 * @library bmpx8x
 * @sensor bmpx8x
 * @comname BMP Atmospheric Pressure Sensor
 * @altname GY65 BMP085 BMP180 BMP183
 * @type pressure
 * @man seeed adafruit sparkfun
 * @con i2c
 *
 * @brief API for the GY65/BMP085 and BMP180 Atmospheric Pressure Sensors
 *
 * Bosch GY65/BMP085 and BMP180 are high-precision, ultra-low
 * power consumption pressure sensors. They operate in the range of
 * 30,000-110,000 Pa.
 *
 * This module has been tested on the GY65/BMP085 and BMP180 sensors.
 *
 * @image html bmp085.jpeg
 * @snippet bmpx8x.cxx Interesting
 */

class BMPX8X {
    public:
        /**
         * Instantiates a BMPX8X object
         *
         * @param bus Number of the used bus
         * @param devAddr Address of the used I2C device
         * @param mode BMP085 mode
         */
        BMPX8X (int bus, int devAddr=0x77, uint8_t mode=BMP085_ULTRAHIGHRES);

        /**
         * BMPX8X object destructor; basically, it closes the I2C connection.
         * ~BMPX8X ();
         * LE: there is no need for the destructor, as the I2C connection
         * will be closed when the m_i2ControlCtx variable will go out of
         * scope (when all the BMPX8X objects will be destroyed)
         */
        /**
         * Returns the calculated pressure
         */
        int32_t getPressure ();

        /**
         *
         * Gets raw pressure data
         */
        int32_t getPressureRaw ();

        /**
         * Gets raw temperature data from the sensor
         */
        int16_t getTemperatureRaw ();

        /**
         * Returns the calculated temperature
         */
        float getTemperature ();

        /**
         * With a given absolute altitude, sea level can be calculated
         *
         * @param altitudeMeters Altitude
         */
        int32_t getSealevelPressure(float altitudeMeters = 0);

        /**
         * With a given sea level, altitude in meters can be calculated
         *
         * @param sealevelPressure Sea level
         */
        float getAltitude (float sealevelPressure = 101325);

        /**
         * Return latest calculated temperature value in Celcius
         * See ITemperatureSensor
         */
        int getTemperatureCelcius();

        /**
         * Returns name of module. This is the string in library name after libupm_

         * @return name of module
         */
        const char* getModuleName();

        /**
         * Calculates B5 (check the spec for more information)
         *
         * @param UT
         */
        int32_t computeB5 (int32_t UT);

        /**
         * Reads a two-byte register
         *
         * @param reg Address of the register
         */
        uint16_t i2cReadReg_16 (int reg);

        /**
         * Writes to a one-byte register
         *
         * @param reg Address of the register
         * @param value Byte to be written
         */
        mraa::Result i2cWriteReg (uint8_t reg, uint8_t value);

        /**
         * Reads a one-byte register
         *
         * @param reg Address of the register
         */
        uint8_t i2cReadReg_8 (int reg);

    private:
        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c m_i2ControlCtx;

        uint8_t oversampling;
        int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
        uint16_t ac4, ac5, ac6;
};

=======
#include <unistd.h>
#include <string.h>

#include <mraa/i2c.h>

#include "upm.h"

#include "bmpx8x_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


    /**
     * @file bmpx8x.h
     * @library bmpx8x
     * @brief C API for the bmpx8x driver
     *
     * @include bmpx8x.c
     */

    /**
     * Device context
     */
    typedef struct _bmpx8x_context {
        mraa_i2c_context i2c;

        // our oversampling (precision)
        BMPX8X_OSS_T oversampling;

        // compensated temperature and pressure
        float temperature;
        int pressure;

        // compensation coefficients
        int16_t ac1;
        int16_t ac2;
        int16_t ac3;
        uint16_t ac4;
        uint16_t ac5;
        uint16_t ac6;
        int16_t b1;
        int16_t b2;
        int16_t mb;
        int16_t mc;
        int16_t md;
    } *bmpx8x_context;


    /**
     * BMPX8X initialization.
     *
     * By default, the device is initialized to it's highest accuracy
     * (BMP085_OSS_ULTRAHIGHRES).
     *
     * @param bus I2C bus number.
     * @param addr I2C address of the device.
     * @return Device context, or NULL if an error occurred.
     */
    bmpx8x_context bmpx8x_init(int bus, int addr);

    /**
     * BMPX8X close function.
     *
     * @param dev Device context.
     */
    void bmpx8x_close(bmpx8x_context dev);

    /**
     * Return the chip ID.
     *
     * @param dev The device context.
     * @return The chip ID.
     */
    uint8_t bmpx8x_get_chip_id(const bmpx8x_context dev);

    /**
     * Initialize the device, read calibration data, and start
     * operation.  This function is called from bmpx8x_init() so it
     * will not typically need to be called by a user unless the
     * device is reset.  This method will call
     * bmpx8x_set_oversampling() with the passed parameter.
     *
     * @param dev The device context.
     * @param oss One of the BMPX8X_OSS_T values.  The default set
     * at bmpx8x_init() time is BMP085_OSS_ULTRAHIGHRES.
     * @return UPM result.
     */
    upm_result_t bmpx8x_devinit(const bmpx8x_context dev,
                                BMPX8X_OSS_T oss);

    /**
     * Perform a device reset.  The device will be reset as if it was
     * just powered on.  All compensation values will be lost.  You
     * should call bmpx8x_devinit() afterward, or perform the same
     * steps that bmpx8x_devinit() performs before attempting to use
     * the device.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmpx8x_reset(const bmpx8x_context dev);

    /**
     * Update the internal stored values from sensor data.
     *
     * @param dev The device context.
     * @return UPM result.
     */
    upm_result_t bmpx8x_update(const bmpx8x_context dev);

    /**
     * Set the oversampling (precision mode) of the device.  Higher
     * precision requires more time to complete.
     *
     * @param dev The device context.
     * @param oss The desired oversampling mode, one of the
     * BMPX8X_OSS_T values.
     */
    void bmpx8x_set_oversampling(const bmpx8x_context dev,
                                 BMPX8X_OSS_T oss);

    /**
     * Returns the pressure in Pascals.  bmpx8x_update() must have
     * been called prior to calling this function.
     *
     * @param dev Device context.
     * @return The pressure in Pascals.
     */
    int bmpx8x_get_pressure(const bmpx8x_context dev);

    /**
     * Returns the temperature in degrees Celsius.  bmpx8x_update()
     * must have been called prior to calling this function.
     *
     * @param dev Device context.
     * @return The temperature in degrees Celsius.
     */
    float bmpx8x_get_temperature(const bmpx8x_context dev);

    /**
     * With a given current altitude, calculate pressure at sea level.
     * bmpx8x_update() must have been called prior to calling this
     * function.
     *
     * @param dev Device context.
     * @param altitude Current altitude in Meters.
     * @return The pressure in Pascals at sea level.
     */
    int bmpx8x_get_sealevel_pressure(const bmpx8x_context dev,
                                     float altitude);

    /**
     * With a given sea level, calculate altitude in meters.
     * bmpx8x_update() must have been called prior to calling this
     * function.
     *
     * @param dev Device context.
     * @param sealevel Sea level pressure in Pascals. If a negative
     * number, or zero is supplied, a default sealevel of 101325 Pa
     * will be used instead.
     * @return The current altitude in Meters.
     */
    float bmpx8x_get_altitude(const bmpx8x_context dev, int sealevel);

    /**
     * Read a register.
     *
     * @param dev The device context.
     * @param reg The register to read.
     * @return The value of the register.
     */
    uint8_t bmpx8x_read_reg(const bmpx8x_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer.
     *
     * @param dev The device context.
     * @param buffer The buffer to store the results.
     * @param len The number of registers to read.
     * @return The number of bytes read, or -1 on error.
     */
    int bmpx8x_read_regs(const bmpx8x_context dev, uint8_t reg,
                         uint8_t *buffer, int len);

    /**
     * Write to a register.
     *
     * @param dev The device context.
     * @param reg The register to write to.
     * @param val The value to write.
     * @return UPM result.
     */
    upm_result_t bmpx8x_write_reg(const bmpx8x_context dev,
                                  uint8_t reg, uint8_t val);

#ifdef __cplusplus
>>>>>>> master
}
#endif
