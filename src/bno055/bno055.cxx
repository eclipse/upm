/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-17 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdexcept>

#include "bno055.hpp"
#include "upm_string_parser.hpp"

using namespace upm;
using namespace std;

// conversion from Celsius to Fahrenheit

static float c2f(float c)
{
    return (c * (9.0 / 5.0) + 32.0);
}

BNO055::BNO055(int bus, uint8_t addr) :
    m_bno055(bno055_init(bus, addr, nullptr))
{
    if (!m_bno055)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_init() failed");
}

BNO055::BNO055(std::string initStr) : mraaIo(initStr)
{
    mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
    std::vector<std::string> upmTokens;

    if (mraaIo.getLeftoverStr() != "") {
        upmTokens = UpmStringParser::parse(mraaIo.getLeftoverStr());
    }

    m_bno055 = bno055_init(0, 0, descs);

    if(!m_bno055)
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_init() failed");

    std::string::size_type sz, prev_sz;
    for(std::string tok : upmTokens)
    {
        if(tok.substr(0, 9) == "writeReg:")
        {
            uint8_t reg = std::stoul(tok.substr(9), &sz, 0);
            tok = tok.substr(9);
            uint8_t val = std::stoul(tok.substr(sz + 1), nullptr, 0);
            writeReg(reg, val);
        }
        if(tok.substr(0, 8) == "setPage:")
        {
            uint8_t page = std::stoul(tok.substr(8), &sz, 0);
            tok = tok.substr(8);
            bool force = std::stoi(tok.substr(sz + 1), nullptr, 0);
            setPage(page, force);
        }
        if(tok.substr(0, 17) == "setClockExternal:")
        {
            bool extClock = std::stoi(tok.substr(17), nullptr, 0);
            setClockExternal(extClock);
        }
        if(tok.substr(0, 21) == "setTemperatureSource:")
        {
            BNO055_TEMP_SOURCES_T src = (BNO055_TEMP_SOURCES_T)std::stoi(tok.substr(21), nullptr, 0);
            setTemperatureSource(src);
        }
        if(tok.substr(0, 22) == "setAccelerometerUnits:")
        {
            bool mg = std::stoi(tok.substr(22), nullptr, 0);
            setAccelerometerUnits(mg);
        }
        if(tok.substr(0, 18) == "setGyroscopeUnits:")
        {
            bool radians = std::stoi(tok.substr(18), nullptr, 0);
            setGyroscopeUnits(radians);
        }
        if(tok.substr(0, 14) == "setEulerUnits:")
        {
            bool radians = std::stoi(tok.substr(14), nullptr, 0);
            setEulerUnits(radians);
        }
        if(tok.substr(0, 17) == "setOperationMode:")
        {
            BNO055_OPERATION_MODES_T mode = (BNO055_OPERATION_MODES_T)std::stoi(tok.substr(17), nullptr, 0);
            setOperationMode(mode);
        }
        if(tok.substr(0, 19) == "setInterruptEnable:")
        {
            uint8_t enables = std::stoul(tok.substr(19), nullptr, 0);
            setInterruptEnable(enables);
        }
        if(tok.substr(0, 17) == "setInterruptMask:")
        {
            uint8_t enables = std::stoul(tok.substr(17), nullptr, 0);
            setInterruptMask(enables);
        }
        if(tok.substr(0, 22) == "setAccelerationConfig:")
        {
            BNO055_ACC_RANGE_T range = (BNO055_ACC_RANGE_T)std::stoi(tok.substr(22), &sz, 0);
            tok = tok.substr(22);
            prev_sz = sz;
            BNO055_ACC_BW_T bw = (BNO055_ACC_BW_T)std::stoi(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            BNO055_ACC_PWR_MODE_T pwr = (BNO055_ACC_PWR_MODE_T)std::stoi(tok.substr(sz + 1), nullptr, 0);
            setAccelerationConfig(range, bw, pwr);
        }
        if(tok.substr(0, 22) == "setMagnetometerConfig:")
        {
            BNO055_MAG_ODR_T odr = (BNO055_MAG_ODR_T)std::stoi(tok.substr(22), &sz, 0);
            tok = tok.substr(22);
            prev_sz = sz;
            BNO055_MAG_OPR_T opr = (BNO055_MAG_OPR_T)std::stoi(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            BNO055_MAG_POWER_T pwr = (BNO055_MAG_POWER_T)std::stoi(tok.substr(sz + 1), nullptr, 0);
            setMagnetometerConfig(odr, opr, pwr);
        }
        if(tok.substr(0, 19) == "setGyroscopeConfig:")
        {
            BNO055_GYR_RANGE_T range = (BNO055_GYR_RANGE_T)std::stoi(tok.substr(19), &sz, 0);
            tok = tok.substr(22);
            prev_sz = sz;
            BNO055_GYR_BW_T bw = (BNO055_GYR_BW_T)std::stoi(tok.substr(prev_sz + 1), &sz, 0);
            tok = tok.substr(prev_sz + 1);
            BNO055_GYR_POWER_MODE_T pwr = (BNO055_GYR_POWER_MODE_T)std::stoi(tok.substr(sz + 1), nullptr, 0);
            setGyroscopeConfig(range, bw, pwr);
        }
    }
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
    uint8_t rv = 0;
    if (bno055_read_reg(m_bno055, reg, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_read_reg() failed");

    return rv;
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
    uint8_t rv = 0;
    if (bno055_get_chip_id(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_chip_id() failed");

    return rv;
}

uint8_t BNO055::getACCID()
{
    uint8_t rv = 0;
    if (bno055_get_acc_id(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_acc_id() failed");

    return rv;
}

uint8_t BNO055::getMAGID()
{
    uint8_t rv = 0;
    if (bno055_get_mag_id(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_mag_id() failed");

    return rv;
}

uint8_t BNO055::getGYRID()
{
    uint8_t rv = 0;
    if (bno055_get_gyr_id(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_gyr_id() failed");

    return rv;
}

uint16_t BNO055::getSWRevID()
{
    uint16_t rv = 0;
    if (bno055_get_sw_revision(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_sw_revision() failed");

    return rv;
}

uint8_t BNO055::getBootLoaderID()
{
    uint8_t rv = 0;
    if (bno055_get_bootloader_id(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_bootloader_id() failed");

    return rv;
}

void BNO055::setPage(uint8_t page, bool force)
{
    if (bno055_set_page(m_bno055, page, force))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_page() failed");
}

void BNO055::setClockExternal(bool extClock)
{
    if (bno055_set_clock_external(m_bno055, extClock))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_clock_external() failed");
}

void BNO055::setTemperatureSource(BNO055_TEMP_SOURCES_T src)
{
    if (bno055_set_temperature_source(m_bno055, src))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_temperature_source() failed");
}

void BNO055::setAccelerometerUnits(bool mg)
{
    if (bno055_set_accelerometer_units(m_bno055, mg))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_accelerometer_units() failed");
}

void BNO055::setGyroscopeUnits(bool radians)
{
    if (bno055_set_gyroscope_units(m_bno055, radians))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_gyroscope_units() failed");
}

void BNO055::setEulerUnits(bool radians)
{
    if (bno055_set_euler_units(m_bno055, radians))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_euler_units() failed");
}

void BNO055::setOperationMode(BNO055_OPERATION_MODES_T mode)
{
    if (bno055_set_operation_mode(m_bno055, mode))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_operation_mode() failed");
}

void BNO055::getCalibrationStatus(int *mag, int *acc, int *gyr, int *sys)
{
    if (bno055_get_calibration_status(m_bno055, mag, acc, gyr, sys))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_calibration_status() failed");
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
    if (bno055_reset_system(m_bno055))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_reset_system() failed");
}

void BNO055::resetInterruptStatus()
{
    if (bno055_reset_interrupt_status(m_bno055))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": reset_interrupt_status() failed");
}

uint8_t BNO055::getInterruptStatus()
{
    uint8_t rv = 0;
    if (bno055_get_interrupt_status(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055get_interrupt_status() failed");

    return rv;
}

uint8_t BNO055::getInterruptEnable()
{
    uint8_t rv = 0;
    if (bno055_get_interrupt_enable(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_interrupt_enable() failed");

    return rv;
}

void BNO055::setInterruptEnable(uint8_t enables)
{
    if (bno055_set_interrupt_enable(m_bno055, enables))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_interrupt_enable() failed");
}

uint8_t BNO055::getInterruptMask()
{
    uint8_t rv = 0;
    if (bno055_get_interrupt_mask(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_interrupt_mask() failed");

    return rv;
}

void BNO055::setInterruptMask(uint8_t mask)
{
    if (bno055_set_interrupt_mask(m_bno055, mask))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_interrupt_mask() failed");
}

BNO055_SYS_STATUS_T BNO055::getSystemStatus()
{
    BNO055_SYS_STATUS_T rv;
    if (bno055_get_system_status(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_system_status() failed");

    return rv;
}

BNO055_SYS_ERR_T BNO055::getSystemError()
{
    BNO055_SYS_ERR_T rv;
    if (bno055_get_system_error(m_bno055, &rv))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_get_system_error() failed");

    return rv;
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

std::vector<float> BNO055::getAcceleration()
{
    std::vector<float> v(3);

    bno055_get_accelerometer(m_bno055, &v[0], &v[1], &v[2]);
    return v;
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
    if (bno055_set_acceleration_config(m_bno055, range, bw, pwr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_acceleration_config() failed");
}

void BNO055::setMagnetometerConfig(BNO055_MAG_ODR_T odr,
                                   BNO055_MAG_OPR_T opr,
                                   BNO055_MAG_POWER_T pwr)
{
    if (bno055_set_magnetometer_config(m_bno055, odr, opr, pwr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_magnetometer_config() failed");
}

void BNO055::setGyroscopeConfig(BNO055_GYR_RANGE_T range,
                                BNO055_GYR_BW_T bw,
                                BNO055_GYR_POWER_MODE_T pwr)
{
    if (bno055_set_gyroscope_config(m_bno055, range, bw, pwr))
        throw std::runtime_error(string(__FUNCTION__)
                                 + ": bno055_set_gyroscope_config() failed");
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
