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

// we have to do it the old skool way
#include <mraa/i2c.h>

#include "bmi160.hpp"

extern "C" {
#include "bosch_bmi160.h"
}

// We do not need this define anyway.  It conflicts with mraa::SUCCESS.
#undef SUCCESS

using namespace upm;
using namespace std;

static mraa_i2c_context i2cContext = NULL;

// Our bmi160 info structure
struct bmi160_t s_bmi160;

// bus read and write functions for use with the bmi driver code
s8 bmi160_i2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
  if (!i2cContext)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": i2c context is NULL");
    }

  int retries = 10;

  // There seems to be some occasional flakyness with reads when
  // moving the sensor around
  while (retries >= 0)
    {
      int rv = mraa_i2c_read_bytes_data(i2cContext, reg_addr, reg_data, cnt);

      if (rv < 0)
        {
          usleep(100000);
          retries--;
        }
      else
        return 0;
    }

  throw std::runtime_error(std::string(__FUNCTION__) +
                           ": mraa_i2c_read_bytes_data() failed");

  return 0;
}

s8 bmi160_i2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
  if (!i2cContext)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": i2c context is NULL");
    }

  // FIXME  fprintf(stderr, "%s: %02x: cnt %d\n", __FUNCTION__, reg_addr, cnt);
  uint8_t buffer[cnt + 1];

  buffer[0] = reg_addr;
  for (int i=0; i<cnt; i++)
    buffer[i+1] = reg_data[i];

  mraa_result_t rv = mraa_i2c_write(i2cContext, buffer, cnt+1);

  if (rv != MRAA_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_write() failed");
    }

  return 0;
}

// delay for some milliseconds
void bmi160_delay_ms(u32 msek)
{
  usleep(msek * 1000);
}


BMI160::BMI160(int bus, uint8_t address)
{
  m_addr = address;

  // We need to use the C MRAA interface to avoid issue with C++ <-> C
  // calling convention issues, also we need a global
  // mraa_i2c_context

  if (!(i2cContext = mraa_i2c_init(bus)))
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");
    }

  if (mraa_i2c_address(i2cContext, m_addr) != MRAA_SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_address() failed");
      return;
    }

  // init the driver interface functions
  s_bmi160.bus_write = bmi160_i2c_bus_write;
  s_bmi160.bus_read = bmi160_i2c_bus_read;
  s_bmi160.delay_msec = bmi160_delay_ms;
  s_bmi160.dev_addr = m_addr;

  // Init our driver interface pointers
  bmi160_init(&s_bmi160);

  m_accelX = 0.0;
  m_accelY = 0.0;
  m_accelZ = 0.0;

  m_gyroX = 0.0;
  m_gyroY = 0.0;
  m_gyroZ = 0.0;

  m_magX = 0.0;
  m_magY = 0.0;
  m_magZ = 0.0;

  m_accelScale = 1.0;
  m_gyroScale = 1.0;

  m_magEnabled = false;

  if (!init())
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": init() failed");
    }
}

BMI160::~BMI160()
{
  mraa_i2c_stop(i2cContext);
  i2cContext = NULL;
}

bool BMI160::init()
{
  // This should be interesting...
  const u32 C_BMI160_THIRTY_U8X = 30;

  enableMagnetometer(true);

  /*Set the accel mode as Normal write in the register 0x7E*/
  bmi160_set_command_register(ACCEL_MODE_NORMAL);

  /* bmi160_delay_ms in ms*/
  bmi160_delay_ms(C_BMI160_THIRTY_U8X);

  /*Set the gyro mode as Normal write in the register 0x7E*/
  bmi160_set_command_register(GYRO_MODE_NORMAL);

  /* bmi160_delay_ms in ms*/
  bmi160_delay_ms(C_BMI160_THIRTY_U8X);

  /* Set the accel bandwidth as OSRS4 */
  bmi160_set_accel_bw(BMI160_ACCEL_OSR4_AVG1);
  bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

  /* Set the gryo bandwidth as Normal */
  bmi160_set_gyro_bw(BMI160_GYRO_NORMAL_MODE);
  bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

  /* set gyro data rate as 200Hz*/
  bmi160_set_gyro_output_data_rate(BMI160_GYRO_OUTPUT_DATA_RATE_200HZ);
  bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

  /* set accel data rate as 200Hz*/
  bmi160_set_accel_output_data_rate(BMI160_ACCEL_OUTPUT_DATA_RATE_200HZ,
                                    BMI160_ACCEL_OSR4_AVG1);
  bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

  setAccelerometerScale(ACCEL_RANGE_2G);
  setGyroscopeScale(GYRO_RANGE_125);

  return true;
}


void BMI160::update()
{
  struct bmi160_gyro_t gyroxyz;
  struct bmi160_accel_t accelxyz;
  struct bmi160_mag_xyz_s32_t magxyz;

  // read gyro data
  bmi160_read_gyro_xyz(&gyroxyz);

  // read accel data
  bmi160_read_accel_xyz(&accelxyz);

  // read mag data
  if (m_magEnabled)
    bmi160_bmm150_mag_compensate_xyz(&magxyz);

  // read the sensor time
  u32 v_sensor_time;
  bmi160_get_sensor_time(&v_sensor_time);
  m_sensorTime = (unsigned int)v_sensor_time;

  m_accelX = float(accelxyz.x);
  m_accelY = float(accelxyz.y);
  m_accelZ = float(accelxyz.z);

  m_gyroX = float(gyroxyz.x);
  m_gyroY = float(gyroxyz.y);
  m_gyroZ = float(gyroxyz.z);

  if (m_magEnabled)
    {
      m_magX = float(magxyz.x);
      m_magY = float(magxyz.y);
      m_magZ = float(magxyz.z);
    }
}

void BMI160::setAccelerometerScale(ACCEL_RANGE_T scale)
{
  s8 v_range = BMI160_ACCEL_RANGE_2G;
  // store scaling factor

  switch (scale)
    {
    case ACCEL_RANGE_2G:
      v_range = BMI160_ACCEL_RANGE_2G;
      m_accelScale = 16384.0;
      break;

    case ACCEL_RANGE_4G:
      v_range = BMI160_ACCEL_RANGE_4G;
      m_accelScale = 8192.0;
      break;

    case ACCEL_RANGE_8G:
      v_range = BMI160_ACCEL_RANGE_8G;
      m_accelScale = 4096.0;
      break;

    case ACCEL_RANGE_16G:
      v_range = BMI160_ACCEL_RANGE_16G;
      m_accelScale = 2048.0;
      break;

    default: // should never occur, but...
      m_accelScale = 1.0;        // set a safe, though incorrect value
      throw std::logic_error(string(__FUNCTION__) +
                             ": internal error, unsupported scale");
      break;
    }

  bmi160_set_accel_range(v_range);

  return;
}

void BMI160::setGyroscopeScale(GYRO_RANGE_T scale)
{
  u8 v_range = BMI160_GYRO_RANGE_2000_DEG_SEC;

  // store scaling factor

  switch (scale)
    {
    case GYRO_RANGE_125:
      v_range = BMI160_GYRO_RANGE_125_DEG_SEC;
      m_gyroScale = 262.4;
      break;

    case GYRO_RANGE_250:
      v_range = BMI160_GYRO_RANGE_250_DEG_SEC;
      m_gyroScale = 131.2;
      break;

    case GYRO_RANGE_500:
      v_range = BMI160_GYRO_RANGE_500_DEG_SEC;
      m_gyroScale = 65.6;
      break;

    case GYRO_RANGE_1000:
      v_range = BMI160_GYRO_RANGE_1000_DEG_SEC;
      m_gyroScale = 32.8;
      break;

    case GYRO_RANGE_2000:
      v_range = BMI160_GYRO_RANGE_2000_DEG_SEC;
      m_gyroScale = 16.4;
      break;

    default: // should never occur, but...
      m_gyroScale = 1.0;        // set a safe, though incorrect value
      throw std::logic_error(string(__FUNCTION__) +
                             ": internal error, unsupported scale");
      break;
    }

  bmi160_set_gyro_range(v_range);

  return;
}

void BMI160::getAccelerometer(float *x, float *y, float *z)
{
  if (x)
    *x = m_accelX / m_accelScale;

  if (y)
    *y = m_accelY / m_accelScale;

  if (z)
    *z = m_accelZ / m_accelScale;
}

void BMI160::getGyroscope(float *x, float *y, float *z)
{
  if (x)
    *x = m_gyroX / m_gyroScale;

  if (y)
    *y = m_gyroY / m_gyroScale;

  if (z)
    *z = m_gyroZ / m_gyroScale;
}

void BMI160::getMagnetometer(float *x, float *y, float *z)
{
  if (x)
    *x = m_magX;

  if (y)
    *y = m_magY;

  if (z)
    *z = m_magZ;
}

float *BMI160::getAccelerometer()
{
  float *values = new float[3]; // x, y, and then z

  getAccelerometer(&values[0], &values[1], &values[2]);

  return values;
}

float *BMI160::getGyroscope()
{
  float *values = new float[3]; // x, y, and then z

  getGyroscope(&values[0], &values[1], &values[2]);

  return values;
}

float *BMI160::getMagnetometer()
{
  float *values = new float[3]; // x, y, and then z

  getMagnetometer(&values[0], &values[1], &values[2]);

  return values;
}

void BMI160::enableMagnetometer(bool enable)
{
  // butchered from support example
  if (!enable)
    {
      bmi160_set_bmm150_mag_and_secondary_if_power_mode(MAG_SUSPEND_MODE);
      bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);
      bmi160_set_if_mode(0x00);
      bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

      m_magEnabled = false;
      m_magX = 0;
      m_magY = 0;
      m_magZ = 0;
    }
  else
    {
      u8 v_bmm_chip_id_u8 = BMI160_INIT_VALUE;
      /* Init the magnetometer */
      bmi160_bmm150_mag_interface_init(&v_bmm_chip_id_u8);

      /* bmi160_delay_ms in ms*/
      bmi160_delay_ms(BMI160_GEN_READ_WRITE_DELAY);

      m_magEnabled = true;
    }
}

unsigned int BMI160::getSensorTime()
{
  return m_sensorTime;
}
