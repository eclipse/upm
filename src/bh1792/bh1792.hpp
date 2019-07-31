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
#include <string>
#include "bh1792.h"
#include <mraa/initio.hpp>

/**
 * @brief BH1792 Heart Rate Sensor
 * @defgroup bh1792 libupm-bh1792
 * @ingroup rohm i2c medical
 */
/**
 * @library bh1792
 * @sensor bh1792
 * @comname Heart Rate Sensor
 * @type medical
 * @man rohm
 * @con i2c
 *
 * @brief C++ API for the bh1792 driver
 *
 * @snippet bh1792.cxx Interesting
 */

namespace upm {
    class BH1792 {
    public:

        /**
         * @brief Init the sensor with specific bus and address. Before
         * starting measurement, one of (EnableSyncMode, EnableNonSyncMode,
         * or EnableSingleMode) has to be called.
         *
         * @param bus I2C bus number
         * @param addr I2C sensor address
         * @throws std::runtime_error on initialization failure
         */
        BH1792(int bus = 0, int addr = 0x5b);

        /**
         * Instantiates BH1792 Heart Rate Sensor based on a given string.
         *
         * @param initStr string containing specific information for BH1792 initialization.
         */
        BH1792(std::string initStr);


        /**
         * @brief Close and free sensor
         */
        virtual ~BH1792();

        /**
         * @brief Check "who am I" register value to identify the sensor
         *
         * @throws std::runtime_error if this value is incorrect
         */
        void CheckWhoAmI();

        /**
         * @brief Checks if the measuremnt on sensor is enabled
         *
         * @return True if measurement is enabled, or false otherwise
         */
        bool IsEnabled();

        /**
         * @brief Initiates a software reset to the sensor. All register values
         * will be written to their defaults, thus intended operation mode need
         * to be enabled.
         *
         * @throws std::runtime_error if software reset fails
         */
        void SoftReset();

        /**
         * @brief Gets the value of measurement time is milli-seconds
         *
         * @return measurement time float in milli-seconds
         * @throws std::runtime_error if reading measurement time fails
         */
        float GetMeasurementTimeMS();

        /**
         * @brief Sets the green LED Current value
         *
         * @param current The current value, accepted values are between 0-63
         * @throws std::runtime_error if setting current fails
         * @throws std::range_error if current value out of range
         */
        void SetGreenLedsCurrent(uint16_t current);

        /**
         * @brief Gets the green LED current value
         *
         * @return Green LED used current
         * @throws std::runtime_error if reading current fails
         */
        int GetGreenLedsCurrent();

        /**
         * @brief Sets the IR LED Current value
         *
         * @param current The current value, accepted values are between 0-63
         * @throws std::runtime_error if setting current fails
         * @throws std::range_error if current value out of range
         */
        void SetIrLedCurrent(uint16_t current);

        /**
         * @brief Gets the green LED current value
         *
         * @return Green LED used current
         * @throws std::runtime_error if reading current fails
         */
        int GetIrLedCurrent();

        /**
         * @brief Sets interrupt IR threshold value for non-synchronized mode
         *
         * @param threshold Value to be written, range 0-65536
         * @throws std::runtime_error if setting threshold fails
         */
        void SetIrThreshold(uint16_t threshold);

        /**
         * @brief Gets interrupt IR threshold value
         *
         * @return Green LED used current
         * @throws std::runtime_error if reading current fails
         */
        int GetIrThreshold();

        /**
         * @brief Gets the current size of built-in FIFO
         *
         * @return Number of entries store in FIFO
         * @throws std::runtime_error if reading size fails
         */
        int GetFifoSize();

        /**
         * @brief Disables interrupt function. Interrupt is enabled by default
         * on all operation modes, so this have to be called after enabling a
         * specific operation mode
         *
         * @throws std::runtime_error if setting interrupt disable bits fails
         */
        void DisableInterrupt();

        /**
         * @brief Clears the interrupt flag allowing more interrupts to be
         * raised
         *
         * @throws std::runtime_error if clearing interrupt fails
         */
        void ClearInterrupt();

        /**
         * @brief Starts the measuremnt on sensor or enable the sensor
         *
         * @throws std::runtime_error if starting measurement fails
         */
        void StartMeasurement();

        /**
         * @breif Stops the measurement by setting the software reset bit. This
         * function will reset all config registers. To resume from previous
         * configuration call RestartMeasurement().
         *
         * @throws std::runtime_error if disabling measurement fails
         */
        void StopMeasurement();

        /**
         * @brief Restart the measurement, This function stops measurement,
         * then re-writes the configuration registers again and start
         * measurement with same configuration.
         *
         * @throws std::runtime_error if restarting measurement fails
         */
        void RestartMeasurement();

        /**
         * @brief Gets the fifo data of 32 entries and resets the WATERMARK
         * interrupt flag.
         *
         * @return vector of values of the retrieved FIFO values, which are
         * vectors containing 2 values each (led off, led_on)
         * @throws std::runtime_error if reading FIFO data fails
         */
        std::vector<std::vector<int>> GetFifoData();

        /**
         * @brief Reads and discard the 32 FIFO entries to reset the WATERMARK
         * interrupt flag.
         *
         * @throws std::runtime_error if reading FIFO data fails
         */
        void DiscardFifoData();

        /**
         * @brief Gets the values from using green LED light
         *
         * @return vector of values (led off, led on) of green led values
         * @throws std::runtime_error if reading green data fails
         */
        std::vector<int> GetGreenData();

        /**
         * @brief Gets the values from using IR LED light
         *
         * @return vector of values (led off, led on) of IR led values
         * @throws std::runtime_error if reading IR data fails
         */
        std::vector<int> GetIrData();

        /**
         * @brief Enables and configures the sensor to use synchronized mode.
         * This will use the built-in FIFO and raises a WATERMARK interrupt
         * when FIFO size reaches 32 entries. To clear that interrupt flag FIFO
         * must be read or discarded. This mode uses only green LED.
         *
         * You need to call StartMeasurement() after this.
         *
         * @param measFreq Measurement frequncy mode, valid values 32, 64, 128,
         * 256, 1024
         * @param green_current Green LED current value
         * @throws std::runtime_error if enabling synchronized mode fails
         * @throws std::invalid_argument if measurement frequency is invalid
         */
        void EnableSyncMode(uint16_t measFreq, uint16_t green_current);

        /**
         * @brief Enabled and configures the sensor to use non-synchronized
         * mode. This mode uses IR_THRESHOLD interrupt, it will raise an
         * interrupt if IR LED ON value is equal or greater than the threshold
         * value. Interrupt must be cleared before receiving new interrupts.
         * This mode uses only IR LED.
         *
         * You need to call StartMeasurement() after this.
         *
         * @param ir_current The IR LED current value
         * @param threshold The IR LED interrupt threshold value
         *
         * @throws std::runtime_error if enabling non-synchronized mode fails
         */
        void EnableNonSyncMode(uint16_t ir_current, uint16_t threshold);

        /**
         * @brief Enables and configures the sensor to use single mode. This
         * sets the sensor to take only one measurement and raise an interrupt
         * after measurement is finished. Interrupt flags has to be cleared to
         * recieved new interrupts. This mode uses both green and IR LEDs.
         *
         * You need to call StartMeasurement() after this. If you need to get
         * continuous values, clear interrupt and StartMeasurement()
         * continuously.
         *
         * @param led_type LED light type to use, takes GREEN or IR (0 or 1)
         * @param current The chosen led_type current value
         * @throws std::runtime_error if enabling single mode fails
         */
        void EnableSingleMode(LED_TYPES led_type, uint16_t current);

        /**
         * @brief Installs the ISR to a given GPIO pin
         *
         * @param edge Edge type to raise ISR with, of type mraa_gpio_edge_t
         * @param pin GPIO pin number
         * @param isr Pointer to ISR function
         * @param isr_args Arguments to pass the ISR function
         * @throws std::runtime_error if installing ISR fails
         */
        void InstallISR(mraa_gpio_edge_t edge, int pin, void (*isr)(void *),
                        void *isr_args);

        /**
         * @brief Installs the ISR to a given GPIO pin
         *
         * @param pin GPIO pin number
         * @param isr Pointer to ISR function
         * @param isr_args Arguments to pass the ISR function
         * @throws std::runtime_error if installing ISR fails
         */
        void InstallISR(int pin, void (*isr)(void *), void *isr_args);

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
        bh1792_context m_bh1792;
        mraa::MraaIo mraaIo;

        /* Disable implicit copy and assignment operators */
        BH1792(const BH1792 &) = delete;
        BH1792 &operator=(const BH1792 &) = delete;
    };
}
