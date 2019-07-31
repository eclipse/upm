/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

#pragma once

#include <vector>
#include "bh1749.h"
#include <mraa/initio.hpp>

/**
 * @brief BH1749 Color Sensor
 * @defgroup bh1749 libupm-bh1749
 * @ingroup rohm i2c color
 */

/**
 * @library bh1749
 * @sensor bh1749
 * @comname Color Sensor
 * @type color sensor
 * @man rohm
 * @con i2c
 *
 * @brief C++ API wrapper for the bh1749 driver
 *
 * @snippet bh1749.cxx Interesting
 */

namespace upm {
    class BH1749 {
    public:
        /**
         * @brief Initialize a sensor instance with bus and address.
         *
         * This calls SensorInit() to set default values for operating mode,
         * gains, measurement time, interrupt source and then sets threshold
         * high to 511.
         *
         * @param bus I2C bus number
         * @param addr I2C sensor address
         * @throws std::runtime_error on initialization failure
         */
        BH1749(int bus = 0, int addr = 0x39);

        /**
         * Instantiates BH1749 Color Sensor based on a given string.
         *
         * @param initStr string containing specific information for BH1749 Sensor initialization.
         */
        BH1749(std::string initStr);

        /**
         * @brief Close and free sensor
         */
        virtual ~BH1749();

        /**
         * @brief Enables RGB color measurement on the sensor
         *
         * @throws std::runtime_error if bh1749_enable() fails
         */
        void Enable();

        /**
         * @brief Disables RGB color measurement on the sensor
         *
         * @throws std::runtime_error if bh1749_disable() fails
         */
        void Disable();

        /**
         * @brief Check "who am I" register value to identify the sensor
         *
         * @throws std::runtime_error if this value is incorrect
         */
        void CheckWhoAmI();

        /**
         * @brief Initializes (writes) configuration values to sensor
         *
         * @param opMode Operating mode choice, a value of OPERATING_MODES enum
         * @param measTime Measurement time choice, a value of MEAS_TIMES enum
         * @param rgbGain RGB gain choice, a value of RGB_GAINS enum
         * @param irGain IR gain choice, a value of IR_GAINS enum
         * @param intSource interrupt source choice, a value of INT_SOURCES enum
         *
         * @throws std::runtime_error if one of values setting fails
         */
        void SensorInit(OPERATING_MODES opMode,
                        MEAS_TIMES measTime,
                        RGB_GAINS rgbGain,
                        IR_GAINS irGain,
                        INT_SOURCES intSource);

        /**
         * @brief Sets operating mode (interrupt persistance)
         *
         * @param opMode Operating mode choice, a value of OPERATING_MODES enum
         * @throws std::runtime_error if writing value fails
         */
        void SetOperatingMode(OPERATING_MODES opMode);

        /**
         * @brief Gets operating mode (interrupt persistance) value
         *
         * @param opMode pointer (uint8_t) to save value
         * @return operating mode value
         * @throws std::runtime_error if reading the value fails
         */
        int GetOperatingMode();

        /**
         * @brief Sets measurement time (ODR)
         *
         * @param measTime measurement time choice, a value of MEAS_TIMES enum
         * @throws std::runtime_error if writing value fails
         */
        void SetMeasurementTime(MEAS_TIMES measTime);

        /**
         * @brief Gets measurement time (ODR) value
         *
         * @return measurement time value
         * @throws std::runtime_error if reading the value fails
         */
        int GetMeasurementTime();

        /**
         * @brief Sets RGB gain value
         *
         * @param rgbGain RGB gain choice, a value of RGB_GAINS enum
         * @throws std::runtime_error if writing value fails
         */
        void SetRgbGain(RGB_GAINS rgbGain);

        /**
         * @brief Gets RGB gain value
         *
         * @return RGB gain value value
         * @throws std::runtime_error if reading the value fails
         */
        int GetRgbGain();

        /**
         * @brief Sets IR gain value
         *
         * @param irGain IR gain choice, a value of IR_GAINS enum
         * @throws std::runtime_error if writing value fails
         */
        void SetIrGain(IR_GAINS irGain);

        /**
         * @brief Gets IR gain value
         *
         * @return IR gain value value
         * @throws std::runtime_error if reading the value fails
         */
        int GetIrGain();

        /**
         * @brief Sets interrupt source value
         *
         * @param intSource interrupt source choice, a value of INT_SOURCES enum
         * @throws std::runtime_error if writing value fails
         */
        void SetIntSource(INT_SOURCES intSource);

        /**
         * @brief Gets interrupt source value
         *
         * @return interrupt source value
         * @throws std::runtime_error if reading the value fails
         */
        char GetInterruptSourceChar();

        /**
         * @brief Enables interrupt mode
         *
         * @throws std::runtime_error if writing enable bit fails
         */
        void EnableInterrupt();

        /**
         * @brief Disables interrupt mode
         *
         * @throws std::runtime_error if writing disable bit fails
         */
        void DisableInterrupt();

        /**
         * @brief Resets interrupt status (clear) to allow new interrupt reads
         *
         * @throws std::runtime_error if writing reset bit fails
         */
        void ResetInterrupt();

        /**
         * @brief Gets interrupt status, whether interrupt is raised or not
         *
         * @return true if interrupt is raised, otherwise false
         */
        bool IsInterrupted();

        /**
         * @brief Gets interrupt mode status
         *
         * @return true if interrupt is enabled, otherwise false
         */
        bool IsInterruptEnabled();

        /**
         * @brief Initiates a software reset to the sensor. All register values
         * will be written to their defaults, thus sensor_init() must be called
         * after this, and thresholds also needs to be set.
         *
         * @throws std::runtime_error if writing reset bit fails
         */
        void SoftReset();

        /**
         * @brief Sets threshold high value
         *
         * @param threshold interrupt threshold high value, range 0-65536
         * @throws std::runtime_error if writing value fails
         */
        void SetThresholdHigh(uint16_t threshold);

        /**
         * @brief Gets threshold high value
         *
         * @return threshold high value
         * @throws std::runtime_error if reading the value fails
         */
        int GetThresholdHigh();

        /**
         * @brief Sets threshold low value
         *
         * @param threshold interrupt threshold low value, range 0-65536
         * @throws std::runtime_error if writing value fails
         */
        void SetThresholdLow(uint16_t threshold);

        /**
         * @brief Gets threshold low value
         *
         * @return threshold low value
         * @throws std::runtime_error if reading the value fails
         */
        int GetThresholdLow();

        /**
         * @brief Gets value of Red color channel
         *
         * @return red channel value
         * @throws std::runtime_error if reading the value fails
         */
        uint16_t GetRed();

        /**
         * @brief Gets value of Green color channel
         *
         * @return green channel value
         * @throws std::runtime_error if reading the value fails
         */
        uint16_t GetGreen();

        /**
         * @brief Gets value of Blue color channel
         *
         * @return blue channel value
         * @throws std::runtime_error if reading the value fails
         */
        uint16_t GetBlue();

        /**
         * @brief Gets value of IR color channel
         *
         * @return ir channel value
         * @throws std::runtime_error if reading the value fails
         */
        uint16_t GetIr();

        /**
         * @brief Gets value of Green2 color channel
         *
         * @return green2 channel value
         * @throws std::runtime_error if reading the value fails
         */
        uint16_t GetGeen2();

        /**
         * @brief Gets all channels measurements values
         *
         * @return vector with all channels values ordered as:
         * Red, Green, Blue, IR, Green2
         * @throws std::runtime_error if reading one of the values fails
         */
        std::vector<uint16_t> GetMeasurements();

        /**
         * @brief Installs the ISR to a given GPIO pin
         *
         * @param edge Edge type to raise ISR with, of type mraa_gpio_edge_t
         * @param pin GPIO pin number
         * @param isr Pointer to ISR function
         * @param isr_args Arguments to pass the ISR function
         * @throws std::runtime_error if installing ISR fails
         */
        void InstallISR(mraa_gpio_edge_t edge, int pin, void (*isr)(void *), void *isr_args);

        /**
         * @brief Removes the ISR if it is installed
         */
        void RemoveISR();

        /**
         * @brief Gets a dump of configuration registers as a string
         *
         * @return string of dumped registers
         * @throws std::runtime_error if reading one of the registers fails
         */
        std::string RegistersDump();

    private:
        bh1749_context m_bh1749;
        mraa::MraaIo mraaIo;

        /* Disable implicit copy and assignment operators */
        BH1749(const BH1749 &) = delete;
        BH1749 &operator=(const BH1749 &) = delete;
    };
}
