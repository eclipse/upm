/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-17 Intel Corporation.
 *
 * The MIT License
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

#include "bno055.hpp"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit

static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

BNO055::BNO055(int bus, uint8_t addr) :
    m_bno055(bno055_init(bus, addr))
{
    if (!m_bno055)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_init() failed");
}

BNO055::~BNO055()
{
    bno055_close(m_bno055);
}

void BNO055::update()
{
    if (bno055_update(m_bno055))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_update() failed");
}

uint8_t BNO055::readReg(uint8_t reg)
{
    return bno055_read_reg(m_bno055, reg);
}

void BNO055::readRegs(uint8_t reg, uint8_t *buffer, int len)
{
    if (bno055_read_regs(m_bno055, reg, buffer, len))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_read_regs() failed");
}

void BNO055::writeReg(uint8_t reg, uint8_t val)
{
    if (bno055_write_reg(m_bno055, reg, val))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_write_reg() failed");
}

void BNO055::writeRegs(uint8_t reg, uint8_t *buffer, int len)
{
    if (bno055_write_regs(m_bno055, reg, buffer, len))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_write_regs() failed");
}

uint8_t BNO055::getChipID()
{
    return bno055_get_chip_id(m_bno055);
}

uint8_t BNO055::getACCID()
{
    return bno055_get_acc_id(m_bno055);
}

uint8_t BNO055::getMAGID()
{
    return bno055_get_mag_id(m_bno055);
}

uint8_t BNO055::getGYRID()
{
    return bno055_get_gyr_id(m_bno055);
}

uint16_t BNO055::getSWRevID()
{
    return bno055_get_sw_revision(m_bno055);
}

uint8_t BNO055::getBootLoaderID()
{
    return bno055_get_bootloader_id(m_bno055);
}

void BNO055::setPage(uint8_t page, bool force)
{
    if (bno055_set_page(m_bno055, page, force))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_page() failed");
}

void BNO055::setClockExternal(bool extClock)
{
    bno055_set_clock_external(m_bno055, extClock);
}

void BNO055::setTemperatureSource(BNO055_TEMP_SOURCES_T src)
{
    bno055_set_temperature_source(m_bno055, src);
}

void BNO055::setAccelerometerUnits(bool mg)
{
    bno055_set_accelerometer_units(m_bno055, mg);
}

void BNO055::setGyroscopeUnits(bool radians)
{
    bno055_set_gyroscope_units(m_bno055, radians);
}

void BNO055::setEulerUnits(bool radians)
{
    bno055_set_euler_units(m_bno055, radians);
}

void BNO055::setOperationMode(BNO055_OPERATION_MODES_T mode)
{
    bno055_set_operation_mode(m_bno055, mode);
}

void BNO055::getCalibrationStatus(int *mag, int *acc, int *gyr, int *sys)
{
    bno055_get_calibration_status(m_bno055, mag, acc, gyr, sys);
}

vector<int> BNO055::getCalibrationStatus()
{
    int v[4]; // mag, acc, gyr, sys;

    getCalibrationStatus(&v[0], &v[1], &v[2], &v[3]);
    return vector<int>(v, v+4);
}

bool BNO055::isFullyCalibrated()
{
    return bno055_is_fully_calibrated(m_bno055);
}

void BNO055::resetSystem()
{
    bno055_reset_system(m_bno055);
}

void BNO055::resetInterruptStatus()
{
    bno055_reset_interrupt_status(m_bno055);
}

uint8_t BNO055::getInterruptStatus()
{
    return bno055_get_interrupt_status(m_bno055);
}

uint8_t BNO055::getInterruptEnable()
{
    return bno055_get_interrupt_enable(m_bno055);
}

void BNO055::setInterruptEnable(uint8_t enables)
{
    return bno055_set_interrupt_enable(m_bno055, enables);
}

uint8_t BNO055::getInterruptMask()
{
    return bno055_get_interrupt_mask(m_bno055);
}

void BNO055::setInterruptMask(uint8_t mask)
{
    return bno055_set_interrupt_mask(m_bno055, mask);
}

BNO055_SYS_STATUS_T BNO055::getSystemStatus()
{
    return bno055_get_system_status(m_bno055);
}

BNO055_SYS_ERR_T BNO055::getSystemError()
{
    return bno055_get_system_error(m_bno055);
}

std::vector<uint8_t> BNO055::readCalibrationData()
{
    uint8_t calibrationData[BNO055_CALIBRATION_DATA_SIZE];

    if (bno055_read_calibration_data(m_bno055, calibrationData,
                                     BNO055_CALIBRATION_DATA_SIZE))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_read_calibration_data() failed");

    return vector<uint8_t>(calibrationData,
                           calibrationData+BNO055_CALIBRATION_DATA_SIZE);
}

void BNO055::writeCalibrationData(vector<uint8_t> calibrationData)
{
    if (bno055_write_calibration_data(m_bno055, calibrationData.data(),
                                      calibrationData.size()))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_write_calibration_data() failed");
}

float BNO055::getTemperature(bool fahrenheit)
{
    float temperature = bno055_get_temperature(m_bno055);

    if (fahrenheit)
        return c2f(temperature);
    else
        return temperature;
}

void BNO055::getEulerAngles(float *heading, float *roll, float *pitch)
{
    bno055_get_euler_angles(m_bno055, heading, roll, pitch);
}

vector<float> BNO055::getEulerAngles()
{
    float v[3];
    getEulerAngles(&v[0], &v[1], &v[2]);
    return vector<float>(v, v+3);
}

void BNO055::getQuaternions(float *w, float *x, float *y, float *z)
{
    bno055_get_quaternions(m_bno055, w, x, y, z);
}

vector<float> BNO055::getQuaternions()
{
    float v[4];
    getQuaternions(&v[0], &v[1], &v[2], &v[3]);
    return vector<float>(v, v+4);
}

void BNO055::getLinearAcceleration(float *x, float *y, float *z)
{
    bno055_get_linear_acceleration(m_bno055, x, y, z);
}

vector<float> BNO055::getLinearAcceleration()
{
    float v[3];
    getLinearAcceleration(&v[0], &v[1], &v[2]);
    return vector<float>(v, v+3);
}

void BNO055::getGravityVectors(float *x, float *y, float *z)
{
    bno055_get_gravity_vectors(m_bno055, x, y, z);
}

vector<float> BNO055::getGravityVectors()
{
    static float v[3];
    getGravityVectors(&v[0], &v[1], &v[2]);
    return vector<float>(v, v+3);
}

void BNO055::getAccelerometer(float *x, float *y, float *z)
{
    bno055_get_accelerometer(m_bno055, x, y, z);
}

vector<float> BNO055::getAccelerometer()
{
    static float v[3];
    getAccelerometer(&v[0], &v[1], &v[2]);
    return vector<float>(v, v+3);
}

void BNO055::getMagnetometer(float *x, float *y, float *z)
{
    bno055_get_magnetometer(m_bno055, x, y, z);
}

vector<float> BNO055::getMagnetometer()
{
    float v[3];
    getMagnetometer(&v[0], &v[1], &v[2]);
    return vector<float>(v, v+3);
}

void BNO055::getGyroscope(float *x, float *y, float *z)
{
    bno055_get_gyroscope(m_bno055, x, y, z);
}

vector<float> BNO055::getGyroscope()
{
    float v[3];
    getGyroscope(&v[0], &v[1], &v[2]);
    return vector<float>(v, v+3);
}

void BNO055::setAccelerationConfig(BNO055_ACC_RANGE_T range,
                                   BNO055_ACC_BW_T bw,
                                   BNO055_ACC_PWR_MODE_T pwr)
{
    bno055_set_acceleration_config(m_bno055, range, bw, pwr);
}

void BNO055::setMagnetometerConfig(BNO055_MAG_ODR_T odr,
                                   BNO055_MAG_OPR_T opr,
                                   BNO055_MAG_POWER_T pwr)
{
    bno055_set_magnetometer_config(m_bno055, odr, opr, pwr);
}

void BNO055::setGyroscopeConfig(BNO055_GYR_RANGE_T range,
                                BNO055_GYR_BW_T bw,
                                BNO055_GYR_POWER_MODE_T pwr)
{
    bno055_set_gyroscope_config(m_bno055, range, bw, pwr);
}

void BNO055::installISR(int gpio, mraa_gpio_edge_t level,
                        void (*isr)(void *), void *arg)
{
    if (bno055_install_isr(m_bno055, gpio, level, isr, arg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_install_isr() failed");
}

void BNO055::uninstallISR()
{
    bno055_uninstall_isr(m_bno055);
}
