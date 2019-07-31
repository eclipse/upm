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

#include <interfaces/iMoisture.hpp>
#include <interfaces/iTemperature.hpp>
#include "ims.h"

namespace upm {
/**
 * @brief Catnip Electronics I2C Moisture Sensor
 * @defgroup ims libupm-ims
 * @ingroup catnip i2c liquid light temp
 */

/**
 * @library ims
 * @sensor ims
 * @comname I2C Moisture Sensor
 * @type liquid light temp
 * @man catnip
 * @web https://www.tindie.com/products/miceuz/i2c-soil-moisture-sensor/
 * @con i2c
 *
 * @brief API for the Catnip Electronics I2C moisture sensor
 *
 * I2C sensor which can be used to read:
 *    moisture
 *    light
 *    temperature
 *
 *    This sensor must run at 100 kHz
 *
 * @image html ims.png
 * @snippet ims.cxx Interesting
 */

class IMS : virtual public iMoisture, virtual public iTemperature {
    public:
        /**
         * I2C Moisture Sensor constructor
         *
         * Initialize I2C Moisture Sensor
         * @param i2c_bus Target I2C bus
         * @param i2c_address Target I2C address (default is 0x20)
         * @return sensor context pointer
         * @throws std::runtime_error if sensor initializate fails
         */
        IMS(int16_t i2c_bus, int16_t i2c_address = IMS_ADDRESS_DEFAULT);

        /**
         * IMS destructor
         */
        virtual ~IMS() {};

        /**
         * Write I2C Moisture Sensor registers
         * @param cmd Write command
         * @param wr_data Target data to write (only used for IMS_SET_ADDRESS)
         * @throws std::runtime_error if I2C write command fails
         */
        void write(IMS_WR_COMMAND cmd, uint8_t wr_data);

        /**
         * Read I2C Moisture Sensor registers
         * @param cmd Read command
         * @return Data returned from sensor (1 or 2 bytes depending on CMD)
         * @throws std::runtime_error if I2C read command fails
         */
        uint16_t read(IMS_RD_COMMAND cmd);

        /**
         * Get sensor version
         * @return Sensor version
         * @throws std::runtime_error if I2C read command fails
         */
        uint16_t get_version();

        /**
         * Get moisture reading from sensor
         * @return Unitless, relative capacitance value (moisture)
         * @throws std::runtime_error if I2C read command fails
         */
        uint16_t get_moisture();

        /**
         * Get moisture reading from sensor
         * @return Unitless, relative capacitance value (moisture)
         * @throws std::runtime_error if I2C read command fails
         */
        virtual int getMoisture();

        /**
         * Get light reading from LED on device.  The technical data for the I2C
         * moisture sensor specifies a 3 second wait.  Loop for 3 seconds
         * checking the GET_BUSY register.  IF the sensor is NOT ready after 3
         * seconds, assume there is NO light and return a max uint16_t (dark)
         * value.
         * @return rd_data Unitless, relative value for brightness
         *        dark (0xFFFF) ---> light (0x0000)
         * @throws std::runtime_error if I2C write/read command fails
         */
        uint16_t get_light();

        /**
         * Get temperature reading from device
         * @return rd_data Temperature in degrees Celsius
         * @throws std::runtime_error if I2C read command fails
         */
        float get_temperature();

        /**
         * Get temperature reading from device
         * @return rd_data Temperature in degrees Celsius
         * @throws std::runtime_error if I2C read command fails
         */
        virtual float getTemperature();

        /**
         * Reset sensor
         * @throws std::runtime_error if I2C write command fails
         */
        void reset();

        /**
         * Set I2C address AND reset sensor
         * @param address_new New I2C for device
         * @throws std::runtime_error if I2C write command fails
         */
        void reset_i2c_address(uint8_t address_new);

        /**
         * Put device into low-power mode.  Device wakes on any I2C command.
         * @throws std::runtime_error if I2C write command fails
         */
        void sleep();
    private:
        /* device context struct */
        ims_context* _dev;
};
}
