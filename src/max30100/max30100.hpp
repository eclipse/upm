/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <iostream>

#include "mraa/i2c.h"
#include "max30100.h"

namespace upm {

/* Callback class for continuously reading samples */
class Callback {
    public:
        virtual ~Callback() { }
        /* Default run method, called for each new sample in continous
         * sampling mode.
         * Override this method */
        virtual void run(max30100_value samp)
        { std::cout << "Base sample IR: " << samp.IR << " R: " << samp.R << std::endl; }
};

/**
 * @brief MAX30100 Pulse Oximeter and Heart Rate Sensor
 * @defgroup max30100 libupm-max30100
 * @ingroup maxim i2c medical
 */

/**
 * @library max30100
 * @sensor max30100
 * @comname Pulse Oximeter and Heart-rate Sensor
 * @type medical
 * @man maxim
 * @web https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/MAX30100.html
 * @con i2c gpio
 *
 * @brief API for the Pulse oximeter and heart-rate sensor
 *
 * The MAX30100 is an integrated pulse oximetry and heartrate monitor sensor
 * solution. It combines two LEDs, a photodetector, optimized optics, and
 * low-noise analog signal processing to detect pulse oximetry and heart-rate
 * signals.
 *
 * I2C sensor which can be used to read:
 *    Heart-rate
 *    Peripheral capillary oxygen saturation
 *    temperature
 *
 * @image html max30100.png
 * @snippet max30100.cxx Interesting
 */

class MAX30100 {
    public:
        /**
         * Oximeter and heart-rate sensor constructor
         *
         * Initialize Oximeter and heart-rate sensor.  Note, the I2C address
         * is 0x57.
         * @param i2c_bus Target I2C bus
         * @return sensor context pointer
         * @throws std::runtime_error if sensor initialization fails
         */
        MAX30100(int16_t i2c_bus);

        /**
         * MAX30100 destructor
         */
        virtual ~MAX30100() {};

        /**
         * Sample a single set of infrared/red values
         *
         * Note, all setup (sample rate, LED current, and pulse width must be done
         * prior to calling this sample method.
         *
         * @return One IR/R sample
         * @throws std::runtime_error on I2C command failure
         */
        max30100_value sample();

        /**
         * Continuously sample Infrared/Red values.
         *
         * This method requires a GPIO pin which is used to signal
         * sample/samples ready.  The INT * pin is open-drain and requires a
         * pullup resistor. The interrupt pin is not designed  to sink large
         * currents, so the pull-up resistor value should be large, such as
         * 4.7k ohm.  The RCWL-0530 PCB which this library was designed with
         * had the I2C lines and INT pin pulled up to 1.8v.
         *
         * Note, all setup (sample rate, mode, LED current, and pulse width
         * must be done prior to calling this sample method.
         *
         * @param gpio_pin GPIO pin for interrupt (input from sensor INT pin)
         * @param buffered Enable buffered sampling.  In buffered sampling mode,
         * the device reads 16 samples at a time.  This can help with I2C read
         * timing.
         *      buffered == true, enable buffered sampling
         *      buffered == false, single-sample mode
         * @param cb Pointer to instance of Callback class.  If parameter is left
         * NULL, a default instance of the Callback class will be used which
         * prints out the IR/R values.
         * @throws std::runtime_error on I2C command failure
         */
        void sample_continuous(int gpio_pin, bool buffered, Callback *cb = NULL);

        /**
         * Stop continuous sampling.  Disable interrupts.
         */
        void sample_stop();

        /**
         * Read Oximeter and heart-rate sensor registers
         * @param reg Target register to read
         * @return Data returned from sensor
         * @throws std::runtime_error if I2C read command fails
         */
        uint8_t read(MAX30100_REG reg);

        /**
         * Write Oximeter and heart-rate sensor registers
         * @param reg Target register to write
         * @param wr_data Target data to write
         * @throws std::runtime_error if I2C write command fails
         */
        void write(MAX30100_REG reg, uint8_t wr_data);

        /**
         * Get sensor version
         * Sensor version is a 2 byte value:
         *      upper byte = PART ID
         *      lower byte = REVISION ID
         *
         * example:
         *      version() return 0x1105
         *      0x11 = PART ID
         *      0x05 = REVISION
         * @return Sensor version
         * @throws std::runtime_error on I2C command failure
         */
        uint16_t version();

        /**
         * Get temperature reading from device
         * @return rd_data Temperature in degrees Celsius
         * @throws std::runtime_error on I2C command failure
         */
        float temperature();

        /**
         * Set the sampling mode (none vs red only vs SpO2)
         *
         * @param mode Target sampling mode
         * @throws std::runtime_error on I2C command failure
         */
        void mode(MAX30100_MODE mode);

        /**
         * Get the sampling mode
         *
         * @return Current sampling mode
         * @throws std::runtime_error on I2C command failure
         */
        MAX30100_MODE mode();

        /**
         * Enable or disable high-resolution mode
         *
         * @param enable High-resolution enable
         *      true == SpO2 ADC resolution of 16 bit with 1.6ms LED pw
         * @throws std::runtime_error on I2C command failure
         */
        void high_res_enable(bool enable);

        /**
         * Get the high-resolution enable bit
         *
         * @return Current high-resolution bit value
         * @throws std::runtime_error on I2C command failure
         */
        bool high_res_enable();

        /**
         * Set the sample rate
         *
         * @param sample_rate Target sample rate
         * @throws std::runtime_error on I2C command failure
         */
        void sample_rate(MAX30100_SR sample_rate);

        /**
         * Get the sample rate
         *
         * @return Current sample rate
         * @throws std::runtime_error on I2C command failure
         */
        MAX30100_SR sample_rate();

        /**
         * Set the LED pulse width
         *
         * @param pulse_width Target LED pulse width
         * @throws std::runtime_error on I2C command failure
         */
        void pulse_width(MAX30100_LED_PW pulse_width);

        /**
         * Get the LED pulse width
         *
         * @return Current LED pulse width
         * @throws std::runtime_error on I2C command failure
         */
        MAX30100_LED_PW pulse_width();

        /**
         * Set the current for the infrared and red LEDs
         *
         * @param ir LED current enum
         * @param r LED current enum
         * @throws std::runtime_error on I2C command failure
         */
        void current(MAX30100_LED_CURRENT ir, MAX30100_LED_CURRENT r);

        /**
         * Get the infrared LED current
         *
         * @throws std::runtime_error on I2C command failure
         */
        MAX30100_LED_CURRENT current_ir();

        /**
         * Get the red LED current
         *
         * @throws std::runtime_error on I2C command failure
         */
        MAX30100_LED_CURRENT current_r();

        /**
         * Reset sensor
         *
         * When the RESET bit is set to one, all configuration, threshold,
         * and data registers are reset to their power-on-state. The only
         * exception is writing both RESET and TEMP_EN bits to one at the
         * same time since temperature data registers 0x16 and 0x17 are not
         * cleared. The RESET bit is cleared automatically back to zero after
         * the reset sequence is completed.
         *
         * @throws std::runtime_error on I2C command failure
         */
        void reset();

        /**
         * Put device into power-save mode.  While in power-save mode, all
         * registers retain their values, and write/read operations function
         * as normal. All interrupts are cleared to zero in this mode.
         *
         * @param sleep Enter/exit power-save mode
         * @throws std::runtime_error on I2C command failure
         */
        void sleep(bool sleep);

        /* Callback pointer available for a user-specified callback */
        Callback *_callback;
    private:
        /* base Callback instance to use if none provided */
        Callback _default_callback;

        /* device context struct */
        max30100_context* _dev;
};
}
