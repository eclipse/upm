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
#include <string.h>

#include "bno055.hpp"

using namespace upm;
using namespace std;

// conversion from fahrenheit to celsius and back

static float f2c(float f)
{
  return ((f - 32.0) / (9.0 / 5.0));
}

static float c2f(float c)
{
  return (c * (9.0 / 5.0) + 32.0);
}

BNO055::BNO055(int bus, uint8_t addr) :
  m_i2c(bus), m_gpioIntr(0)
{

  m_addr = addr;

  clearData();

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::runtime_error(string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }

  // forcibly set page 0, so we are synced
  setPage(0, true);

  // set config mode
  setOperationMode(OPERATION_MODE_CONFIGMODE);

  // default to internal clock
  setClockExternal(false);

  // we specifically avoid doing a reset so that if the device is
  // already calibrated, it will remain so.

  // check the chip id

  uint8_t chipID = readReg(REG_CHIP_ID);
  if (chipID != BNO055_CHIPID)
    {
      throw std::runtime_error(string(__FUNCTION__)
                               + ": invalid chip ID.  Expected "
                               + std::to_string(int(BNO055_CHIPID))
                               + ", got "
                               + std::to_string(int(chipID)));
      return;
    }

  // default to temperature C
  setTemperatureUnits(true);

  // default to accelerometer temp
  setTemperatureSource(TEMP_SOURCE_ACC);

  // set accel units to m/s^2
  setAccelerometerUnits(false);

  // set gyro units to degrees
  setGyroscopeUnits(false);

  // set Euler units to degrees
  setEulerUnits(false);

  // by default, we set the operating mode to the NDOF fusion mode
  setOperationMode(OPERATION_MODE_NDOF);
}

BNO055::~BNO055()
{
  uninstallISR();
}

void BNO055::update()
{
  setPage(0);

  // temperature first, we always store as C
  float tmpF = float((int8_t)readReg(REG_TEMPERATURE));
  if (m_tempIsC)
    m_temperature = tmpF;
  else
    m_temperature = f2c(tmpF * 2.0);

  updateFusionData();
  updateNonFusionData();
}

uint8_t BNO055::readReg(uint8_t reg)
{
  return m_i2c.readReg(reg);
}

void BNO055::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
  m_i2c.readBytesReg(reg, buffer, len);
}

bool BNO055::writeReg(uint8_t reg, uint8_t val)
{
  mraa::Result rv;
  if ((rv = m_i2c.writeReg(reg, val)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": I2c.writeReg() failed");
    }

  return true;
}

bool BNO055::writeRegs(uint8_t reg, uint8_t *buffer, int len)
{
  uint8_t buf[len + 1];

  buf[0] = reg;
  for (int i=0; i<len; i++)
    buf[i+1] = buffer[i];

  mraa::Result rv;
  if ((rv = m_i2c.write(buf, len+1)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": I2c.write() failed");
    }

  return true;
}

uint8_t BNO055::getChipID()
{
  setPage(0);
  return readReg(REG_CHIP_ID);
}

uint8_t BNO055::getACCID()
{
  setPage(0);
  return readReg(REG_ACC_ID);
}

uint8_t BNO055::getMAGID()
{
  setPage(0);
  return readReg(REG_MAG_ID);
}

uint8_t BNO055::getGYRID()
{
  setPage(0);
  return readReg(REG_GYR_ID);
}

uint16_t BNO055::getSWRevID()
{
  setPage(0);

  uint16_t vers = uint16_t( readReg(REG_SW_REV_ID_LSB) |
                            (readReg(REG_SW_REV_ID_MSB) << 8) );

  return vers;
}

uint8_t BNO055::getBootLoaderID()
{
  setPage(0);
  return readReg(REG_BL_REV_ID);
}

void BNO055::setPage(uint8_t page, bool force)
{
  // page can only be 0 or 1
  if (!(page == 0 || page == 1))
    throw std::out_of_range(string(__FUNCTION__) +
                            ": page can only be 0 or 1");

  if (force || page != m_currentPage)
    writeReg(REG_PAGE_ID, page);

  m_currentPage = page;
}

void BNO055::setClockExternal(bool extClock)
{
  setPage(0);

  // first we need to be in config mode
  OPERATION_MODES_T currentMode = m_currentMode;
  setOperationMode(OPERATION_MODE_CONFIGMODE);

  uint8_t reg = readReg(REG_SYS_TRIGGER);

  if (extClock)
    reg |= SYS_TRIGGER_CLK_SEL;
  else
    reg &= ~SYS_TRIGGER_CLK_SEL;

  writeReg(REG_SYS_TRIGGER, reg);

  // now reset our operating mode
  setOperationMode(currentMode);
}

void BNO055::setTemperatureSource(TEMP_SOURCES_T src)
{
  setPage(0);
  writeReg(REG_TEMP_SOURCE, src);
}

void BNO055::setTemperatureUnits(bool celsius)
{
  setPage(0);

  uint8_t reg = readReg(REG_UNIT_SEL);

  if (celsius)
    reg &= ~UNIT_SEL_TEMP_UNIT;
  else
    reg |= UNIT_SEL_TEMP_UNIT;

  writeReg(REG_UNIT_SEL, reg);

  m_tempIsC = celsius;
}

void BNO055::setAccelerometerUnits(bool mg)
{
  setPage(0);

  uint8_t reg = readReg(REG_UNIT_SEL);

  if (mg)
    {
      reg |= UNIT_SEL_ACC_UNIT;
      m_accUnitScale = 1.0;
    }
  else
    {
      reg &= ~UNIT_SEL_ACC_UNIT;
      m_accUnitScale = 100.0;
    }

  writeReg(REG_UNIT_SEL, reg);
}

void BNO055::setGyroscopeUnits(bool radians)
{
  setPage(0);

  uint8_t reg = readReg(REG_UNIT_SEL);

  if (radians)
    {
      reg |= UNIT_SEL_GYR_UNIT;
      m_gyrUnitScale = 900.0;
    }
  else
    {
      reg &= ~UNIT_SEL_GYR_UNIT;
      m_gyrUnitScale = 16.0;
    }

  writeReg(REG_UNIT_SEL, reg);
}

void BNO055::setEulerUnits(bool radians)
{
  setPage(0);

  uint8_t reg = readReg(REG_UNIT_SEL);

  if (radians)
    {
      reg |= UNIT_SEL_EUL_UNIT;
      m_eulUnitScale = 900.0;
    }
  else
    {
      reg &= ~UNIT_SEL_EUL_UNIT;
      m_eulUnitScale = 16.0;
    }

  writeReg(REG_UNIT_SEL, reg);
}

void BNO055::setOperationMode(OPERATION_MODES_T mode)
{
  setPage(0);

  // we clear all of our loaded data on mode changes
  clearData();

  uint8_t reg = readReg(REG_OPER_MODE);

  reg &= ~(_OPR_MODE_OPERATION_MODE_MASK << _OPR_MODE_OPERATION_MODE_SHIFT);

  reg |= (mode << _OPR_MODE_OPERATION_MODE_SHIFT);

  writeReg(REG_OPER_MODE, reg);
  m_currentMode = mode;

  usleep(30);
}

void BNO055::getCalibrationStatus(int *mag, int *acc, int *gyr, int *sys)
{
  setPage(0);

  uint8_t reg = readReg(REG_CALIB_STAT);

  if (mag)
    *mag = (reg >> _CALIB_STAT_MAG_SHIFT) & _CALIB_STAT_MAG_MASK;

  if (acc)
    *acc = (reg >> _CALIB_STAT_ACC_SHIFT) & _CALIB_STAT_ACC_MASK;

  if (gyr)
    *gyr = (reg >> _CALIB_STAT_GYR_SHIFT) & _CALIB_STAT_GYR_MASK;

  if (sys)
    *sys = (reg >> _CALIB_STAT_SYS_SHIFT) & _CALIB_STAT_SYS_MASK;
}

int *BNO055::getCalibrationStatus()
{
  static int v[4]; // mag, acc, gyr, sys;

  getCalibrationStatus(&v[0], &v[1], &v[2], &v[3]);
  return v;
}

bool BNO055::isFullyCalibrated()
{
  int mag, acc, gyr, sys;

  getCalibrationStatus(&mag, &acc, &gyr, &sys);

  // all of them equal to 3 means fully calibrated
  if (mag == 3 && acc == 3 && gyr == 3 && sys == 3)
    return true;
  else
    return false;
}

void BNO055::resetSystem()
{
  setPage(0);

  uint8_t reg = readReg(REG_SYS_TRIGGER);

  reg |= SYS_TRIGGER_RST_SYS;

  writeReg(REG_SYS_TRIGGER, reg);
  sleep(1);
}

void BNO055::resetInterruptStatus()
{
  setPage(0);

  uint8_t reg = readReg(REG_SYS_TRIGGER);

  reg |= SYS_TRIGGER_RST_INT;

  writeReg(REG_SYS_TRIGGER, reg);
}

uint8_t BNO055::getInterruptStatus()
{
  setPage(0);

  return readReg(REG_INT_STA);
}

uint8_t BNO055::getInterruptEnable()
{
  setPage(1);

  return readReg(REG_INT_EN);
}

void BNO055::setInterruptEnable(uint8_t enables)
{
  setPage(1);

  writeReg(REG_INT_EN, enables);
}

uint8_t BNO055::getInterruptMask()
{
  setPage(1);

  return readReg(REG_INT_MSK);
}

void BNO055::setInterruptMask(uint8_t mask)
{
  setPage(1);

  writeReg(REG_INT_MSK, mask);
}

BNO055::SYS_STATUS_T BNO055::getSystemStatus()
{
  setPage(0);

  return static_cast<BNO055::SYS_STATUS_T>(readReg(REG_SYS_STATUS));
}

BNO055::SYS_ERR_T BNO055::getSystemError()
{
  setPage(0);

  return static_cast<BNO055::SYS_ERR_T>(readReg(REG_SYS_ERROR));
}

string BNO055::readCalibrationData()
{
  if (!isFullyCalibrated())
    {
      cerr << __FUNCTION__ << ": Sensor must be fully calibrated first."
           << endl;
      return "";
    }

  // should be at page 0, but lets make sure
  setPage(0);

  // first we need to go back into config mode
  OPERATION_MODES_T currentMode = m_currentMode;
  setOperationMode(OPERATION_MODE_CONFIGMODE);

  uint8_t calibData[calibrationDataNumBytes];
  readRegs(REG_ACC_OFFSET_X_LSB, calibData, calibrationDataNumBytes);

  string rv((char *)calibData, calibrationDataNumBytes);

  // now reset our operating mode
  setOperationMode(currentMode);

  return rv;
}

void BNO055::writeCalibrationData(string calibData)
{
  if (calibData.size() != calibrationDataNumBytes)
    {
      throw std::invalid_argument(std::string(__FUNCTION__)
                                  + ": calibData string must be exactly "
                                  + std::to_string(calibrationDataNumBytes)
                                  + " bytes long");
    }

  // should be at page 0, but lets make sure
  setPage(0);

  // first we need to go back into config mode
  OPERATION_MODES_T currentMode = m_currentMode;
  setOperationMode(OPERATION_MODE_CONFIGMODE);

  // write the data
  writeRegs(REG_ACC_OFFSET_X_LSB, (uint8_t *)calibData.c_str(),
            calibData.size());

  // now reset our operating mode
  setOperationMode(currentMode);
}

float BNO055::getTemperature(bool fahrenheit)
{
  if (fahrenheit)
    return c2f(m_temperature);
  else
    return m_temperature;
}

void BNO055::clearData()
{
  m_magX = m_magY = m_magZ = 0;
  m_accX = m_accY = m_accZ = 0;
  m_gyrX = m_gyrY = m_gyrZ = 0;
  m_eulHeading = m_eulRoll = m_eulPitch = 0;
  m_quaW = m_quaX = m_quaY = m_quaZ = 0;
  m_liaX = m_liaY = m_liaZ = 0;
  m_grvX = m_grvY = m_grvZ = 0;
}

bool BNO055::updateFusionData()
{
  // bail if we are in config mode, or aren't in a fusion mode...
  if (m_currentMode == OPERATION_MODE_CONFIGMODE ||
      m_currentMode < OPERATION_MODE_IMU)
    return false;

  setPage(0);

  // FIXME/MAYBE? - abort early if SYS calibration is == 0?

  const int fusionBytes = 26;
  uint8_t buf[fusionBytes];

  readRegs(REG_EUL_HEADING_LSB, buf, fusionBytes);

  m_eulHeading = float(int16_t(buf[0] | (buf[1] << 8)));
  m_eulRoll = float(int16_t(buf[2] | (buf[3] << 8)));
  m_eulPitch = float(int16_t(buf[4] | (buf[5] << 8)));

  m_quaW = float(int16_t(buf[6] | (buf[7] << 8)));
  m_quaX = float(int16_t(buf[8] | (buf[9] << 8)));
  m_quaY = float(int16_t(buf[10] | (buf[11] << 8)));
  m_quaZ = float(int16_t(buf[12] | (buf[13] << 8)));

  m_liaX = float(int16_t(buf[14] | (buf[15] << 8)));
  m_liaY = float(int16_t(buf[16] | (buf[17] << 8)));
  m_liaZ = float(int16_t(buf[18] | (buf[19] << 8)));

  m_grvX = float(int16_t(buf[20] | (buf[21] << 8)));
  m_grvY = float(int16_t(buf[22] | (buf[23] << 8)));
  m_grvZ = float(int16_t(buf[24] | (buf[25] << 8)));

  return true;
}

bool BNO055::updateNonFusionData()
{
  // bail if we are in config mode...
  if (m_currentMode == OPERATION_MODE_CONFIGMODE)
    return false;

  setPage(0);

  const int nonFusionBytes = 18;
  uint8_t buf[nonFusionBytes];

  readRegs(REG_ACC_DATA_X_LSB, buf, nonFusionBytes);

  m_accX = float(int16_t(buf[0] | (buf[1] << 8)));
  m_accY = float(int16_t(buf[2] | (buf[3] << 8)));
  m_accZ = float(int16_t(buf[4] | (buf[5] << 8)));

  m_magX = float(int16_t(buf[6] | (buf[7] << 8)));
  m_magY = float(int16_t(buf[8] | (buf[9] << 8)));
  m_magZ = float(int16_t(buf[10] | (buf[11] << 8)));

  m_gyrX = float(int16_t(buf[12] | (buf[13] << 8)));
  m_gyrY = float(int16_t(buf[14] | (buf[15] << 8)));
  m_gyrZ = float(int16_t(buf[16] | (buf[17] << 8)));

  return true;
}

void BNO055::getEulerAngles(float *heading, float *roll, float *pitch)
{
  if (heading)
    *heading = m_eulHeading / m_eulUnitScale;

  if (roll)
    *roll = m_eulRoll / m_eulUnitScale;

  if (pitch)
    *pitch = m_eulPitch / m_eulUnitScale;
}

float *BNO055::getEulerAngles()
{
  static float v[3];
  getEulerAngles(&v[0], &v[1], &v[2]);
  return v;
}

void BNO055::getQuaternions(float *w, float *x, float *y, float *z)
{
  // from the datasheet
  const float scale = float(1.0 / (1 << 14));

  if (w)
    *w = m_quaW * scale;

  if (x)
    *x = m_quaX * scale;

  if (y)
    *y = m_quaY * scale;

  if (z)
    *z = m_quaZ * scale;
}

float *BNO055::getQuaternions()
{
  static float v[4];
  getQuaternions(&v[0], &v[1], &v[2], &v[3]);
  return v;
}

void BNO055::getLinearAcceleration(float *x, float *y, float *z)
{
  if (x)
    *x = m_liaX / m_accUnitScale;

  if (y)
    *y = m_liaY / m_accUnitScale;

  if (z)
    *z = m_liaZ / m_accUnitScale;
}

float *BNO055::getLinearAcceleration()
{
  static float v[3];
  getLinearAcceleration(&v[0], &v[1], &v[2]);
  return v;
}

void BNO055::getGravityVectors(float *x, float *y, float *z)
{
  if (x)
    *x = m_grvX / m_accUnitScale;

  if (y)
    *y = m_grvY / m_accUnitScale;

  if (z)
    *z = m_grvZ / m_accUnitScale;
}

float *BNO055::getGravityVectors()
{
  static float v[3];
  getGravityVectors(&v[0], &v[1], &v[2]);
  return v;
}

void BNO055::getAccelerometer(float *x, float *y, float *z)
{
  if (x)
    *x = m_accX / m_accUnitScale;

  if (y)
    *y = m_accY / m_accUnitScale;

  if (z)
    *z = m_accZ / m_accUnitScale;
}

float *BNO055::getAccelerometer()
{
  static float v[3];
  getAccelerometer(&v[0], &v[1], &v[2]);
  return v;
}

void BNO055::getMagnetometer(float *x, float *y, float *z)
{
  // from the datasheet - 16 uT's per LSB
  const float scale = 16.0;

  if (x)
    *x = m_magX / scale;

  if (y)
    *y = m_magY / scale;

  if (z)
    *z = m_magZ / scale;
}

float *BNO055::getMagnetometer()
{
  static float v[3];
  getMagnetometer(&v[0], &v[1], &v[2]);
  return v;
}

void BNO055::getGyroscope(float *x, float *y, float *z)
{
  if (x)
    *x = m_gyrX / m_gyrUnitScale;

  if (y)
    *y = m_gyrY / m_gyrUnitScale;

  if (z)
    *z = m_gyrZ / m_gyrUnitScale;
}

float *BNO055::getGyroscope()
{
  static float v[3];
  getGyroscope(&v[0], &v[1], &v[2]);
  return v;
}

void BNO055::setAccelerationConfig(ACC_RANGE_T range, ACC_BW_T bw,
                                   ACC_PWR_MODE_T pwr)
{
  setPage(1);

  uint8_t reg = ((range << _ACC_CONFIG_ACC_RANGE_SHIFT) |
                 (bw << _ACC_CONFIG_ACC_BW_SHIFT) |
                 (pwr << _ACC_CONFIG_ACC_PWR_MODE_SHIFT));

  writeReg(REG_ACC_CONFIG, reg);
}

void BNO055::setMagnetometerConfig(MAG_ODR_T odr, MAG_OPR_T opr,
                                   MAG_POWER_T pwr)
{
  setPage(1);

  uint8_t reg = ((odr << _MAG_CONFIG_MAG_ODR_SHIFT) |
                 (opr << _MAG_CONFIG_MAG_OPR_MODE_SHIFT) |
                 (pwr << _MAG_CONFIG_MAG_POWER_MODE_SHIFT));

  writeReg(REG_MAG_CONFIG, reg);
}

void BNO055::setGyroscopeConfig(GYR_RANGE_T range, GYR_BW_T bw,
                                GYR_POWER_MODE_T pwr)
{
  setPage(1);

  uint8_t reg = ((range << _GYR_CONFIG0_GYR_RANGE_SHIFT) |
                 (bw << _GYR_CONFIG0_GYR_BW_SHIFT));

  writeReg(REG_GYR_CONFIG0, reg);

  reg = (pwr << _GYR_CONFIG1_GYR_POWER_MODE_SHIFT);

  writeReg(REG_GYR_CONFIG1, reg);
}

#if defined(SWIGJAVA) || (JAVACALLBACK)
void BNO055::installISR(int gpio, mraa::Edge level,
                        jobject runnable)
{
  // delete any existing ISR and GPIO context
  uninstallISR();

  // create gpio context
  m_gpioIntr = new mraa::Gpio(gpio);

  m_gpioIntr->dir(mraa::DIR_IN);
  m_gpioIntr->isr(level, runnable);

}
#else
void BNO055::installISR(int gpio, mraa::Edge level,
                        void (*isr)(void *), void *arg)
{
  // delete any existing ISR and GPIO context
  uninstallISR();

  // create gpio context
  m_gpioIntr = new mraa::Gpio(gpio);

  m_gpioIntr->dir(mraa::DIR_IN);
  m_gpioIntr->isr(level, isr, arg);
}
#endif

void BNO055::uninstallISR()
{
  if (m_gpioIntr)
    {
      m_gpioIntr->isrExit();
      delete m_gpioIntr;

      m_gpioIntr = 0;
    }
}
