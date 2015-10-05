/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <iostream>
#include <stdexcept>
#include <string>

#include "mma7660.h"

using namespace upm;
using namespace std;


MMA7660::MMA7660(int bus, uint8_t address)
{
  m_addr = address;
  m_isrInstalled = false;

  // setup our i2c link
  if ( !(m_i2c = mraa_i2c_init(bus)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_init() failed");
      return;
    }
      
  mraa_result_t rv;
  
  if ( (rv = mraa_i2c_address(m_i2c, m_addr)) != MRAA_SUCCESS)
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_i2c_address() failed");
      return;
    }
}

MMA7660::~MMA7660()
{
  if (m_isrInstalled)
    uninstallISR();

  setModeStandby();
  mraa_i2c_stop(m_i2c);
}

bool MMA7660::writeByte(uint8_t reg, uint8_t byte)
{
  mraa_result_t rv = mraa_i2c_write_byte_data(m_i2c, byte, reg);

  if (rv != MRAA_SUCCESS)
    {
      cerr << __FUNCTION__ << ": mraa_i2c_write_byte() failed." << endl;
      mraa_result_print(rv);
      return false;
    }

  return true;
}

uint8_t MMA7660::readByte(uint8_t reg)
{
  return mraa_i2c_read_byte_data(m_i2c, reg);
}

void MMA7660::getRawValues(int *x, int *y, int *z)
{
  *x = getVerifiedAxis(REG_XOUT);
  *y = getVerifiedAxis(REG_YOUT);
  *z = getVerifiedAxis(REG_ZOUT);
}

#ifdef JAVACALLBACK
int *MMA7660::getRawValues()
{
  int *values = new int[3];
  getRawValues(&values[0], &values[1], &values[2]);
  return values;
}
#endif

void MMA7660::setModeActive()
{
  uint8_t modeReg = readByte(REG_MODE);

  // The D2 (TON bit) should be cleared, and the MODE bit set

  modeReg &= ~MODE_TON;
  modeReg |= MODE_MODE;

  writeByte(REG_MODE, modeReg);
}

void MMA7660::setModeStandby()
{
  uint8_t modeReg = readByte(REG_MODE);

  // the D0 (mode bit) and D2 (TON bit) should be cleared.

  modeReg &= ~MODE_TON;
  modeReg &= ~MODE_MODE;

  writeByte(REG_MODE, modeReg);
}

// read an axis value, verifying it's validity
int MMA7660::getVerifiedAxis(MMA7660_REG_T axis)
{
  // We only want one of the 3 axes

  if (axis > 2)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": axis must be 0, 1, or 2.");
      return 0;
    }

  // we need to check the alert bit and sign bits if the alert bit is
  // set, this means that the register was being updated when the
  // register was read, so re-read until it's clear.
  
  uint8_t val;
  do {
    val = readByte(axis);

    // check alert bit
  } while (val & 0x40);

  // shift the sign bit over, and compensate
  return (char(val << 2) / 4);
}

// read the tilt register, verifying it's validity
uint8_t MMA7660::getVerifiedTilt()
{
  // we need to check the alert bit and sign bits if the alert bit is
  // set, this means that the register was being updated when the
  // register was read, so re-read until it's clear.
  
  uint8_t val;
  do {
    val = readByte(REG_TILT);

    // check alert bit
  } while (val & 0x40);

  return val;
}

uint8_t MMA7660::tiltBackFront()
{
  uint8_t val = getVerifiedTilt();

  // mask off the bits we don't care about
  val &= 0x03;
  return val;
}

uint8_t MMA7660::tiltLandscapePortrait()
{
  uint8_t val = getVerifiedTilt();

  // mask off the bits we don't care about
  val >>= 2;
  val &= 0x07;
  return val;
}

bool MMA7660::tiltTap()
{
  uint8_t val = getVerifiedTilt();

  if (val & 0x20)
    return true;
  else
    return false;
}

bool MMA7660::tiltShake()
{
  uint8_t val = getVerifiedTilt();

  if (val & 0x80)
    return true;
  else
    return false;
}

#ifdef JAVACALLBACK
void MMA7660::installISR(int pin, IsrCallback *cb)
{
        installISR(pin, generic_callback_isr, cb);
}
#endif

void MMA7660::installISR(int pin, void (*isr)(void *), void *arg)
{
  if (m_isrInstalled)
    uninstallISR();

  if ( !(m_gpio = mraa_gpio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_gpio_init() failed, invalid pin?");
      return;
    }

  mraa_gpio_dir(m_gpio, MRAA_GPIO_IN);

  // install our interrupt handler
  mraa_gpio_isr(m_gpio, MRAA_GPIO_EDGE_RISING, 
                isr, arg);
  m_isrInstalled = true;
}

void MMA7660::uninstallISR()
{
  if (!m_isrInstalled)
    return;

  mraa_gpio_isr_exit(m_gpio);
  m_isrInstalled = false;
  mraa_gpio_close(m_gpio);
}

bool MMA7660::setInterruptBits(uint8_t ibits)
{
  return writeByte(REG_INTSU, ibits);
}

bool MMA7660::setSampleRate(MMA7660_AUTOSLEEP_T sr)
{
  return writeByte(REG_SR, sr);
}

void MMA7660::getAcceleration(float *ax, float *ay, float *az)
{
  int x, y, z;

  getRawValues(&x, &y, &z);

  // 21.33, typical counts/g

  *ax = x/21.33;
  *ay = y/21.33;
  *az = z/21.33;
}

#ifdef JAVACALLBACK
float *MMA7660::getAcceleration()
{
  float *values = new float[3];
  getAcceleration(&values[0], &values[1], &values[2]);
  return values;
}
#endif

