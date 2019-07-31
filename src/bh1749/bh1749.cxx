/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include "bh1749.hpp"
#include "upm_string_parser.hpp"

#define DEFAULT_OP_MODE INT_JUDGE_1
#define DEFAULT_MEAS_TIME MEAS_240MS
#define DEFAULT_RGB_GAIN RGB_GAIN_1X
#define DEFAULT_IR_GAIN IR_GAIN_1X
#define DEFAULT_INT_SOURCE RED
#define DEFAULT_THRESHOLD_HIGH 511

using namespace upm;

void BH1749::CheckWhoAmI()
{
    if(bh1749_check_who_am_i(m_bh1749) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_check_who_am_i() failed");
}

BH1749::BH1749(int bus, int addr) : m_bh1749(bh1749_init(bus, addr))
{
    if(!m_bh1749)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_init() failed");
}

BH1749::BH1749(std::string initStr) : mraaIo(initStr)
{
  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
  std::vector<std::string> upmTokens;

  m_bh1749 = (bh1749_context)malloc(sizeof(struct _bh1749_context));
  if(!m_bh1749)
      throw std::runtime_error(std::string(__FUNCTION__) +
                              "bh1749_init() failed");

  m_bh1749->i2c = NULL;
  m_bh1749->interrupt = NULL;

  if(mraa_init() != MRAA_SUCCESS) {
      bh1749_close(m_bh1749);
      throw std::runtime_error(std::string(__FUNCTION__) +
                              "bh1749_init() failed");
  }

  if(!descs->i2cs) {
      bh1749_close(m_bh1749);
      throw std::runtime_error(std::string(__FUNCTION__)
                                + ": mraa_i2c_init() failed");
  } else {
      if( !(m_bh1749->i2c = descs->i2cs[0])) {
          bh1749_close(m_bh1749);
          throw std::runtime_error(std::string(__FUNCTION__)
                                    + ": mraa_i2c_init() failed");
      }
  }

  if(bh1749_check_who_am_i(m_bh1749) != UPM_SUCCESS)
      throw std::runtime_error(std::string(__FUNCTION__)
                                + ": bh1749_init() failed");

  m_bh1749->enabled = false;
  m_bh1749->isrEnabled = false;

  if(bh1749_sensor_init(m_bh1749, DEFAULT_OP_MODE, DEFAULT_MEAS_TIME,
      DEFAULT_RGB_GAIN, DEFAULT_IR_GAIN, DEFAULT_INT_SOURCE) != UPM_SUCCESS)
      throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_init() failed");
  bh1749_set_threshold_high(m_bh1749, DEFAULT_THRESHOLD_HIGH);
}

BH1749::~BH1749()
{
    bh1749_close(m_bh1749);
}

void BH1749::Enable()
{
    if(bh1749_enable(m_bh1749) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_enable() failed");
}

void BH1749::Disable()
{
    if(bh1749_disable(m_bh1749) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_disable() failed");
}

void BH1749::SensorInit(OPERATING_MODES opMode,
                        MEAS_TIMES measTime,
                        RGB_GAINS rgbGain,
                        IR_GAINS irGain,
                        INT_SOURCES intSource)
{
    if(bh1749_sensor_init(m_bh1749, opMode, measTime, rgbGain, irGain, intSource) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_sensor_init() failed");
}

void BH1749::SetOperatingMode(OPERATING_MODES opMode)
{
    if(bh1749_set_operating_mode(m_bh1749, opMode) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_set_operating_mode() failed");
}

int BH1749::GetOperatingMode()
{
    uint8_t opMode;
    if(bh1749_get_operating_mode(m_bh1749, &opMode) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_operating_mode() failed");
    return opMode;
}

void BH1749::SetMeasurementTime(MEAS_TIMES measTime)
{
    if(bh1749_set_measurement_time(m_bh1749, measTime) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_set_measurement_time() failed");
}

int BH1749::GetMeasurementTime()
{
    uint8_t measTime;
    if(bh1749_get_measurement_time(m_bh1749, &measTime) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_measurement_time() failed");
    return measTime;
}

void BH1749::SetRgbGain(RGB_GAINS rgbGain)
{
    if(bh1749_set_rgb_gain(m_bh1749, rgbGain) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_set_rgb_gain() failed");
}

int BH1749::GetRgbGain()
{
    uint8_t rgbGain;
    if(bh1749_get_rgb_gain(m_bh1749, &rgbGain) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_rgb_gain() failed");
    return rgbGain;
}

void BH1749::SetIrGain(IR_GAINS irGain)
{
    if(bh1749_set_ir_gain(m_bh1749, irGain) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_set_ir_gain() failed");
}

int BH1749::GetIrGain()
{
    uint8_t irGain;
    if(bh1749_get_ir_gain(m_bh1749, &irGain) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_ir_gain() failed");
    return irGain;
}

void BH1749::SetIntSource(INT_SOURCES intSource)
{
    if(bh1749_set_int_source(m_bh1749, intSource) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_set_int_source() failed");
}

char BH1749::GetInterruptSourceChar()
{
    char intSource = bh1749_get_interrupt_source_char(m_bh1749);
    if( intSource == ' ')
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_interrupt_source_char() failed");
    return intSource;
}

void BH1749::EnableInterrupt()
{
    if(bh1749_enable_interrupt(m_bh1749) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_enable_interrupt() failed");
}

void BH1749::DisableInterrupt()
{
    if(bh1749_disable_interrupt(m_bh1749) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_disable_interrupt() failed");
}

void BH1749::ResetInterrupt()
{
    if(bh1749_reset_interrupt(m_bh1749) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_reset_interrupt() failed");
}

bool BH1749::IsInterrupted()
{
    return bh1749_is_interrupted(m_bh1749);
}

bool BH1749::IsInterruptEnabled()
{
    return bh1749_is_interrupt_enabled(m_bh1749);
}

void BH1749::SoftReset()
{
    if(bh1749_soft_reset(m_bh1749) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_soft_reset() failed");
}

void BH1749::SetThresholdHigh(uint16_t threshold)
{
    if(bh1749_set_threshold_high(m_bh1749, threshold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_set_threshold_high() failed");
}

int BH1749::GetThresholdHigh()
{
    uint16_t threhold;
    if(bh1749_get_threshold_high(m_bh1749, &threhold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_threshold_high() failed");
    return threhold;
}

void BH1749::SetThresholdLow(uint16_t threshold)
{
    if(bh1749_set_threshold_low(m_bh1749, threshold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_set_threshold_low() failed");
}

int BH1749::GetThresholdLow()
{
    uint16_t threhold;
    if(bh1749_get_threshold_low(m_bh1749, &threhold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_threshold_low() failed");
    return threhold;
}

uint16_t BH1749::GetRed()
{
    uint16_t red;
    if(bh1749_get_red(m_bh1749, &red) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_red() failed");
    return red;
}

uint16_t BH1749::GetGreen()
{
    uint16_t green;
    if(bh1749_get_green(m_bh1749, &green) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_green() failed");
    return green;
}

uint16_t BH1749::GetBlue()
{
    uint16_t blue;
    if(bh1749_get_blue(m_bh1749, &blue) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_blue() failed");
    return blue;
}

uint16_t BH1749::GetIr()
{
    uint16_t ir;
    if(bh1749_get_ir(m_bh1749, &ir) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_ir() failed");
    return ir;
}

uint16_t BH1749::GetGeen2()
{
    uint16_t green2;
    if(bh1749_get_green2(m_bh1749, &green2) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_green2() failed");
    return green2;
}

std::vector<uint16_t> BH1749::GetMeasurements()
{
    uint16_t res[5];
    if(bh1749_get_measurements(m_bh1749, res) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_get_measurements() failed");

    std::vector<uint16_t> result(res, res + sizeof res / sizeof res[0]);
    return result;
}

void BH1749::InstallISR(mraa_gpio_edge_t edge, int pin, void (*isr)(void *), void *isr_args)
{
    if(bh1749_install_isr(m_bh1749, edge, pin, isr, isr_args) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_install_isr() failed");
}

void BH1749::RemoveISR()
{
    bh1749_remove_isr(m_bh1749);
}

std::string BH1749::RegistersDump()
{
    char dump[255];
    std::string dumpStr;

    if(bh1749_registers_dump(m_bh1749, dump) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1749_registers_dump() failed");
    dumpStr = dump;
    return dumpStr;
}
