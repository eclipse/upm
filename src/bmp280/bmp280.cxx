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

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <math.h>
#include <string.h>

#include "bmp280.hpp"

using namespace upm;
using namespace std;

// Uncomment the following to use test data as specified in the
// datasheet, section 3.12.  This really only tests the compensation
// algorithm.

// #define BMP280_USE_TEST_DATA

// conversion from fahrenheit to celsius and back

static float f2c(float f)
{
  return ((f - 32.0) / (9.0 / 5.0));
}

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

BMP280::BMP280(int bus, int addr, int cs, uint8_t theChipID) :
  m_i2c(0), m_spi(0), m_gpioCS(0)
{

  m_addr = addr;

  m_temperature = 0;
  m_pressure = 0;
  m_isSPI = false;

  clearData();

  if (addr < 0)
    m_isSPI = true;

  mraa::Result rv;

  if (m_isSPI)
    {
      m_spi = new mraa::Spi(bus);

      // Only create cs context if we are actually using a valid pin.
      // A hardware controlled pin should specify cs as -1.
      if (cs >= 0)
        {
          m_gpioCS = new mraa::Gpio(cs);
          m_gpioCS->dir(mraa::DIR_OUT);
        }

      m_spi->mode(mraa::SPI_MODE0);
      m_spi->frequency(5000000);

      // toggle it on/off so chip switches into SPI mode.  For a hw CS
      // pin, the first SPI transaction should accomplish this.
      csOn();
      usleep(10000);
      csOff();
    }
  else
    {
      // I2C
      m_i2c = new mraa::I2c(bus);

      if ((rv = m_i2c->address(m_addr)) != mraa::SUCCESS)
        {
          throw std::runtime_error(string(__FUNCTION__) +
                                   ": I2c.address() failed");
        }
    }

  // check the chip id

  uint8_t chipID = readReg(REG_CHIPID);
  if (chipID != theChipID)
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": invalid chip ID.  Expected "
                               + std::to_string(int(theChipID))
                               + ", got "
                               + std::to_string(int(chipID)));
    }

  // set sleep mode for now
  setMeasureMode(MODE_SLEEP);

  // read calibration data
  readCalibrationData();

  // set the default mode to the highest resolution mode
  setUsageMode(USAGE_MODE_INDOOR_NAV);
}

BMP280::~BMP280()
{
  if (m_i2c)
    delete m_i2c;

  if (m_spi)
    delete m_spi;

  if (m_gpioCS)
    delete m_gpioCS;
}

void BMP280::update()
{
  int32_t temp = 0;
  int32_t pres = 0;

  const int dataLen = 6;
  uint8_t data[dataLen];
  memset(data, 0, dataLen);

  // If we are using a forced mode, then we need to manually trigger
  // the measurement, and wait for it to complete.

  if (m_mode == MODE_FORCED)
    {
      // bmp280 measure mode will return to sleep after completion...
      setMeasureMode(MODE_FORCED);

      uint8_t stat;
      do 
        {
          usleep(10000); // 10ms
          stat = readReg(REG_STATUS);
        } while (stat & STATUS_MEASURING);
    }

  int rv;
  if ((rv = readRegs(REG_PRESSURE_MSB, data, dataLen)) != dataLen)
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": readRegs() failed, returned "
                               + std::to_string(rv));
    }

  // 20 bits unsigned stored in a 32bit signed quanty

#if defined(BMP280_USE_TEST_DATA)
  // taken from datasheet, section 3.12
  temp = 519888;
  pres = 415148;
#else
  temp = ( (data[5] >> 4) | (data[4] << 4) | (data[3] << 12) );
  pres = ( (data[2] >> 4) | (data[1] << 4) | (data[0] << 12) );
#endif

  m_temperature = float(bmp280_compensate_T_int32(temp));
  m_temperature /= 100.0;

  m_pressure = float(bmp280_compensate_P_int64(pres));
  m_pressure /= 256.0;
}

float BMP280::getAltitude(float sealLevelhPA)
{
  // adapted from the NOAA pdf: pressureAltitude.pdf
  return 44307.69 * (1.0 - pow((m_pressure/100.0) / sealLevelhPA, 0.190284));
}

uint8_t BMP280::readReg(uint8_t reg)
{
  if (m_isSPI)
    {
      reg |= 0x80; // needed for read
      uint8_t pkt[2] = {reg, 0};

      csOn();
      if (m_spi->transfer(pkt, pkt, 2))
        {
          csOff();
          throw std::runtime_error(string(__FUNCTION__)
                                   + ": Spi.transfer() failed");
        }
      csOff();

#if 0
      cerr << "readReg: " << std::hex << "p0: " << (int)pkt[0] << " p1: "
           << (int)pkt[1] << endl;
#endif // 0

      return pkt[1];
    }
  else
    return m_i2c->readReg(reg);
}

int BMP280::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
  if (m_isSPI)
    {
      reg |= 0x80; // needed for read

      uint8_t sbuf[len + 1];
      memset((char *)sbuf, 0, len + 1);
      sbuf[0] = reg;

      // We need to do it this way for edison - ie: use a single
      // transfer rather than breaking it up into two like we used to.
      // This means a buffer copy is now required, but that's the way
      // it goes.

      csOn();
      if (m_spi->transfer(sbuf, sbuf, len + 1))
        {
          csOff();
          throw std::runtime_error(string(__FUNCTION__)
                                   + ": Spi.transfer(buf) failed");
        }
      csOff();

      // now copy it into user buffer
      for (int i=0; i<len; i++)
        buffer[i] = sbuf[i + 1];

      // so... did this work on edison????

#if 0
      cerr << "readRegs(): " << std::hex;
      for (int i=0; i<len; i++)
        cerr << (int)buffer[i] << " ";
      cerr << endl;
#endif // 0

      return len;
    }
  else
    return m_i2c->readBytesReg(reg, buffer, len);
}

void BMP280::writeReg(uint8_t reg, uint8_t val)
{
  if (m_isSPI)
    {
      reg &= 0x7f; // mask off 0x80 for writing
      uint8_t pkt[2] = {reg, val};

      csOn();
      if (m_spi->transfer(pkt, NULL, 2))
        {
          csOff();
          throw std::runtime_error(string(__FUNCTION__)
                                   + ": Spi.transfer() failed");
        }
      csOff();
    }
  else
    {

      mraa::Result rv;
      if ((rv = m_i2c->writeReg(reg, val)) != mraa::SUCCESS)
        {
          throw std::runtime_error(std::string(__FUNCTION__)
                                   + ": I2c.writeReg() failed");
        }
    }
}

void BMP280::clearData()
{
  m_t_fine = 0;

  m_dig_T1 = 0;
  m_dig_T2 = 0;
  m_dig_T3 = 0;

  m_dig_P1 = 0;
  m_dig_P2 = 0;
  m_dig_P3 = 0;
  m_dig_P4 = 0;
  m_dig_P5 = 0;
  m_dig_P6 = 0;
  m_dig_P7 = 0;
  m_dig_P8 = 0;
  m_dig_P9 = 0;
}

uint8_t BMP280::getChipID()
{

  return readReg(REG_CHIPID);
}

void BMP280::reset()
{
  writeReg(REG_RESET, BMP280_RESET_BYTE);
  sleep(1);
}


void BMP280::readCalibrationData()
{
#if defined(BMP280_USE_TEST_DATA)
  cerr << "WARNING: Test data is being used" << endl;
  // This data is taken from the datasheet, section 3.12
  m_dig_T1 = 27504;
  m_dig_T2 = 26435;
  m_dig_T3 = -1000;

  m_dig_P1 = 36477;
  m_dig_P2 = -10685;
  m_dig_P3 = 3024;
  m_dig_P4 = 2855;
  m_dig_P5 = 140;
  m_dig_P6 = -7;
  m_dig_P7 = 15500;
  m_dig_P8 = -14600;
  m_dig_P9 = 6000;
  
#else

  uint8_t calibData[CALIBRATION_BYTES];
  readRegs(REG_CALIB00, calibData, CALIBRATION_BYTES);

  m_dig_T1 = uint16_t((calibData[1] << 8) | calibData[0]);
  m_dig_T2 = int16_t((calibData[3] << 8) | calibData[2]);
  m_dig_T3 = int16_t((calibData[5] << 8) | calibData[4]);

# if 0
  cerr << std::dec << "T1: " << (int)m_dig_T1
       << " T2: " << (int)m_dig_T2
       << " T3: " << (int)m_dig_T3
       << endl;
# endif // 0

  m_dig_P1 = uint16_t((calibData[7] << 8) | calibData[6]);
  m_dig_P2 = int16_t((calibData[9] << 8) | calibData[8]);
  m_dig_P3 = int16_t((calibData[11] << 8) | calibData[10]);
  m_dig_P4 = int16_t((calibData[13] << 8) | calibData[12]);
  m_dig_P5 = int16_t((calibData[15] << 8) | calibData[14]);
  m_dig_P6 = int16_t((calibData[17] << 8) | calibData[16]);
  m_dig_P7 = int16_t((calibData[19] << 8) | calibData[18]);
  m_dig_P8 = int16_t((calibData[21] << 8) | calibData[20]);
  m_dig_P9 = int16_t((calibData[23] << 8) | calibData[22]);

# if 0
  cerr << std::dec << "P1: " << (int)m_dig_P1
       << " P2: " << (int)m_dig_P2
       << " P3: " << (int)m_dig_P3
       << " P4: " << (int)m_dig_P4
       << " P5: " << (int)m_dig_P5
       << endl;
  cerr << std::dec << "P6: " << (int)m_dig_P6
       << " P7: " << (int)m_dig_P7
       << " P8: " << (int)m_dig_P8
       << " P9: " << (int)m_dig_P9
       << endl;
# endif // 0


#endif
}

float BMP280::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

float BMP280::getPressure()
{
  return m_pressure;
}

void BMP280::setFilter(FILTER_T filter)
{
  uint8_t reg = readReg(REG_CONFIG);

  reg &= ~(_CONFIG_FILTER_MASK << _CONFIG_FILTER_SHIFT);
  reg |= (filter << _CONFIG_FILTER_SHIFT);

  writeReg(REG_CONFIG, reg);
}

void BMP280::setTimerStandby(T_SB_T tsb)
{
  uint8_t reg = readReg(REG_CONFIG);

  reg &= ~(_CONFIG_T_SB_MASK << _CONFIG_T_SB_SHIFT);
  reg |= (tsb << _CONFIG_T_SB_SHIFT);

  writeReg(REG_CONFIG, reg);
}

void BMP280::setMeasureMode(MODES_T mode)
{
  uint8_t reg = readReg(REG_CTRL_MEAS);

  reg &= ~(_CTRL_MEAS_MODE_MASK << _CTRL_MEAS_MODE_SHIFT);
  reg |= (mode << _CTRL_MEAS_MODE_SHIFT);

  writeReg(REG_CTRL_MEAS, reg);
  m_mode = mode;
}

void BMP280::setOversampleRatePressure(OSRS_P_T rate)
{
  uint8_t reg = readReg(REG_CTRL_MEAS);

  reg &= ~(_CTRL_MEAS_OSRS_P_MASK << _CTRL_MEAS_OSRS_P_SHIFT);
  reg |= (rate << _CTRL_MEAS_OSRS_P_SHIFT);

  writeReg(REG_CTRL_MEAS, reg);
}

void BMP280::setOversampleRateTemperature(OSRS_T_T rate)
{
  uint8_t reg = readReg(REG_CTRL_MEAS);

  reg &= ~(_CTRL_MEAS_OSRS_T_MASK << _CTRL_MEAS_OSRS_T_SHIFT);
  reg |= (rate << _CTRL_MEAS_OSRS_T_SHIFT);

  writeReg(REG_CTRL_MEAS, reg);
}

uint8_t BMP280::getStatus()
{
  return readReg(REG_STATUS);
}

void BMP280::setUsageMode(USAGE_MODE_T mode)
{
  // set up the regs for the given usage mode.  These settings come
  // from the recomendations in the BMP280 datasheet, section 3.4
  // Filter Selection.

  m_temperature = 0;
  m_pressure = 0;

  // set sleep mode first
  setMeasureMode(MODE_SLEEP);
  
  switch (mode)
    {
    case USAGE_MODE_HANDHELD_LOW_POWER:
      setOversampleRatePressure(OSRS_P_OVERSAMPLING_16);
      setOversampleRateTemperature(OSRS_T_OVERSAMPLING_2);
      setFilter(FILTER_4);
      setMeasureMode(MODE_NORMAL);

      break;

    case USAGE_MODE_HANDHELD_DYNAMIC:
      setOversampleRatePressure(OSRS_P_OVERSAMPLING_4);
      setOversampleRateTemperature(OSRS_T_OVERSAMPLING_1);
      setFilter(FILTER_16);
      setMeasureMode(MODE_NORMAL);

      break;

    case USAGE_MODE_WEATHER_MONITOR:
      setOversampleRatePressure(OSRS_P_OVERSAMPLING_1);
      setOversampleRateTemperature(OSRS_T_OVERSAMPLING_1);
      setFilter(FILTER_OFF);
      setMeasureMode(MODE_FORCED);

      break;

    case USAGE_MODE_FLOOR_CHG_DETECT:
      setOversampleRatePressure(OSRS_P_OVERSAMPLING_4);
      setOversampleRateTemperature(OSRS_T_OVERSAMPLING_1);
      setFilter(FILTER_4);
      setMeasureMode(MODE_NORMAL);

      break;

    case USAGE_MODE_DROP_DETECT:
      setOversampleRatePressure(OSRS_P_OVERSAMPLING_2);
      setOversampleRateTemperature(OSRS_T_OVERSAMPLING_1);
      setFilter(FILTER_OFF);
      setMeasureMode(MODE_NORMAL);

      break;

    case USAGE_MODE_INDOOR_NAV:
      setOversampleRatePressure(OSRS_P_OVERSAMPLING_16);
      setOversampleRateTemperature(OSRS_T_OVERSAMPLING_2);
      setFilter(FILTER_16);
      setMeasureMode(MODE_NORMAL);

      break;

    default:
      throw std::logic_error(string(__FUNCTION__)
                             + ": invalid mode specified");
    }
}

void BMP280::csOn()
{
  if (m_gpioCS)
    m_gpioCS->write(0);
}

void BMP280::csOff()
{
  if (m_gpioCS)
    m_gpioCS->write(1);
}

// These functions come from the BMP280 datasheet, section 3.11.3

// Returns temperature in DegC, resolution is 0.01 DegC. Output value
// of "5123" equals 51.23 DegC.  t_fine carries fine temperature as
// global value
int32_t BMP280::bmp280_compensate_T_int32(int32_t adc_T)
{
  int32_t var1, var2, T;
  var1 = ((((adc_T>>3) - ((int32_t)m_dig_T1<<1))) * ((int32_t)m_dig_T2)) >> 11;
  var2 = (((((adc_T>>4) - ((int32_t)m_dig_T1)) * ((adc_T>>4) - ((int32_t)m_dig_T1))) >> 12) *
          ((int32_t)m_dig_T3)) >> 14;
  m_t_fine = var1 + var2;
  T = (m_t_fine * 5 + 128) >> 8;
  return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format
// (24 integer bits and 8 fractional bits).  Output value of
// “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t BMP280::bmp280_compensate_P_int64(int32_t adc_P)
{
  int64_t var1, var2, p;
  var1 = ((int64_t)m_t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)m_dig_P6;
  var2 = var2 + ((var1*(int64_t)m_dig_P5)<<17);
  var2 = var2 + (((int64_t)m_dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)m_dig_P3)>>8) + ((var1 * (int64_t)m_dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)m_dig_P1)>>33;
  if (var1 == 0)
    {
      return 0; // avoid exception caused by division by zero
    }
  p = 1048576-adc_P;
  p = (((p<<31)-var2)*3125)/var1;
  var1 = (((int64_t)m_dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)m_dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)m_dig_P7)<<4);
  return (uint32_t)p;
}
