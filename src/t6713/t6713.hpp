/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


#include "mraa/i2c.hpp"

#include <interfaces/iGas.hpp>

namespace upm {
/**
 * @brief Amphenol Telaire 6713 Series CO2 Module
 * @defgroup t6713 libupm-t6713
 * @ingroup i2c amphenol gaseous ico2sensor
 */

/**
 * @library t6713
 * @sensor t6713
 * @comname I2C/UART High Accuracy CO2 Sensor
 * @altname T6713
 * @type gaseous
 * @man amphenol
 * @web http://amphenol-sensors.com/en/products/co2/co2-modules/3215-t6700
 * @con i2c
 * @if ico2sensor
 *
 * @brief C++ API for Amphenol Telaire 6713 Series CO2 Module
 *
 * The Telaire 6713 Series CO2 Module
 * [6713](http://amphenol-sensors.com/en/products/co2/co2-modules/3215-t6700#specifications-t6713)
 * is a CO2 Module sensor.
 *
 * @snippet t6713.cxx Interesting
 * @snippet t6713-co2-sensor.cxx Interesting
 */

#define UPM_THROW(msg) throw std::runtime_error(std::string(__FUNCTION__) + ": " + (msg))

namespace t6713_co2
{
    /* COMMAND VALUES */
    typedef enum
    {
        T6713_COMMAND_RESET,
        T6713_COMMAND_STATUS,
        T6713_COMMAND_GET_FIRMWARE_REVISION,
        T6713_COMMAND_GET_GAS_PPM
    }MODBUS_COMMANDS;

    typedef enum
    {
        ERROR_CONDITION,
        FLASH_ERROR,
        CALIBRATION_ERROR,
        WARMUP_MODE,
        SINGLE_POINT_CALIBRATION,
        RS232,
        I2C,
        RS485
    }STATUS;

    typedef struct
    {
        uint8_t function_code;
        uint8_t register_address_msb;
        uint8_t register_address_lsb;
        uint8_t input_registers_to_read_msb;
        uint8_t input_registers_to_read_lsb;

    }COMMAND;

    typedef struct
    {
        uint8_t function_code;
        uint8_t byte_count;
        uint8_t status_msb;
        uint8_t status_lsb;

    }RESPONSE;

    typedef enum
    {
        READ_INPUT_REGISTERS = 4,
        WRITE_SINGLE_COIL,
        WRITE_SINGLE_HOLDING_REGISTER
    }FUNCTION_CODES;
}//namespace t6713_co2

class T6713: virtual public iGas {
    public:
        /**
        * Instantiates a T6713 object
        *
        * @param bus number of used bus
        */
        T6713 (int bus);

        /**
        * T6713 object destructor.
        */
        virtual ~T6713 () {};

        /**
        * Returns sensor module name
        */
        virtual const char* getModuleName() { return "t6713"; }

        /**
        * Get relative humidity measurement.
        */
        uint16_t getPpm ();

        /**
        * Get relative humidity measurement.
        */
        float getConcentration ();

        /**
        * Get the firmware version
        */
        uint16_t getFirmwareRevision();


    private:
        mraa::Result runCommand(t6713_co2::MODBUS_COMMANDS command);
        uint16_t getSensorData(t6713_co2::MODBUS_COMMANDS cmd);
        t6713_co2::STATUS getStatus();

        mraa::I2c i2c;
        mraa::Result status;
};

}//namespace upm
