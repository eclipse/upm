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
#include <upm.h>
#include <mraa/uart.h>
#include <mraa/i2c.h>
#include <mraa/gpio.h>

#include "ecezo_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file ecezo.h
     * @library ecezo
     * @brief C API for the EC-EZO EC Sensor
     *
     * @include ecezo.c
     */

    /**
     * Device context
     */
    typedef struct _ecezo_context {
        mraa_uart_context        uart;
        mraa_i2c_context         i2c;

        // our values
        float                    ec;          // electrical conductivity
        float                    tds;         // total dissolved solids
        float                    salinity;
        float                    sg;          // specific gravity

    } *ecezo_context;

    /**
     * ECEZO Initializer for UART operation
     *
     * @param uart Specify which uart to use.
     * @param baudrate Specify the baudrate to use.  The device defaults
     * to 9600 baud, though the datasheet implies the default is 38400.
     * @return an initialized device context on success, NULL on error.
     */
    ecezo_context ecezo_uart_init(unsigned int uart, unsigned int baudrate);

    /**
     * ECEZO Initializer for I2C operation
     *
     * @param bus Specify which the I2C bus to use.
     * @param addr Specify the I2C address to use.  This is
     * configurable on the device, so there is no default.
     * @return an initialized device context on success, NULL on error.
     */
    ecezo_context ecezo_i2c_init(unsigned int bus, uint8_t addr);

    /**
     * ECEZO sensor close function
     */
    void ecezo_close(ecezo_context dev);

    /**
     * Query the device for a reading, parse the response, and store
     * the read values into the device context.  This function must be
     * called prior to calling any function that returns the data,
     * like ecezo_get_ec().
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t ecezo_update(const ecezo_context dev);

    /**
     * For accurate readings, the temperature of the liquid being
     * measured should be known. This function allows you to specify
     * the liquid's temperature (in Celsius) so that proper
     * compensation can take place.  How you measure this temperature
     * is up to you.  By default, the device will assume a temperature
     * of 25C.
     *
     * @param dev Device context
     * @param temp The temperature of the liquid being measured
     * @return UPM result
     */
    upm_result_t ecezo_set_temperature(const ecezo_context dev, float temp);

    /**
     * Set the K value of the probe being used.  By default, this is
     * 1.0. Valid values are between 0.1 and 10.0.
     *
     * @param dev Device context
     * @param k The K value of the probe
     * @return UPM result
     */
    upm_result_t ecezo_set_k_value(const ecezo_context dev, float k);

    /**
     * Enable or disable Sleep mode.
     *
     * @param dev Device context
     * @param enable True to enable sleep mode, false to wake up
     * @return UPM result
     */
    upm_result_t ecezo_set_sleep(const ecezo_context dev, bool enable);

    /**
     * Retrieve the last measured Electrical Conductivity (EC) value
     * in microsiemens.  ecezo_update() must have been called before
     * calling this function.
     *
     * @param dev Device context
     * @return EC value in microsiemens
     */
    float ecezo_get_ec(const ecezo_context dev);

    /**
     * Retrieve the last measured Total Dissolved solids (TDS) value.
     * ecezo_update() must have been called before calling this
     * function.
     *
     * @param dev Device context
     * @return TDS value
     */
    float ecezo_get_tds(const ecezo_context dev);

    /**
     * Retrieve the last measured Salinity value.  ecezo_update() must
     * have been called before calling this function.
     *
     * @param dev Device context
     * @return Salinity value
     */
    float ecezo_get_salinity(const ecezo_context dev);

    /**
     * Retrieve the last measured Specific Gravity (SG) value.
     * ecezo_update() must have been called before calling this
     * function.
     *
     * @param dev Device context
     * @return SG value
     */
    float ecezo_get_sg(const ecezo_context dev);

    /**
     * Specify calibration data for calibrating the device.  See the
     * datasheet for details on how calibration is performed.  This
     * function provides a mechanism for clearing out, and setting
     * calibration data.
     *
     * A simple one point calibration might work as follows:
     *
     * 1. CLEAR the calibration data
     * 2. with a dry probe, set the DRY point.
     * 3. with the probe immersed in a standardized solution, set the
     * ONE parameter to the solution's known EC value in microsiemens.
     *
     * A two point calibration might work as follows:
     *
     * 1. CLEAR the calibration data
     * 2. with a dry probe, set the DRY point.
     * 3. with the probe immersed in the lowest EC standardized
     * solution, set the LOW parameter to the solution's known EC
     * value in microsiemens.
     * 4. with the probe immersed in the highest EC standardized
     * solution, set the HIGH parameter to the solution's known EC
     * value in microsiemens.
     *
     * @param dev Device context
     * @param cal One of the ECEZO_CALIBRATION_T values
     * @param ec The EC value of the calibration fluid.  This
     * parameter is ignored when cal is either ECEZO_CALIBRATE_CLEAR
     * or ECEZO_CALIBRATE_DRY.
     * @return UPM result
     */
    upm_result_t ecezo_calibrate(const ecezo_context dev,
                                 ECEZO_CALIBRATION_T cal,
                                 float ec);

    /**
     * Enable or disable "continuous" operation.  In continuous
     * operation, the device will sample and emit readings every
     * second.  The driver disables this mode by default.  If you wish
     * to use continuous mode, you will be responsible for reading and
     * parsing the returned data yourself.
     *
     * The functionality of this driver depends on continuous mode
     * being disabled.  When disabled, the driver will manually
     * request a reading when desired via ecezo_update().
     *
     * @param dev Device context
     * @param enable true to enable continuous mode, false to disable.
     * @return UPM result
     */
    upm_result_t ecezo_set_continuous(const ecezo_context dev, bool enable);

    /**
     * Directly send a command to the device and optionally get a
     * response.  This is a low level function and should not be
     * called unless you know what you are doing.
     *
     * @param dev Device context
     * @param cmd command to send to the device.  See the datasheet
     * for valid commands.
     * @param buffer Optional buffer in which to return any data.
     * NULL if you are not interested in any returned data.
     * @param len Length of the buffer, or 0 if you are not interested
     * in returned data
     * @return Number of characters read back, 0 if a timeout or no
     * data, -1 if an error
     */
    int ecezo_send_command(const ecezo_context dev, char *cmd,
                           char *buffer, int len);

    /**
     * Read character data from the device.  This is a low level
     * function and should not be called unless you know what you are
     * doing.
     *
     * @param dev sensor context
     * @param buffer The character buffer to read data into.
     * @param len The maximum size of the buffer
     * @return The number of bytes successfully read, or -1 on error
     */
    int ecezo_read(const ecezo_context dev, char *buffer, size_t len);

    /**
     * Write character data to the device.  This is a low level
     * function and should not be called unless you know what you are
     * doing.
     *
     * @param dev sensor context
     * @param buffer The character buffer containing data to write.
     * @param len The number of bytes to write.
     * @return The number of bytes successfully written, or -1 on error.
     */
    upm_result_t ecezo_write(const ecezo_context dev, char *buffer, size_t len);


#ifdef __cplusplus
}
#endif
