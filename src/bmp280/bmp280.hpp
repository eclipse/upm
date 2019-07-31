/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include "bmp280.h"
#include "mraa/initio.hpp"

#include <interfaces/iPressure.hpp>
#include <interfaces/iTemperature.hpp>

namespace upm {

    /**
     * @brief BMP280/BME280 Digital Pressure Sensors
     * @defgroup bmp280 libupm-bmp280
     * @ingroup i2c spi gpio pressure bosch adafruit
     */

    /**
     * @library bmp280
     * @sensor bmp280
     * @comname Digital Pressure Sensor
     * @type pressure
     * @man bosch adafruit
     * @con i2c spi gpio
     * @web https://www.adafruit.com/products/2651
     *
     * @brief API for the BMP280 Digital Pressure Sensor
     *
     * The BMP280 is an absolute barometric pressure sensor especially
     * designed for mobile applications. The sensor module is housed in
     * an extremely compact 8-pin metal-lid LGA package with a footprint
     * of only 2.0 * 2.5 mm2 and 0.95 mm package height. Its small
     * dimensions and its low power consumption of 2.7 uA @1Hz allow the
     * implementation in battery driven devices such as mobile phones,
     * GPS modules or watches.
     *
     * As the successor to the widely adopted BMP180, the BMP280
     * delivers high performance in all applications that require
     * precise pressure measurement. The BMP280 operates at lower noise,
     * supports new filter modes and an SPI interface within a footprint
     * 63% smaller than the BMP180.
     *
     * @snippet bmp280.cxx Interesting
     */

    class BMP280 : virtual public iPressure, virtual public iTemperature {
    public:

        /**
         * BMP280 constructor.
         *
         * This driver supports both the BMP280 and the BME280.  The
         * BME280 adds a humidity sensor.  The device type is detected
         * automatically by querying the chip id register.
         *
         * This device can support both I2C and SPI. For SPI, set the addr
         * to -1, and specify a positive integer representing the Chip
         * Select (CS) pin for the cs argument.  If you are using a
         * hardware CS pin, then you can connect the proper pin to the
         * hardware CS pin on your MCU and supply -1 for cs.  The default
         * operating mode is I2C.
         *
         * @param bus I2C or SPI bus to use.
         * @param addr The I2C address for this device.  Use -1 for SPI.
         * @param cs The gpio pin to use for the SPI Chip Select.  Use -1
         * for I2C, or for SPI with a hardware controlled pin.
         * @throws std::runtime_error on failure.
         */
        BMP280(int bus=BMP280_DEFAULT_I2C_BUS, int addr=BMP280_DEFAULT_ADDR,
               int cs=-1);

        /**
         * Instantiates BMP280/BME280 Digital Pressure Sensors based on a given string.
         *
         * @param initStr string containing specific information for BMP280/BME280 initialization.
         */
        BMP280(std::string initStr);

        /**
         * BMP280 Destructor.
         */
        virtual ~BMP280();

        /**
         * Update the internal stored values from sensor data.
         *
         * @throws std::runtime_error on failure.
         */
        virtual void update();

        /**
         * Return the chip ID.
         *
         * @return The chip ID (BMP280_CHIPID).
         */
        uint8_t getChipID();

        /**
         * Reset the sensor, as if by a power-on-reset.
         */
        void reset();

        /**
         * Return the current measured temperature.  Note, this is not
         * ambient temperature - this is the temperature used to fine tune
         * the pressure measurement.  update() must have been called prior
         * to calling this method.
         *
         * @param fahrenheit true to return data in Fahrenheit, false for
         * Celicus.  Celsius is the default.
         * @return The temperature in degrees Celsius or Fahrenheit.
         */
        float getTemperature(bool fahrenheit);

        /**
         * Return the current measured temperature.  Note, this is not
         * ambient temperature - this is the temperature used to fine tune
         * the pressure measurement.  update() must have been called prior
         * to calling this method.
         *
         * @return The temperature in degrees Celsius.
         */
        virtual float getTemperature();

        /**
         * Return the current measured pressure in Pascals (Pa).  update()
         * must have been called prior to calling this method.
         *
         * @return The pressure in Pascals (Pa).
         */
        virtual float getPressure();

        /**
         * Set the pressure at sea level in hecto-Pascals (hPA).  This
         * value is used to compute the altitude based on the
         * pressure.  At driver initialization time, this value is set
         * to 1013.25 hPA.
         *
         * @param seaLevelhPA The pressure at sea level in hectoPascals
         * (hPa).  The default is 1013.25 hPA, (101325 Pa).
         * @return The computed altitude in meters.
         */
        void setSeaLevelPreassure(float seaLevelhPA=1013.25);

        /**
         * Return the current computed altitude in meters.  update()
         * must have been called prior to calling this method.
         *
         * @return The computed altitude in meters.
         */
        float getAltitude();

        /**
         * Set a general usage mode.  This function can be used to
         * configure the filters and oversampling for a particular use
         * case.  These setting are documented in the BMP280 datasheet.
         * The default mode set in the constructor is
         * USAGE_MODE_INDOOR_NAV, the highest resolution mode.
         *
         * @param mode One of the BMP280_USAGE_MODE_T values.
         */
        virtual void setUsageMode(BMP280_USAGE_MODE_T mode);

        /**
         * Set the temperature sensor oversampling parameter.  See the
         * data sheet for details.  This value can be automatically set to
         * a suitable value by using one of the predefined modes for
         * setUsageMode().
         *
         * @param rate One of the BMP280_OSRS_T_T values.
         */
        void setOversampleRateTemperature(BMP280_OSRS_T_T rate);

        /**
         * Set the pressure sensor oversampling parameter.  See the
         * data sheet for details.  This value can be automatically set to
         * a suitable value by using one of the predefined modes for
         * setUsageMode().
         *
         * @param rate One of the BMP280_OSRS_P_T values.
         */
        void setOversampleRatePressure(BMP280_OSRS_P_T rate);

        /**
         * Set the timer standby value.  When in NORMAL operating mode,
         * this timer governs how long the chip will wait before
         * performing a measurement.  See the data sheet for details.
         *
         * @param tsb One of the BMP280_T_SB_T values.
         */
        void setTimerStandby(BMP280_T_SB_T tsb);

        /**
         * Set the IIR filtering parameter.  See the data sheet for
         * details.  This value can be automatically set to a suitable
         * value by using one of the predefined modes for setUsageMode().
         *
         * @param filter One of the BMP280_FILTER_T values.
         */
        void setFilter(BMP280_FILTER_T filter);

        /**
         * Set the default measuring mode.  Basic values are forced,
         * sleep, and normal.  See the data sheet for details.  This value
         * can be automatically set to a suitable value by using one of
         * the predefined modes for setUsageMode().
         *
         * @param mode One of the BMP280_MODES_T values.
         */
        void setMeasureMode(BMP280_MODES_T mode);


        // Interface support
        const char *getModuleName()
        {
            return "BMP280";
        };

        int getTemperatureCelsius()
        {
            return int(getTemperature(false));
        };

        int getPressurePa()
        {
            return int(getPressure());
        };

    protected:
        bmp280_context m_bmp280;
        mraa::MraaIo mraaIo;

        /**
         * Return the value of the BMP280_REG_STATUS register.
         *
         * @return Contents of the status register.
         */
        uint8_t getStatus();

        /**
         * Read a register.
         *
         * @param reg The register to read
         * @return The value of the register
         */
        uint8_t readReg(uint8_t reg);

        /**
         * Read contiguous registers into a buffer.
         *
         * @param buffer The buffer to store the results
         * @param len The number of registers to read
         * @return The number of bytes read, or -1 on error
         */
        int readRegs(uint8_t reg, uint8_t *buffer, int len);

        /**
         * Write to a register
         *
         * @param reg The register to write to
         * @param val The value to write
         * @throws std::runtime_error on failure.
         */
        void writeReg(uint8_t reg, uint8_t val);

    private:
        /* Disable implicit copy and assignment operators */
        BMP280(const BMP280&) = delete;
        BMP280 &operator=(const BMP280&) = delete;
    };
}
