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

#include "upm/iPressureSensor.hpp"
#include "upm/iTemperatureSensor.hpp"

#define BMP280_DEFAULT_I2C_BUS 0
#define BMP280_DEFAULT_SPI_BUS 0
#define BMP280_DEFAULT_ADDR 0x77
#define BMP280_DEFAULT_CHIPID 0x58

namespace upm {

  /**
   * @brief BMP280 Digital Pressure Sensor
   * @defgroup bmp280 libupm-bmp280
   * @ingroup i2c spi gpio pressure
   */

  /**
   * @library bmp280
   * @sensor bmp280
   * @comname BMP280 Digital Pressure Sensor
   * @type pressure
   * @man adafruit
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

  class BMP280 : public ITemperatureSensor, public IPressureSensor {
  public:
    // special reset byte
    const uint8_t BMP280_RESET_BYTE = 0xb6;

    // number of bytes of stored calibration data
    const int CALIBRATION_BYTES = 26;

    /**
     * BMP280 registers
     */
    typedef enum : uint8_t {
      // Do not write into reserved bits.

      // read-only factory calibration data
      REG_CALIB00                      = 0x88,
      REG_CALIB01                      = 0x89,
      REG_CALIB02                      = 0x8a,
      REG_CALIB03                      = 0x8b,
      REG_CALIB04                      = 0x8c,
      REG_CALIB05                      = 0x8d,
      REG_CALIB06                      = 0x8e,
      REG_CALIB07                      = 0x8f,
      REG_CALIB08                      = 0x90,
      REG_CALIB09                      = 0x91,
      REG_CALIB10                      = 0x92,
      REG_CALIB11                      = 0x93,
      REG_CALIB12                      = 0x94,
      REG_CALIB13                      = 0x95,
      REG_CALIB14                      = 0x96,
      REG_CALIB15                      = 0x97,
      REG_CALIB16                      = 0x98,
      REG_CALIB17                      = 0x99,
      REG_CALIB18                      = 0x9a,
      REG_CALIB19                      = 0x9b,
      REG_CALIB20                      = 0x9c,
      REG_CALIB21                      = 0x9d,
      REG_CALIB22                      = 0x9e,
      REG_CALIB23                      = 0x9f,
      REG_CALIB24                      = 0xa0,
      REG_CALIB25                      = 0xa1,

      REG_CHIPID                       = 0xd0,
      REG_RESET                        = 0xe0,
      REG_STATUS                       = 0xf3,
      REG_CTRL_MEAS                    = 0xf4,
      REG_CONFIG                       = 0xf5,

      REG_PRESSURE_MSB                 = 0xf7,
      REG_PRESSURE_LSB                 = 0xf8,
      REG_PRESSURE_XLSB                = 0xf9,

      REG_TEMPERATURE_MSB              = 0xfa,
      REG_TEMPERATURE_LSB              = 0xfb,
      REG_TEMPERATURE_XLSB             = 0xfc
    } BMP280_REGS_T;

    /**
     * REG_CONFIG bits
     */
    typedef enum {
      CONFIG_SPI3W_EN                  = 0x01,

      // 0x02 reserved

      CONFIG_FILTER0                   = 0x04,
      CONFIG_FILTER1                   = 0x08,
      CONFIG_FILTER2                   = 0x10,
      _CONFIG_FILTER_MASK              = 7,
      _CONFIG_FILTER_SHIFT             = 2,

      CONFIG_T_SB0                     = 0x20,
      CONFIG_T_SB1                     = 0x40,
      CONFIG_T_SB2                     = 0x80,
      _CONFIG_T_SB_MASK                = 7,
      _CONFIG_T_SB_SHIFT               = 5
    } CONFIG_BITS_T;

    /**
     * FILTER values (samples to reach >= 75% of step response)
     */
    typedef enum {
      FILTER_OFF                       = 0, // 1 samples
      FILTER_2                         = 1, // 2
      FILTER_4                         = 2, // 5
      FILTER_8                         = 3, // 11
      FILTER_16                        = 4  // 22
    } FILTER_T;

    /**
     * T_SB values (timer standby)
     */
    typedef enum {
      T_SB_0_5                         = 0, // 0.5ms
      T_SB_62_5                        = 1, // 62.5ms
      T_SB_125                         = 2, // 125ms
      T_SB_250                         = 3,
      T_SB_500                         = 4,
      T_SB_1000                        = 5,
      T_SB_2000                        = 6, // bme280 - 10ms
      T_SB_4000                        = 7  // bme280 - 20ms
    } T_SB_T;


    /**
     * REG_CTRL_MEAS bits
     */
    typedef enum {
      CTRL_MEAS_MODE0                  = 0x01,
      CTRL_MEAS_MODE1                  = 0x02,
      _CTRL_MEAS_MODE_MASK             = 3,
      _CTRL_MEAS_MODE_SHIFT            = 0,

      CTRL_MEAS_OSRS_P0                = 0x04,
      CTRL_MEAS_OSRS_P1                = 0x08,
      CTRL_MEAS_OSRS_P2                = 0x10,
      _CTRL_MEAS_OSRS_P_MASK           = 7,
      _CTRL_MEAS_OSRS_P_SHIFT          = 2,

      CTRL_MEAS_OSRS_T0                = 0x04,
      CTRL_MEAS_OSRS_T1                = 0x08,
      CTRL_MEAS_OSRS_T2                = 0x10,
      _CTRL_MEAS_OSRS_T_MASK           = 7,
      _CTRL_MEAS_OSRS_T_SHIFT          = 5
    } CTRL_MEAS_T;

    /**
     * CTRL_MEAS_MODE values
     */
    typedef enum {
      MODE_SLEEP                       = 0,
      MODE_FORCED                      = 1,
      // 2 is also FORCED mode
      MODE_NORMAL                      = 3
    } MODES_T;

    /**
     * CTRL_MEAS_OSRS_P values
     */
    typedef enum {
      OSRS_P_SKIPPED                   = 0,
      OSRS_P_OVERSAMPLING_1            = 1, // x1
      OSRS_P_OVERSAMPLING_2            = 2, // x2
      OSRS_P_OVERSAMPLING_4            = 3,
      OSRS_P_OVERSAMPLING_8            = 4,
      OSRS_P_OVERSAMPLING_16           = 5
    } OSRS_P_T;

    /**
     * CTRL_MEAS_OSRS_T values
     */
    typedef enum {
      OSRS_T_SKIPPED                   = 0,
      OSRS_T_OVERSAMPLING_1            = 1, // x1
      OSRS_T_OVERSAMPLING_2            = 2, // x2
      OSRS_T_OVERSAMPLING_4            = 3,
      OSRS_T_OVERSAMPLING_8            = 4,
      OSRS_T_OVERSAMPLING_16           = 5
    } OSRS_T_T;

    /**
     * REG_STATUS bits
     */
    typedef enum {
      STATUS_IM_UPDATE                 = 0x01,
      // 0x02-0x04 reserved
      STATUS_MEASURING                 = 0x08
      // 0x10-0x80 reserved
    } STATUS_T;

    /**
     * USAGE_MODE values.  This is a fake specification to configure
     * the various knobs based on their typical use modes, as
     * recommended by Bosch.
     */
    typedef enum {
      USAGE_MODE_HANDHELD_LOW_POWER    = 0,
      USAGE_MODE_HANDHELD_DYNAMIC      = 1,
      USAGE_MODE_WEATHER_MONITOR       = 2, // lowest power consumption
      USAGE_MODE_FLOOR_CHG_DETECT      = 3,
      USAGE_MODE_DROP_DETECT           = 4,
      USAGE_MODE_INDOOR_NAV            = 5  // highest resolution
    } USAGE_MODE_T;

    /**
     * BMP280 constructor.
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
     * @param theChipID The chip ID to use for validation
     */
    BMP280(int bus=BMP280_DEFAULT_I2C_BUS, int addr=BMP280_DEFAULT_ADDR,
           int cs=-1, uint8_t theChipID=BMP280_DEFAULT_CHIPID);

    /**
     * BMP280 Destructor.
     */
    virtual ~BMP280();

    /**
     * Update the internal stored values from sensor data.
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
    float getTemperature(bool fahrenheit=false);

    /**
     * Return the current measured pressure in Pascals (Pa).  update()
     * must have been called prior to calling this method.
     *
     * @return The pressure in Pascals (Pa).
     */
    float getPressure();

    /**
     * Return the current computed altitude in meters.  update()
     * must have been called prior to calling this method.
     *
     * @param seaLevelhPA The pressure at sea level in hectoPascals
     * (hPa).  The default is 1013.25 hPA, (101325 Pa).
     * @return The computed altitude in meters.
     */
    float getAltitude(float seaLevelhPA=1013.25);

    /**
     * Set a general usage mode.  This function can be used to
     * configure the filters and oversampling for a particular use
     * case.  These setting are documented in the BMP280 datasheet.
     * The default mode set in the contructor is
     * USAGE_MODE_INDOOR_NAV, the highest resolution mode.
     *
     * @param mode One of the USAGE_MODE_T values.
     */
    virtual void setUsageMode(USAGE_MODE_T mode);

    /**
     * Set the temperature sensor oversampling parameter.  See the
     * data sheet for details.  This value can be automatically set to
     * a suitable value by using one of the predefined modes for
     * setUsageMode().
     *
     * @param mode One of the OSRS_T_T values.
     */
    void setOversampleRateTemperature(OSRS_T_T rate);

    /**
     * Set the pressure sensor oversampling parameter.  See the
     * data sheet for details.  This value can be automatically set to
     * a suitable value by using one of the predefined modes for
     * setUsageMode().
     *
     * @param mode One of the OSRS_P_T values.
     */
    void setOversampleRatePressure(OSRS_P_T rate);

    /**
     * Set the timer standby value.  When in NORMAL operating mode,
     * this timer governs how long the chip will wait before
     * performing a measurement.  See the data sheet for details.
     *
     * @param mode One of the T_SB_T values.
     */
    void setTimerStandby(T_SB_T tsb);

    /**
     * Set the IIR filtering parameter.  See the data sheet for
     * details.  This value can be automatically set to a suitable
     * value by using one of the predefined modes for setUsageMode().
     *
     * @param mode One of the FILTER_T values.
     */
    void setFilter(FILTER_T filter);

    /**
     * Set the default measuring mode.  Basic values are forced,
     * sleep, and normal.  See the data sheet for details.  This value
     * can be automatically set to a suitable value by using one of
     * the predefined modes for setUsageMode().
     *
     * @param mode One of the MODES_T values.
     */
    void setMeasureMode(MODES_T mode);


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
    mraa::I2c *m_i2c;
    mraa::Spi *m_spi;
    mraa::Gpio *m_gpioCS;

    uint8_t m_addr;

    // always stored in C
    float m_temperature;

    // pressure in Pa
    float m_pressure;

    // shared calibration data - set in temp conversion, used in
    // pressure conversion.
    int32_t m_t_fine;

    // current operating mode.  MODE_FORCED requires special attention
    // in update()
    MODES_T m_mode;

    // return the status register
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
     */
    void writeReg(uint8_t reg, uint8_t val);

    // clear member data...
    void clearData();

    // read the calibration data
    virtual void readCalibrationData();

    // SPI chip select
    void csOn();
    void csOff();

  private:
    // are we doing SPI?
    bool m_isSPI;

    // calibration data temperature
    uint16_t m_dig_T1;
    int16_t m_dig_T2;
    int16_t m_dig_T3;

    // calibration data pressure
    uint16_t m_dig_P1;
    int16_t m_dig_P2;
    int16_t m_dig_P3;
    int16_t m_dig_P4;
    int16_t m_dig_P5;
    int16_t m_dig_P6;
    int16_t m_dig_P7;
    int16_t m_dig_P8;
    int16_t m_dig_P9;

    // Bosch supplied conversion/compensation functions from the
    // datasheet.
    int32_t bmp280_compensate_T_int32(int32_t adc_T);
    uint32_t bmp280_compensate_P_int64(int32_t adc_P);
  };
}
