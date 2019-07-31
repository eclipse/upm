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

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>
#include <mraa/aio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file otp538u.h
     * @library otp538u
     * @brief C API for the OTP538U IR Temperature Sensor
     *
     * @include otp538u.c
     */

    /**
     * device context
     */
    typedef struct _otp538u_context
    {
        mraa_aio_context aioA;
        mraa_aio_context aioO;

        bool             debug;
        float            internalVRef;
        float            aref;
        int              vResistance;
        float            offsetVoltage;
        int              adcResolution;
    } *otp538u_context;


    /**
     * OTP538U constructor
     *
     * @param pinA Analog pin to use for the ambient temperature
     * @param pinO Analog pin to use for the object temperature
     * @param aref Analog reference voltage
     * @return intialized context, or NULL if error
     */
    otp538u_context otp538u_init(int pinA, int pinO, float aref);

    /**
     * OTP538U destructor
     *
     * @param dev Device context
     */
    void otp538u_close(otp538u_context dev);

    /**
     * Gets the ambient temperature in Celsius
     *
     * @param dev Device context
     * @param temp Ambient temperature
     * @return UPM status
     */
    upm_result_t otp538u_get_ambient_temperature(const otp538u_context dev,
                                                 float *temperature);

    /**
     * Gets the object temperature in Celsius
     *
     * @param dev Device context
     * @param temp Object temperature
     * @return UPM status
     */
    upm_result_t otp538u_get_object_temperature(const otp538u_context dev,
                                                float *temperature);

    /**
     * Sets the offset voltage
     *
     * The Seeed Studio wiki gives an example of calibrating the sensor
     * and calculating the offset voltage to apply. Currently, the
     * default value is set, but you can use the function to set one
     * of your own.
     *
     * @param dev Device context
     * @param vOffset Desired offset voltage
     */
    void otp538u_set_voltage_offset(const otp538u_context dev, float offset);

    /**
     * Sets the output resistance value
     *
     * The Seeed Studio wiki example uses a value of 2,000,000 in one of
     * the equations used to calculate voltage. The value is the
     * resistance of a resistor they use in the output stage of their
     * SIG2 output. This was 'decoded' by looking at the EAGLE* files
     * containing their schematics for this device.
     *
     * @param dev Device context
     * @param outResistance Value of the output resistor; default is 2M Ohm
     */
    void otp538u_set_output_resistence(const otp538u_context dev,
                                       int resistance);

    /**
     * Sets the reference voltage of the internal Seeed Studio voltage
     * regulator on the sensor board.
     *
     * The Seeed Studio wiki example uses a value of 2.5 in one of the
     * equations used to calculate the resistance of the ambient
     * thermistor. The value is the voltage of an internal voltage
     * regulator used on the sensor board. This was 'decoded' by
     * looking at the EAGLE files containing their schematics for this
     * device.
     *
     * @param dev Device context
     * @param vref Reference voltage of the internal sensor; default
     * is 2.5
     */
    void otp538u_set_ivref(const otp538u_context dev, float vref);

    /**
     * Enable debugging output (linux platforms only).
     *
     * @param dev Device context
     * @param enable true to enable some debug output, false otherwise
     */
    void otp538u_set_debug(const otp538u_context dev, bool enable);


#ifdef __cplusplus
}
#endif
