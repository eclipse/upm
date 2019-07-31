/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "rsc.h"
#include <string>
#include <interfaces/iPressure.hpp>
#include <interfaces/iTemperature.hpp>

namespace upm {
    /**
     * @brief Honeywell RSC TruStability Pressure and Temperature Sensors
     * @defgroup rsc libupm-rsc
     * @ingroup honeywell spi pressure temp
     */

    /**
     * @library rsc
     * @sensor rsc
     * @comname Honeywell RSC TruStability Pressure and Temperature Sensors
     * @altname Honeywell RSC TruStability Boardmount Pressure Sensors
     * @type pressure temp
     * @man honeywell
     * @con spi
     * @web https://sensing.honeywell.com/honeywell-sensing-trustability-rsc-series-data-sheet-32321348-b-en.pdf
     *
     * @brief Description for the RSC TruStability line
     *
     * The RSC Series is a piezoresistive silicon pressure sensor
     * offering a digital output for reading pressure over the specified
     * full scale pressure span and temperature range. It is calibrated
     * and temperature compensated for sensor offset, sensitivity,
     * temperature effects, and non-linearity using a 24-bit analog-todigital
     * converter with integrated EEPROM. Pressure data may
     * be acquired at rates between 20 and 2000 samples per second
     * over an SPI interface. It is intended for use with non-corrosive,
     * non-ionic gases, such as air and other dry gases, designed and
     * manufactured according to ISO 9001 standards, and is REACH
     * and RoHS compliant.
     *
     * Most of the functionality available on this chip has been implemented.
     * The driver calculates the compensated pressure and temperature values
     * based on the information provided in the datasheet. The user must not mess
     * around with the coefficient and adc init value readings from the EEPROM as
     * these are used to calculate the compensated pressure and temperature values.
     * 
     * In order to get the sensor to work properly on an Edison, you would need to
     * turn off the Edison SPI power management, this can be done with the command:
     * echo on > /sys/devices/pci0000\:00/0000\:00\:07.1/power/control
     *
     * You might have to use external pull up resistors on all the lines.
     * More info about this has been provided in the datasheet with a table and
     * a diagram.
     *
     * @snippet rsc.cxx Interesting
     */
    class RSC : virtual public iPressure, virtual public iTemperature {

    public:
        /**
         * @brief Default constructor for rsc
         *
         * This device supports SPI. The sensor has two chip select
         * pins since there are two ways to access content on the
         * sensor - EEPROM/ADC. We will not be using the SPI CS here
         * because that would work for all SPI operations. Hence, you are
         * expected to initialize any 2 gpio pins for CS by providing the
         * pin numbers.
         *
         * @param bus SPI bus to use.
         * @param cs_ee_pin The CS pin for accessing the EEPROM
         * @param cs_adc_pin The CS pin for accessing the ADC
         */
        RSC(uint8_t bus, uint8_t cs_ee_pin, uint8_t cs_adc_pin);

        /**
         * rsc destructor
         */
        ~RSC();

        /**
         * This function provides the sensor name as a string to
         * the user.
         * 
         * @return std::string sensor name
         */
        std::string getSensorName();

        /**
         * This function provides the sensor serial number as a 
         * string to the user.
         * 
         * @return std::string sensor serial number
         */
        std::string getSensorSerialNumber();

        /**
         * This function provides the pressure range which the 
         * sensor can read out. This will vary for the different
         * kinds of RSC sensors. This value is read from the EEPROM		 
         * 
         * @return float Total sensor pressure range
         */
        float getPressureRange();

        /**
         * This function provides the minimum pressure which the 
         * sensor can read out. This will vary for the different
         * kinds of RSC sensors. This value is read from the EEPROM		 
         * 
         * @return float Sensor minimum pressure
         */
        float getMinimumPressure();

        /**
         * This function gets the units that the particular RSC
         * sensor being used reads out the pressure data in.
         * Following are the possible units data can be read out
         * in:
         * 1. Pascal
         * 2. Kilo Pascal
         * 3. Mega Pascal
         * 4. PSI
         * 5. inH2O
         * 6. Bar
         * 7. milli Bar
         *
         * This info can be retrieved from the EEPROM
         *
         * @return std::string pressure units
         */
        std::string getPressureUnit();

        /**
         * Function returns the type of RSC pressure sensor being
         * used. This info is available in the EEPROM.
         * Types:
         * 1. Differential
         * 2. Absolute
         * 3. Gauge
         * 
         * @return std::string sensor type
         */
        std::string getPressureType();

        /**
         * Function returns the initial state that the ADC
         * registers need to be set to. This info is retrieved
         * from the EEPROM.
         * 
         * @return std::string sensor type
         */
        uint8_t *getInitialADCState();

        /**
         * Function used to update the coefficients. This function
         * doesn't return the coefficent values. Intstead, it updates
         * the 2-D array with the coefficient values read from the
         * EEPROM. This function is mostly for internal use only.
         */
        void updateCoefficients();

        /**
         * Function used to set up the ADC with the initial values
         * of the ADC registers read from the EEPROM.
         *
         * @param adc_init_values pointer to uint8_t array containing
         * the initial ADC values
         */
        void setupADC(uint8_t* adc_init_values);

        /**
         * Function to get the compensated temperature value.
         *
         * @return float compensated temperature value
         */
        virtual float getTemperature();

        /**
         * Function to get the compensated pressure value
         *
         * @return float compensated pressure value
         */
        virtual float getPressure();

        /**
         * Function to set the mode for the RSC sensor:
         * There are 2 types of modes available:
         *
         * 1. Normal Mode - 256 KHz
         * 2. Fast Mode - 512 KHz
         *
         * @return mode enum of type RSC_MODE
         */
        void setMode(RSC_MODE mode);

        /**
         * Function to set the data rate for the RSC sensor:
         * Following are the different data rates available:
         *
         * In Normal Mode
         * 1. 20 SPS
         * 2. 45 SPS
         * 3. 90 SPS
         * 4. 175 SPS
         * 5. 330 SPS
         * 6. 600 SPS
         * 7. 1000 SPS
         *
         * In Fast Mode
         * 1. 40 SPS
         * 2. 90 SPS
         * 3. 180 SPS
         * 4. 350 SPS
         * 5. 660 SPS
         * 6. 1200 SPS
         * 7. 2000 SPS
         *
         * SPS - Samples per Second
         *
         * @return dr enum of type RSC_DATA_RATE
         */
        void setDataRate(RSC_DATA_RATE dr);

    private:
        rsc_context m_rsc;
        RSC(const RSC& src) { /* do not create copied */}
        RSC& operator=(const RSC&) {return *this;}
        uint8_t m_adc_coeff[4];
    };
}
