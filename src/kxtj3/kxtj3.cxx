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
#include <stdexcept>
#include <string>

#include "kxtj3.hpp"

using namespace upm;

KXTJ3::KXTJ3(int bus, uint8_t addr) : m_kxtj3(kxtj3_init(bus, addr))
{
    if (!m_kxtj3)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_init() failed");
}

KXTJ3::~KXTJ3()
{
    kxtj3_close(m_kxtj3);
}

void KXTJ3::SensorInit(KXTJ3_ODR_T odr, KXTJ3_RESOLUTION_T resolution, KXTJ3_G_RANGE_T g_range)
{
    if (kxtj3_sensor_init(m_kxtj3, odr, resolution, g_range) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_sensor_init() failed");
}

uint8_t KXTJ3::GetWhoAmI()
{
    uint8_t who_am_i;
    if (kxtj3_get_who_am_i(m_kxtj3, &who_am_i) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_get_who_am_i() failed");

    return who_am_i;
}

void KXTJ3::SensorActive()
{
    if (kxtj3_set_sensor_active(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_sensor_active() failed");
}

void KXTJ3::SensorStandby()
{
    if (kxtj3_set_sensor_standby(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_sensor_standby() failed");
}

void KXTJ3::SetGRange(KXTJ3_G_RANGE_T g_range)
{
    if (kxtj3_set_g_range(m_kxtj3, g_range) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_g_range() failed");
}

void KXTJ3::SetResolution(KXTJ3_RESOLUTION_T resolution)
{
    if (kxtj3_set_resolution(m_kxtj3, resolution) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_resolution() failed");
}

void KXTJ3::SetOdr(KXTJ3_ODR_T odr)
{
    if (kxtj3_set_odr(m_kxtj3, odr) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_odr() failed");
}

void KXTJ3::SetOdrForWakeup(KXTJ3_ODR_WAKEUP_T odr)
{
    if (kxtj3_set_odr_wakeup_function(m_kxtj3, odr) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_odr_wakeup_function() failed");
}

void KXTJ3::SelfTestDigitalCommunication()
{
    if (kxtj3_self_test_digital_communication(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_self_test_digital_communication() failed");
}

void KXTJ3::SensorSelfTest()
{
    if (kxtj3_sensor_self_test(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_sensor_self_test() failed");
}

void KXTJ3::SensorSoftwareReset()
{
    if (kxtj3_sensor_software_reset(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_sensor_software_reset() failed");
}

std::vector<float> KXTJ3::GetAccelerationRawVector()
{
    std::vector<float> xyz(3);
    if (kxtj3_get_acceleration_data_raw(m_kxtj3, &xyz[0], &xyz[1], &xyz[2]) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_get_acceleration_data_raw() failed");

    return xyz;
}

std::vector<float> KXTJ3::GetAccelerationVector()
{
    std::vector<float> xyz(3);
    if (kxtj3_get_acceleration_data(m_kxtj3, &xyz[0], &xyz[1], &xyz[2]) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_get_acceleration_data() failed");

    return xyz;
}

float KXTJ3::GetAccelerationSamplePeriod()
{
    return kxtj3_get_acceleration_sampling_period(m_kxtj3);
}

float KXTJ3::GetWakeUpSamplePeriod()
{
    return kxtj3_get_wakeup_sampling_period(m_kxtj3);
}

void KXTJ3::EnableDataReadyInterrupt()
{
    if (kxtj3_enable_data_ready_interrupt(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_enable_data_ready_interrupt() failed");
}

void KXTJ3::DisableDataReadyInterrupt()
{
    if (kxtj3_disable_data_ready_interrupt(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_disable_data_ready_interrupt() failed");
}

void KXTJ3::EnableWakeUpInterrupt()
{
    if (kxtj3_enable_wakeup_interrupt(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_enable_wakeup_interrupt() failed");
}

void KXTJ3::DisableWakeUpInterrupt()
{
    if (kxtj3_disable_wakeup_interrupt(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_disable_wakeup_interrupt() failed");
}

void KXTJ3::EnableInterruptPin(KXTJ3_INTERRUPT_POLARITY_T polarity, KXTJ3_INTERRUPT_RESPONSE_T response_type)
{
    if (kxtj3_enable_interrupt_pin(m_kxtj3, polarity, response_type) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_enable_interrupt_pin() failed");
}

void KXTJ3::DisableInterruptPin()
{
    if (kxtj3_disable_interrupt_pin(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_disable_interrupt_pin() failed");
}

void KXTJ3::SetInterruptPolarity(KXTJ3_INTERRUPT_POLARITY_T polarity)
{
    if (kxtj3_set_interrupt_polarity(m_kxtj3, polarity) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_interrupt_polarity() failed");
}

void KXTJ3::SetInerruptResponse(KXTJ3_INTERRUPT_RESPONSE_T response_type)
{
    if (kxtj3_set_interrupt_response(m_kxtj3, response_type) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_interrupt_response() failed");
}

bool KXTJ3::GetInterruptStatus()
{
    return kxtj3_get_interrupt_status(m_kxtj3);
}

uint8_t KXTJ3::ReadInterruptSource1()
{
    uint8_t reg_value;
    if (kxtj3_read_interrupt_source1_reg(m_kxtj3, &reg_value) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_read_interrupt_source1_reg() failed");

    return reg_value;
}

KXTJ3_INTERRUPT_SOURCE_T KXTJ3::GetInterruptSource()
{
    return kxtj3_get_interrupt_source(m_kxtj3);
}

void KXTJ3::InstallIsr(mraa_gpio_edge_t edge, int pin, void (*isr)(void *), void *isr_args)
{
    if (kxtj3_install_isr(m_kxtj3, edge, pin, isr, isr_args) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_install_isr() failed");
}

void KXTJ3::UninstallIsr()
{
    kxtj3_uninstall_isr(m_kxtj3);
}

void KXTJ3::ClearInterrupt()
{
    if (kxtj3_clear_interrupt_information(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_clear_interrupt_information() failed");
}

void KXTJ3::EnableWakeUpSingleAxisDirection(KXTJ3_WAKEUP_SOURCE_T axis)
{
    if (kxtj3_enable_wakeup_single_axis_direction(m_kxtj3, axis) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_enable_wakeup_single_axis_direction() failed");
}

void KXTJ3::DisableWakeUpSingleAxisDirection(KXTJ3_WAKEUP_SOURCE_T axis)
{
    if (kxtj3_disable_wakeup_single_axis_direction(m_kxtj3, axis) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_disable_wakeup_single_axis_direction() failed");
}

kxtj3_wakeup_axes KXTJ3::GetWakeUpAxisDirection()
{
    return kxtj3_get_wakeup_axis_and_direction(m_kxtj3);
}

void KXTJ3::EnableWakeUpLatch()
{
    if (kxtj3_enable_wakeup_latch(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_enable_wakeup_latch() failed");
}

void KXTJ3::DisableWakeUpLatch()
{
    if (kxtj3_disable_wakeup_latch(m_kxtj3) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_disable_wakeup_latch() failed");
}

void KXTJ3::SetWakeUpMotionCounter(uint8_t count)
{
    if (kxtj3_set_wakeup_motion_counter(m_kxtj3, count) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_wakeup_motion_counter() failed");
}

void KXTJ3::SetWakeUpMotionTime(float desired_time)
{
    if (kxtj3_set_wakeup_motion_time(m_kxtj3, desired_time) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_wakeup_motion_time() failed");
}

float KXTJ3::GetWakeUpMotionTime()
{
    float out_time;
    if (kxtj3_get_wakeup_motion_time(m_kxtj3, &out_time) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_get_wakeup_motion_time() failed");

    return out_time;
}

void KXTJ3::SetWakeUpNonActivityCounter(uint8_t count)
{
    if (kxtj3_set_wakeup_non_activity_counter(m_kxtj3, count) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_wakeup_non_activity_counter() failed");
}

void KXTJ3::SetWakeUpNonActivityTime(float desired_time)
{
    if (kxtj3_set_wakeup_non_activity_time(m_kxtj3, desired_time) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_wakeup_non_activity_time() failed");
}

float KXTJ3::GetWakeUpNonActivityTime()
{
    float out_time;
    if (kxtj3_get_wakeup_non_activity_time(m_kxtj3, &out_time) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_get_wakeup_non_activity_time() failed");

    return out_time;
}

void KXTJ3::SetWakeUpThresholdCounter(uint16_t count)
{
    if (kxtj3_set_wakeup_threshold_counter(m_kxtj3, count) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_wakeup_threshold_counter() failed");
}

void KXTJ3::SetWakeUpThresholdGRange(float g_threshold)
{
    if (kxtj3_set_wakeup_threshold_g_value(m_kxtj3, g_threshold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_set_wakeup_threshold_g_value() failed");
}

float KXTJ3::GetWakeUpThresholdGRange()
{
    float out_threshold;
    if (kxtj3_get_wakeup_threshold(m_kxtj3, &out_threshold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) + "kxtj3_get_wakeup_threshold() failed");

    return out_threshold;
}