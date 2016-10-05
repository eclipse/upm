/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <mraa/i2c.hpp>
#include <mraa/spi.hpp>
#include <mraa/gpio.hpp>

#include "upm/iHumiditySensor.hpp"

#include "bmp280.hpp"

#define BME280_DEFAULT_I2C_BUS 0
#define BME280_DEFAULT_SPI_BUS 0
#define BME280_DEFAULT_ADDR 0x77
#define BME280_DEFAULT_CHIPID 0x60

namespace upm {

  /**
   * @library bmp280
   * @sensor bme280
   * @comname BME280 Digital Humidity, Pressure, and Temperature Sensor
   * @type pressure humidity
   * @man adafruit
   * @con i2c spi gpio
   * @web https://www.adafruit.com/products/2652
   *
   * @brief API for the BME280 Digital Humidity, Pressure, and
   * Temperature Sensor
   *
   * The BME280 is as combined digital humidity, pressure and
   * temperature sensor based on proven sensing principles. The sensor
   * module is housed in an extremely compact metal-lid LGA package
   * with a footprint of only 2.5 * 2.5 mm2 with a height of 0.93
   * mm. Its small dimensions and its low power consumption allow the
   * implementation in battery driven devices such as handsets, GPS
   * modules or watches. The BME280 is register and performance
   * compatible to the Bosch Sensortec BMP280 digital pressure sensor
   *
   * @snippet bme280.cxx Interesting
   */

  class BME280 : public BMP280, public IHumiditySensor {
  public:

    /**
     * BMP280 registers
     */
    typedef enum : uint8_t {
      // Do not write into reserved bits.

      // We only specify those registers specific to the BME280.  The
      // rest of them can be found in the BMP280 header file.

      // read-only factory calibration data for humidity
      
      REG_CALIB_DIG_H1                 = 0xa1,

      REG_CALIB_DIG_H2_LSB             = 0xe1,
      REG_CALIB_DIG_H2_MSB             = 0xe2,

      REG_CALIB_DIG_H3                 = 0xe3,

      REG_CALIB_DIG_H4_0               = 0xe4, // bits 8 -> 11:4
      REG_CALIB_DIG_H4_1               = 0xe5, // bits 3:0 -> 3:0

      REG_CALIB_DIG_H5_0               = 0xe5, // bits 7:4 -> 3:0
      REG_CALIB_DIG_H5_1               = 0xe6, // bits 8 -> 11:4

      REG_CALIB_DIG_H6                 = 0xe7,

      REG_CTRL_HUM                     = 0xf2,

      REG_HUMIDITY_MSB                 = 0xfd,
      REG_HUMIDITY_LSB                 = 0xfe
    } BME280_REGS_T;

    /**
     * REG_CTRL_HUM bits
     */
    typedef enum {
      CTRL_HUM_OSRS_H0                 = 0x01,
      CTRL_HUM_OSRS_H1                 = 0x02,
      CTRL_HUM_OSRS_H2                 = 0x04,
      _CTRL_HUM_OSRS_H_MASK            = 3,
      _CTRL_HUM_OSRS_H_SHIFT           = 0

      // 0x08-0x80 reserved
    } CTRL_HUM_T;

    /**
     * CTRL_HUM_OSRS_H values
     */
    typedef enum {
      OSRS_H_SKIPPED                   = 0,
      OSRS_H_OVERSAMPLING_1            = 1, // x1
      OSRS_H_OVERSAMPLING_2            = 2, // x2
      OSRS_H_OVERSAMPLING_4            = 3,
      OSRS_H_OVERSAMPLING_8            = 4,
      OSRS_H_OVERSAMPLING_16           = 5
    } OSRS_H_T;


    /**
     * BME280 constructor.
     *
     * This device can support both I2C and SPI. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument.  If you are using a
     * hardware CS pin, then you can connect the proper pin to the
     * hardware CS pin on your MCU and supply -1 for cs.  The default
     * operating mode is I2C.
     *
     * @param bus I2C or SPI bus to use.
     * @param address The address for this device.  -1 for SPI.
     * @param cs The gpio pin to use for the SPI Chip Select.  -1 for
     * I2C or for SPI with a hardware controlled pin.
     * @param theChipID The chip ID used for validation
     */
    BME280(int bus=BME280_DEFAULT_I2C_BUS, int addr=BME280_DEFAULT_ADDR,
           int cs=-1, uint8_t theChipID=BME280_DEFAULT_CHIPID);

    /**
     * BME280 Destructor.
     */
    virtual ~BME280();

    /**
     * Update the internal stored values from sensor data.
     */
    virtual void update();

    /**
     * Return the current measured relative humidity.  update() must
     * have been called prior to calling this method.
     *
     * @return The relative humidity in percent..
     */
    float getHumidity();

    /**
     * Set a general usage mode.  This function can be used to
     * configure the filters and oversampling for a particular use
     * case.  These settings are documented in the BMP280 and BME280
     * datasheets.
     * 
     * @param mode One of the USAGE_MODE_T values.
     */
    virtual void setUsageMode(USAGE_MODE_T mode);

    /**
     * Set the humidity sensor oversampling parameter.  See the data
     * sheet for details.  This value can be automatically set to a
     * suitable value by using one of the predefined modes for
     * setUsageMode().
     *
     * @param mode One of the OSRS_H_T values.
     */
    void setOversampleRateHumidity(OSRS_H_T rate);

    // Interface support
    const char *getModuleName()
    {
      return "BME280";
    };
    
    int getHumidityRelative()
    {
      return int(getHumidity());
    };
    
  protected:
    // relative humidity
    float m_humidity;

    // read the calibration data
    virtual void readCalibrationData();

  private:
    // calibration data humidity
    uint8_t m_dig_H1;
    int16_t m_dig_H2;
    uint8_t m_dig_H3;
    int16_t m_dig_H4;
    int16_t m_dig_H5;
    int8_t m_dig_H6;

    // Bosch supplied conversion/compensation functions from the
    // datasheet.
    uint32_t bme280_compensate_H_int32(int32_t adc_H);
  };
}
