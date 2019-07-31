/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/i2c.h>
#include <mraa/spi.h>
#include <mraa/gpio.h>

#include "bmp280_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file bmp280.h
     * @library bmp280
     * @brief C API for the bmp280 and bme280 driver
     *
     * @include bmp280.c
     */

    /**
     * Device context
     */
    typedef struct _bmp280_context {
        mraa_i2c_context i2c;
        mraa_gpio_context gpio;
        mraa_spi_context spi;

        // are we doing SPI?
        bool isSPI;

        // are we using a bme280?
        bool isBME;

        // always stored in C
        float temperature;

        // pressure in Pa
        float pressure;

        // humidity (relative)
        float humidity;

        // sea level pressure in hectoPascals (hPa)
        float sea_level_hPA;

        // shared calibration data - set in temp conversion, used in
        // pressure conversion.
        int32_t t_fine;

        // current operating mode.  BMP280_MODE_FORCED requires
        // special attention in update()
        BMP280_MODES_T mode;

        // calibration data temperature
        uint16_t dig_T1;
        int16_t dig_T2;
        int16_t dig_T3;

        // calibration data pressure
        uint16_t dig_P1;
        int16_t dig_P2;
        int16_t dig_P3;
        int16_t dig_P4;
        int16_t dig_P5;
        int16_t dig_P6;
        int16_t dig_P7;
        int16_t dig_P8;
        int16_t dig_P9;

        // calibration data humidity (BME280 only)
        uint8_t dig_H1;
        int16_t dig_H2;
        uint8_t dig_H3;
        int16_t dig_H4;
        int16_t dig_H5;
        int8_t dig_H6;
    } *bmp280_context;

    /**
     * BMP280 initialization.
     *
     * This driver supports both the BMP280 and the BME280.  The
     * BME280 adds a humidity sensor.  The device type is detected
     * automatically by querying the chip id register.
     *
     * Both I2C and SPI accesses are supported. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument.  If you are using a
     * hardware CS pin you cannot control (Intel Edison), then you can
     * connect the proper pin to the hardware CS pin on your MCU and
     * supply -1 for cs.  The default operating mode is I2C.
     *
     * @param bus I2C or SPI bus to use.
     * @param address The I2C address for this device.  Use -1 for SPI.
     * @param cs The gpio pin to use for the SPI Chip Select.  Use -1
     * for I2C, or for SPI with a hardware controlled pin.
     * @return Device context, or NULL on error.
     */
    bmp280_context bmp280_init(int bus, int addr, int cs);

    /**
     * BMP280 close function.
     *
     * @param dev Device context.
     */
    void bmp280_close(bmp280_context dev);

    /**
     * Update the internal stored values from sensor data.
     *
     * @param dev Device context.
     * @return UPM result.
     */
    upm_result_t bmp280_update(const bmp280_context dev);

    /**
     * Return the chip ID.
     *
     * @param dev Device context.
     * @return The chip ID (BMP280_CHIPID).
     */
    uint8_t bmp280_get_chip_id(const bmp280_context dev);

    /**
     * Reset the sensor, as if by a power-on-reset.
     *
     * @param dev Device context.
     */
    void bmp280_reset(const bmp280_context dev);

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature - this is the temperature used to fine tune
     * the pressure measurement.  bmp280_update() must have been called prior
     * to calling this method.
     *
     * @param dev Device context.
     * @return The temperature in degrees Celsius.
     */
    float bmp280_get_temperature(const bmp280_context dev);

    /**
     * Return the current measured relative humidity (bme280 only).
     * bmp280_update() must have been called prior to calling this
     * method.  For a bmp280, the returned value will always be 0.
     *
     * @param dev Device context.
     * @return The relative humidity in percent.
     */
    float bmp280_get_humidity(const bmp280_context dev);

    /**
     * Return the current measured pressure in Pascals (Pa).
     * bmp280_update() must have been called prior to calling this
     * method.
     *
     * @param dev Device context.
     * @return The pressure in Pascals (Pa).
     */
    float bmp280_get_pressure(const bmp280_context dev);

    /**
     * Set the default Sea Level Pressure in hectoPascals (hPA).  A
     * default of 1013.25 (101325 Pa) is set during bmp280_init().
     * This value is used for altitude computation.
     *
     * @param dev Device context.
     * @param seaLevelhPA The pressure at sea level in hectoPascals
     * (hPa).
     */
    void bmp280_set_sea_level_pressure(const bmp280_context dev,
                                       float seaLevelhPA);

    /**
     * Return the current computed altitude in meters.
     * bmp280_update() must have been called prior to calling this
     * method.
     *
     * Computing this value requires knowing the pressure at sea
     * level.  bmp280_init() sets this by default to 1013.25 hPA.  Use
     * bmp280_set_sea_level_pressure() to change this value.
     *
     * @param dev Device context.
     * @return The computed altitude in meters.
     */
    float bmp280_get_altitude(const bmp280_context dev);

    /**
     * Set a general usage mode.  This function can be used to
     * configure the filters and oversampling for a particular use
     * case.  These setting are documented in the BMP280 datasheet.
     * The default mode set in the bmp280_init() function is
     * BMP280_USAGE_MODE_INDOOR_NAV, the highest resolution mode.
     *
     * @param dev Device context.
     * @param mode One of the BMP280_USAGE_MODE_T values.
     */
    void bmp280_set_usage_mode(const bmp280_context dev,
                               BMP280_USAGE_MODE_T mode);

    /**
     * Set the temperature sensor oversampling parameter.  See the
     * data sheet for details.  This value can be automatically set to
     * a suitable value by using one of the predefined modes for
     * bmp280_set_usage_mode().
     *
     * @param dev Device context.
     * @param rate One of the BMP280_OSRS_T_T values.
     */
    void bmp280_set_oversample_rate_temperature(const bmp280_context dev,
                                                BMP280_OSRS_T_T rate);

    /**
     * Set the pressure sensor oversampling parameter.  See the
     * data sheet for details.  This value can be automatically set to
     * a suitable value by using one of the predefined modes for
     * bmp280_set_usage_mode().
     *
     * @param dev Device context.
     * @param rate One of the BMP280_OSRS_P_T values.
     */
    void bmp280_set_oversample_rate_pressure(const bmp280_context dev,
                                             BMP280_OSRS_P_T rate);

    /**
     * Set the humidity sensor oversampling parameter (BME280 only).
     * See the data sheet for details.  This value can be
     * automatically set to a suitable value by using one of the
     * predefined modes for bmp280_set_usage_mode().
     *
     * @param dev Device context.
     * @param rate One of the BME280_OSRS_H_T values.
     */
    void bmp280_set_oversample_rate_humidity(const bmp280_context dev,
                                             BME280_OSRS_H_T rate);

    /**
     * Set the timer standby value.  When in NORMAL operating mode,
     * this timer governs how long the chip will wait before
     * performing a measurement.  See the data sheet for details.
     *
     * @param dev Device context.
     * @param tsb One of the BMP280_T_SB_T values.
     */
    void bmp280_set_timer_standby(const bmp280_context dev,
                                  BMP280_T_SB_T tsb);

    /**
     * Set the IIR filtering parameter.  See the data sheet for
     * details.  This value can be automatically set to a suitable
     * value by using one of the predefined modes for
     * bmp280_set_usage_mode().
     *
     * @param dev Device context.
     * @param filter One of the BMP280_FILTER_T values.
     */
    void bmp280_set_filter(const bmp280_context dev, BMP280_FILTER_T filter);

    /**
     * Set the default measuring mode.  Basic values are forced,
     * sleep, and normal.  See the data sheet for details.  This value
     * can be automatically set to a suitable value by using one of
     * the predefined modes for bmp280_set_usage_mode().
     *
     * @param dev Device context.
     * @param mode One of the BMP280_MODES_T values.
     */
    void bmp280_set_measure_mode(const bmp280_context dev,
                                 BMP280_MODES_T mode);

    /**
     * Return the value of the BMP280_REG_STATUS register.
     *
     * @param dev Device context.
     * @return Contents of the status register.
     */
    uint8_t bmp280_get_status(const bmp280_context dev);

    /**
     * Read a register.
     *
     * @param dev Device context.
     * @param reg The register to read
     * @return The value of the register
     */
    uint8_t bmp280_read_reg(const bmp280_context dev, uint8_t reg);

    /**
     * Read contiguous registers into a buffer.  This is a low level
     * function, and should not be used unless you know what you are
     * doing.
     *
     * @param dev Device context.
     * @param buffer The buffer to store the results
     * @param len The number of registers to read
     * @return The number of bytes read, or -1 on error
     */
    int bmp280_read_regs(const bmp280_context dev,
                         uint8_t reg, uint8_t *buffer, int len);

    /**
     * Write to a register.  This is a low level function, and should
     * not be used unless you know what you are doing.
     *
     * @param dev Device context.
     * @param reg The register to write to
     * @param val The value to write
     * @return UPM result.
     */
    upm_result_t bmp280_write_reg(const bmp280_context dev,
                                  uint8_t reg, uint8_t val);

    /**
     * SPI CS on and off functions
     */
    void _csOn(const bmp280_context dev);
    void _csOff(const bmp280_context dev);

    /**
     * Read the calibration data
     */
    upm_result_t _read_calibration_data(const bmp280_context dev);


#ifdef __cplusplus
}
#endif
