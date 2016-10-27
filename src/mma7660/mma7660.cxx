/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include "mma7660.hpp"

using namespace upm;
using namespace std;


MMA7660::MMA7660(int bus, uint8_t address) :
    m_mma7660(mma7660_init(bus, address))
{
    if (!m_mma7660)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_init() failed");
}

MMA7660::~MMA7660()
{
    mma7660_close(m_mma7660);
}

bool MMA7660::writeByte(uint8_t reg, uint8_t byte)
{
    if (mma7660_write_byte(m_mma7660, reg, byte))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_write_byte() failed");
    return true;
}

uint8_t MMA7660::readByte(uint8_t reg)
{
    uint8_t val = 0;
    if (mma7660_read_byte(m_mma7660, reg, &val))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_read_byte() failed");

    return val;
}

void MMA7660::getRawValues(int *x, int *y, int *z)
{
    if (mma7660_get_raw_values(m_mma7660, x, y, z))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_get_raw_values() failed");
}

void MMA7660::setModeActive()
{
    if (mma7660_set_mode_active(m_mma7660))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_set_mode_active() failed");
}

void MMA7660::setModeStandby()
{
    if (mma7660_set_mode_standby(m_mma7660))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_set_mode_standby() failed");
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

    int val;
    if (mma7660_get_verified_axis(m_mma7660, axis, &val))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_get_verified_axis() failed");

    return val;
}

// read the tilt register, verifying it's validity
uint8_t MMA7660::getVerifiedTilt()
{
    uint8_t val;
    if (mma7660_get_verified_tilt(m_mma7660, &val))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_get_verified_axis() failed");

    return val;
}

uint8_t MMA7660::tiltBackFront()
{
    uint8_t val;
    if (mma7660_tilt_back_front(m_mma7660, &val))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_tilt_back_front() failed");

    return val;
}

uint8_t MMA7660::tiltLandscapePortrait()
{
    uint8_t val;
    if (mma7660_tilt_landscape_portrait(m_mma7660, &val))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_tilt_landscape_portrait() failed");

    return val;
}

bool MMA7660::tiltTap()
{
    bool val;
    if (mma7660_tilt_tap(m_mma7660, &val))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_tilt_tap_portrait() failed");

    return val;
}

bool MMA7660::tiltShake()
{
    bool val;
    if (mma7660_tilt_shake(m_mma7660, &val))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_tilt_tap_shake() failed");

    return val;
}

void MMA7660::installISR(int pin, void (*isr)(void *), void *arg)
{
    if (mma7660_install_isr(m_mma7660, pin, isr, arg))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_install_isr() failed");
}

void MMA7660::uninstallISR()
{
    mma7660_uninstall_isr(m_mma7660);
}

bool MMA7660::setInterruptBits(uint8_t ibits)
{
    if (mma7660_set_interrupt_bits(m_mma7660, ibits))
        return false;
    else
        return true;
}

bool MMA7660::setSampleRate(MMA7660_AUTOSLEEP_T sr)
{
    if (mma7660_set_sample_rate(m_mma7660, sr))
        return false;
    else
        return true;
}

void MMA7660::getAcceleration(float *ax, float *ay, float *az)
{
    if (mma7660_get_acceleration(m_mma7660, ax, ay, az))
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": mma7660_get_acceleration() failed");
}

