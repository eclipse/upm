/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/i2c.h>
#include <mraa/spi.h>
#include <mraa/gpio.h>

#include "ms5803_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file ms5803
     * @library ms5803
     * @brief C API for the MS5803 Pressure and Temperature sensor
     *
     * @include ms5803.c
     */

    /**
     * Device context
     */
    typedef struct _ms5803_context {
        mraa_i2c_context        i2c;
        mraa_spi_context        spi;
        // CS pin, if we are using one
        mraa_gpio_context       gpio;

        // whether we are doing I2C or SPI
        bool                    isSPI;

        // stored calibration coefficients
        uint16_t                C[MS5803_MAX_COEFFICIENTS];

        // the command sent to chip depending on OSR configuration for
        // temperature and pressure measurement.
        MS5803_CMD_T            temperatureCmd;
        MS5803_OSR_T            temperatureDelay;

        MS5803_CMD_T            pressureCmd;
        MS5803_OSR_T            pressureDelay;

        // compensated temperature in C
        float                   temperature;
        // compensated pressure in millibars
        float                   pressure;
    } *ms5803_context;

    /**
     * MS5803 initializer
     *
     * @param bus i2c/spi bus to use
     * @param address The address for this device if using I2C.  If
     * using SPI, supply -1 for this parameter.
     * @param cs_pin The GPIO pin to use for Chip Select (CS).  This is
     * only needed for SPI, and only if your SPI implementation
     * requires it.  Otherwise, just pass -1 if not using SPI, or your
     * CS is handled automatically by your SPI implementation.
     * @return an initialized device context on success, NULL on error.
     */
    ms5803_context ms5803_init(unsigned int bus, int address, int cs_pin);

    /**
     * MS5803 close
     *
     * @param dev Device context.
     */
    void ms5803_close(ms5803_context dev);

    /**
     * Reset the device.
     *
     * @param dev Device context.
     * @return UPM Status.
     */
    upm_result_t ms5803_reset(const ms5803_context dev);

    /**
     * Take measurements and store the current sensor values
     * internally.  This function must be called prior to retrieving
     * any sensor values, for example ms5803_get_temperature().
     *
     * @param dev Device context.
     */
    upm_result_t ms5803_update(const ms5803_context dev);

    /**
     * Set the output sampling resolution of the temperature
     * measurement.  Higher values provide a more precise value.  In
     * addition, more precise values require more time to measure.
     * The default set at device intialization is the highest
     * precision supported: MS5803_OSR_4096
     *
     * @param dev Device context.
     * @param osr One of the MS5803_OSR_T values.
     */
    void ms5803_set_temperature_osr(const ms5803_context dev,
                                    MS5803_OSR_T osr);

    /**
     * Set the output sampling resolution of the pressure
     * measurement.  Higher values provide a more precise value.  In
     * addition, more precise values require more time to measure.
     * The default set at device intialization is the highest
     * precision supported: MS5803_OSR_4096
     *
     * @param dev Device context.
     * @param osr One of the MS5803_OSR_T values.
     */
    void ms5803_set_pressure_osr(const ms5803_context dev,
                                 MS5803_OSR_T osr);

    /**
     * Return the latest measured temperature.  ms5803_update() must
     * have been called prior to calling this function.  The returned
     * value is in degrees Celsius.
     *
     * @param dev Device context.
     * @return Temperature in degrees C
     */
    float ms5803_get_temperature(const ms5803_context dev);

    /**
     * Return the latest measured pressure.  ms5803_update() must have
     * been called prior to calling this function.  The returned value
     * is in millibars.
     *
     * @param dev Device context.
     * @return Pressure in mbar
     */
    float ms5803_get_pressure(const ms5803_context dev);

    /**
     * Load a series of factory installed compensation coefficients.
     * This function is called during ms5803_init(), so it should
     * never need to be called again.  It is provided here anyway
     * "just in case".
     *
     * @param dev Device context.
     * @return UPM Status.
     */
    upm_result_t ms5803_load_coefficients(const ms5803_context dev);

    /**
     * Perform a bus read.  This function is bus agnostic.  It is
     * exposed here for those users wishing to perform their own low
     * level accesses.  This is a low level function, and should not
     * be used unless you know what you are doing.
     *
     * @param dev Device context
     * @param cmd The command to send.
     * @param data A pointer to a buffer in which data will be read into.
     * @param len The number of bytes to read.
     * @return UPM Status
     */
    upm_result_t ms5803_bus_read(const ms5803_context dev, uint8_t cmd,
                                 uint8_t *data, uint8_t len);

    /**
     * Perform a bus write.  This function is bus agnostic.  It is
     * exposed here for those users wishing to perform their own low
     * level accesses.  This is a low level function, and should not
     * be used unless you know what you are doing.
     *
     * @param dev Device context
     * @param cmd The command to send.
     * @param data A pointer to a buffer containing data to write.
     * @param len The number of bytes to write.
     * @return UPM Status
     */
    upm_result_t ms5803_bus_write(const ms5803_context dev, uint8_t cmd,
                                  uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif
