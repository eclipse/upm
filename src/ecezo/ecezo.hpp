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

#include <string>
#include <iostream>

#include <stdlib.h>
#include <unistd.h>

#include "ecezo.h"
#include <interfaces/iEC.hpp>

namespace upm {
    /**
     * @brief EC-EZO Sensor
     * @defgroup ecezo libupm-ecezo
     * @ingroup uart i2c liquid
     */

    /**
     * @library ecezo
     * @sensor ecezo
     * @comname Electrical Conductivity (EC) Circuit
     * @type liquid
     * @man sparkfun
     * @con uart i2c
     * @web https://www.sparkfun.com/products/12908
     *
     * @brief API for the EC-EZO EC Sensor
     *
     * This driver was tested with the Atlas Scientific Electrical
     * Conductivity kit.
     *
     * This device can operate in either UART or I2C modes.
     *
     * @snippet ecezo.cxx Interesting
     */

    class ECEZO : virtual public iEC {
    public:

        /**
         * ECEZO object constructor.  This can use either UART
         * (default) or I2C communications.  For UART, specify the
         * uart number as the bus parameter, the baudrate as the
         * addrBaud parameter, and false for the isI2C parameter.
         *
         * For I2C, specify the bus parameter, the I2C address as the
         * addrBaud parameter, and true for the isI2C parameter.
         *
         * @param bus Specify which uart or I2C bus to use
         * @param addrBaud Specify the baudrate if using UART, or the
         * I2C address of the device if using I2C.
         * @param isI2C true if using I2C, false if using a UART
         */
        ECEZO(unsigned int bus=0, unsigned int addrBaud=9600,
              bool isI2C=false);

        /**
         * ECEZO object destructor
         */
        ~ECEZO();

        /**
         * Query the device for a reading, parse the response, and store
         * the read values into the device context.  This function must be
         * called prior to calling any function that returns the data,
         * like getEC().
         */
        void update();

        /**
         * For accurate readings, the temperature of the liquid being
         * measured should be known. This function allows you to specify
         * the liquid's temperature (in Celsius) so that proper
         * compensation can take place.  How you measure this temperature
         * is up to you.  By default, the device will assume a temperature
         * of 25C.
         *
         * @param temp The temperature of the liquid being measured
         * @return UPM result
         */
        void setTemperature(float temp);

        /**
         * Set the K value of the probe being used.  By default, this is
         * 1.0. Valid values are between 0.1 and 10.0.
         *
         * @param k The K value of the probe
          */
        void setKValue(float k);

        /**
         * Enable or disable Sleep mode.
         *
         * @param enable True to enable sleep mode, false to wake up
         */
        void setSleep(bool enable);

        /**
         * Retrieve the last measured Electrical Conductivity (EC)
         * value in microsiemens.  update() must have been called
         * before calling this function.
         *
         * @return EC value in microsiemens
         */
        float getEC();

        /**
         * Measures and retrieves the Electrical Conductivity (EC)
         * value in microsiemens. update() is called during this function.
         *
         * @return EC value in microsiemens
         */
        virtual float getECValue();

        /**
         * Retrieve the last measured Total Dissolved solids (TDS) value.
         * update() must have been called before calling this
         * function.
         *
         * @return TDS value
         */
        float getTDS();

        /**
         * Retrieve the last measured Salinity value.  update() must
         * have been called before calling this function.
         *
         * @return Salinity value
         */
        float getSalinity();

        /**
         * Retrieve the last measured Specific Gravity (SG) value.
         * update() must have been called before calling this
         * function.
         *
         * @return SG value
         */
        float getSG();

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
         * @param cal One of the ECEZO_CALIBRATION_T values
         * @param ec The EC value of the calibration fluid.  This
         * parameter is ignored when cal is either ECEZO_CALIBRATE_CLEAR
         * or ECEZO_CALIBRATE_DRY.
         */
        void calibrate(ECEZO_CALIBRATION_T cal, float ec);


    protected:
        // ecezo device context
        ecezo_context m_ecezo;

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
         * @param enable true to enable continuous mode, false to disable.
         */
        void setContinuous(bool enable);

        /**
         * Directly send a command to the device and optionally get a
         * response.  This is a low level function and should not be
         * called unless you know what you are doing.
         *
         * @param cmd a String containing the command to send to the
         * device.  See the datasheet for valid commands.
         * @return The string response, if any
         */
        std::string sendCommand(std::string cmd);

        /**
         * Read character data from the device.  This is a low level
         * function and should not be called unless you know what you are
         * doing.
         *
         * @return A string containing the data read back, if any
         */
        std::string read();

        /**
         * Write character data to the device.  This is a low level
         * function and should not be called unless you know what you are
         * doing.
         *
         * @param data The string containing data to write.
         */
        void write(std::string data);

    private:
        /* Disable implicit copy and assignment operators */
        ECEZO(const ECEZO&) = delete;
        ECEZO &operator=(const ECEZO&) = delete;
    };
}
