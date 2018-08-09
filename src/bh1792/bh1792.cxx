/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <stdexcept>
#include "bh1792.hpp"

using namespace upm;

BH1792::BH1792(int bus, int addr) : m_bh1792(bh1792_init(bus, addr))
{
    if(!m_bh1792)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_init() failed");
}

BH1792::~BH1792()
{
    bh1792_close(m_bh1792);
}

void BH1792::CheckWhoAmI()
{
    if(bh1792_check_who_am_i(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_check_who_am_i() failed");
}

bool BH1792::IsEnabled()
{
    return bh1792_is_enabled(m_bh1792);
}

void BH1792::SoftReset()
{
    if(bh1792_soft_reset(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_soft_reset() failed");
}

float BH1792::GetMeasurementTimeMS()
{
    float meas_time;

    if(bh1792_get_meas_time_ms(m_bh1792, &meas_time) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_meas_time_ms() failed");

    return meas_time;
}

void BH1792::SetGreenLedsCurrent(uint16_t current)
{
    uint8_t status;

    status = bh1792_set_green_leds_current(m_bh1792, current);
    if(status == UPM_ERROR_OUT_OF_RANGE) {
        throw std::range_error(std::string(__FUNCTION__) +
                "bh1792_set_green_leds_current() failed, current not in range");
    } else if(status != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_set_green_leds_current() failed");
    }
}

int BH1792::GetGreenLedsCurrent()
{
    uint8_t current;

    if(bh1792_get_green_leds_current(m_bh1792, &current) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_green_leds_current() failed");

    return (int)current;
}

void BH1792::SetIrLedCurrent(uint16_t current)
{
    uint8_t status;

    status = bh1792_set_ir_led_current(m_bh1792, current);
    if(status == UPM_ERROR_OUT_OF_RANGE) {
        throw std::range_error(std::string(__FUNCTION__) +
                "bh1792_set_ir_led_current() failed, current not in range");
    } else if(status != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_set_ir_led_current() failed");
    }
}

int BH1792::GetIrLedCurrent()
{
    uint8_t current;

    if(bh1792_get_ir_led_current(m_bh1792, &current) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_ir_led_current() failed");

    return (int)current;
}

void BH1792::SetIrThreshold(uint16_t threshold)
{
    if(bh1792_set_ir_threshold(m_bh1792, threshold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_set_ir_threshold() failed");
}

int BH1792::GetIrThreshold()
{
    uint16_t threshold;

    if(bh1792_get_ir_threshold(m_bh1792, &threshold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_ir_threshold() failed");

    return (int)threshold;
}

int BH1792::GetFifoSize()
{
    uint8_t size;

    if(bh1792_get_fifo_size(m_bh1792, &size) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_fifo_size() failed");

    return (int)size;
}

void BH1792::DisableInterrupt()
{
    if(bh1792_disable_interrupt(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_disable_interrupt() failed");
}

void BH1792::ClearInterrupt()
{
    if(bh1792_clear_interrupt(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_clear_interrupt() failed");
}

void BH1792::StartMeasurement()
{
    if(bh1792_start_measurement(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_start_measurement() failed");
}

void BH1792::StopMeasurement()
{
    if(bh1792_stop_measurement(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_stop_measurement() failed");
}

void BH1792::RestartMeasurement()
{
    if(bh1792_restart_measurement(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_restart_measurement() failed");
}

std::vector<std::vector<int>> BH1792::GetFifoData()
{
    uint16_t fifo_off[FIFO_WATERMARK], fifo_on[FIFO_WATERMARK];

    if(bh1792_get_fifo_data(m_bh1792, fifo_off, fifo_on) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_fifo_data() failed");

    std::vector<int> item;
    std::vector<std::vector<int>> result;
    item.reserve(2);
    result.reserve(FIFO_WATERMARK);
    for(int i = 0; i < FIFO_WATERMARK; i++) {
        item.clear();
        item.push_back(fifo_off[i]);
        item.push_back(fifo_on[i]);
        result.push_back(item);
    }

    return result;
}

void BH1792::DiscardFifoData()
{
    if(bh1792_discard_fifo_data(m_bh1792) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_discard_fifo_data() failed");
}

std::vector<int> BH1792::GetGreenData()
{
    uint16_t green_off, green_on;

    if(bh1792_get_green_data(m_bh1792, &green_off, &green_on) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_green_data() failed");

    std::vector<int> result;
    result.reserve(2);
    result.push_back(green_off);
    result.push_back(green_on);

    return result;
}

std::vector<int> BH1792::GetIrData()
{
    uint16_t ir_off, ir_on;

    if(bh1792_get_ir_data(m_bh1792, &ir_off, &ir_on) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_get_ir_data() failed");

    std::vector<int> result;
    result.reserve(2);
    result.push_back(ir_off);
    result.push_back(ir_on);

    return result;
}

void BH1792::EnableSyncMode(uint16_t measFreq, uint16_t green_current)
{
    uint8_t status;

    status = bh1792_enable_sync_mode(m_bh1792, measFreq, green_current);
    if(status == UPM_ERROR_INVALID_PARAMETER) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                "bh1792_enable_sync_mode() failed, invalid measurement frequncy");
    } else if(status != UPM_SUCCESS) {
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_enable_sync_mode() failed");
    }
}

void BH1792::EnableNonSyncMode(uint16_t ir_current, uint16_t threshold)
{
    if(bh1792_enable_non_sync_mode(m_bh1792, ir_current, threshold) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_enable_non_sync_mode() failed");
}

void BH1792::EnableSingleMode(LED_TYPES led_type, uint16_t current)
{
    if(bh1792_enable_single_mode(m_bh1792, led_type, current) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_enable_single_mode() failed");
}

void BH1792::InstallISR(mraa_gpio_edge_t edge, int pin, void (*isr)(void *), void *isr_args)
{
    if(bh1792_install_isr(m_bh1792, edge, pin, isr, isr_args) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_install_isr() failed");
}

void BH1792::InstallISR(int pin, void (*isr)(void *), void *isr_args)
{
    if(bh1792_install_isr_falling_edge(m_bh1792, pin, isr, isr_args) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_install_isr_falling_edge() failed");
}

void BH1792::RemoveISR()
{
    bh1792_remove_isr(m_bh1792);
}

std::string BH1792::RegistersDump()
{
    char dump[255];
    std::string dumpStr;

    if(bh1792_registers_dump(m_bh1792, dump) != UPM_SUCCESS)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                "bh1792_registers_dump() failed");
    dumpStr = dump;
    return dumpStr;
}
