/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <otp538u.h>
#include <interfaces/iTemperature.hpp>

namespace upm {

    /**
     * @brief OTP538U IR Temperature Sensor
     * @defgroup otp538u libupm-otp538u
     * @ingroup seeed analog light hak
     */
    /**
     * @library otp538u
     * @sensor otp538u
     * @comname Analog IR Temperature Sensor
     * @altname Grove IR Temperature Sensor
     * @type light
     * @man generic
     * @con analog
     * @web http://wiki.seeed.cc/Grove-Infrared_Temperature_Sensor/
     * @kit hak
     *
     * @brief API for the OTP538U IR Temperature Sensor
     *
     * UPM module for the OTP538U IR temperature sensor
     *
     * This module was tested with the Grove IR non-contact temperature
     * sensor.
     *
     * The sensor provides 2 analog outputs: one for the thermistor
     * that measures the ambient temperature, and the other for the thermopile
     * that measures the object temperature.
     *
     * Much of the code depends on analyzing Seeed Studio* examples
     * and the circuit design. As a result, there are several 'magic'
     * numbers derived from their circuit design. These values are used
     * by default.
     *
     * The tables used came from the "538U VT
     * Table__20_200(v1.3).pdf" and "538RT_table.pdf" datasheets.
     *
     * These tables assume the object to be measured is 9 cm (3.54
     * inches) from the sensor.
     *
     * This sensor will not work at 3.3v on the Edsion or the Galileo 2.
     * It works fine on both systems at 5v.  It will work at 3.3v on the
     * Arduino 101 (tested via firmata subplatform on edison).
     *
     * @image html otp538u.jpg
     * @snippet otp538u.cxx Interesting
     */
    class OTP538U : virtual public iTemperature {
    public:
        /**
         * OTP538U constructor
         *
         * @param pinA Analog pin to use for the ambient temperature
         * @param pinO Analog pin to use for the object temperature
         * @param aref Analog reference voltage; default is 5.0 V
         */
        OTP538U(int pinA, int pinO, float aref = 5.0);

        /**
         * OTP538U destructor
         */
        ~OTP538U();

        /**
         * Gets the ambient temperature in Celsius
         *
         * @return Ambient temperature
         */
        float ambientTemperature();

        /**
         * Gets the ambient temperature in Celsius
         *
         * @return Ambient temperature
         */
        virtual float getTemperature();

        /**
         * Gets the object temperature in Celsius
         *
         * @return Object temperature
         */
        float objectTemperature();

        /**
         * Sets the offset voltage
         *
         * The Seeed Studio wiki gives an example of calibrating the sensor
         * and calculating the offset voltage to apply. Currently, the
         * default value is set, but you can use the function to set one
         * of your own.
         *
         * @param vOffset Desired offset voltage
         */
        void setVoltageOffset(float vOffset)
        {
            otp538u_set_voltage_offset(m_otp538u, vOffset);
        };

        /**
         * Sets the output resistance value
         *
         * The Seeed Studio wiki example uses a value of 2,000,000 in one of
         * the equations used to calculate voltage. The value is the
         * resistance of a resistor they use in the output stage of their
         * SIG2 output. This was 'decoded' by looking at the EAGLE* files
         * containing their schematics for this device.
         *
         * @param outResistance Value of the output resistor; default is 2M Ohm
         */
        void setOutputResistence(int outResistance)
        {
            otp538u_set_output_resistence(m_otp538u, outResistance);
        };

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
         * @param vref Reference voltage of the internal sensor; default
         * is 2.5
         */
        void setVRef(float vref)
        {
            otp538u_set_ivref(m_otp538u, vref);
        };

        /**
         * Enable debugging output.
         *
         * @param enable true to enable some debug output, false otherwise
         */
        void setDebug(bool enable)
        {
            otp538u_set_debug(m_otp538u, enable);
        };


    protected:
        otp538u_context m_otp538u;

    private:
        /* Disable implicit copy and assignment operators */
        OTP538U(const OTP538U&) = delete;
        OTP538U &operator=(const OTP538U&) = delete;
    };
}


