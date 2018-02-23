/*
* The MIT License (MIT)
*
* Author: Samuli Rissanen <samuli.rissanen@hotmail.com>
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
#include <string>

#include <unistd.h>

#include "kx122.hpp"

using namespace upm;

KX122::KX122(int bus, int addr, int chip_select) : m_kx122(kx122_init(bus,addr,chip_select))
{
  if(!m_kx122){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_init() failed");
  }
}

KX122::~KX122()
{
  kx122_close(m_kx122);
}

void KX122::deviceInit(KX122_ODR_T odr, KX122_RES_T res, KX122_RANGE_T grange)
{
  if(kx122_device_init(m_kx122,odr,res,grange)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_device_init() failed");
  }
}

float KX122::getSamplePeriod()
{
  return kx122_get_sample_period(m_kx122);
}

void KX122::getWhoAmI(uint8_t *data)
{
  if(kx122_get_who_am_i(m_kx122,data)){
	throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_who_am_i failed");
  }
}
void KX122::getRawAccelerationData(float *x, float *y, float *z)
{
  if(kx122_get_acceleration_data_raw(m_kx122,x,y,z)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_accleration_data_raw failed");
  }
}

void KX122::getAccelerationData(float *x, float *y, float *z)
{
  if(kx122_get_acceleration_data(m_kx122,x,y,z)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_acceleration_data failed");
  }
}

void KX122::softwareReset()
{
  if(kx122_sensor_software_reset(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_sensor_software_reset failed");
  }
}

void KX122::enableIIR()
{
  if(kx122_enable_iir(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_enable_iir failed");
  }
}

void KX122::disableIIR()
{
  if(kx122_disable_iir(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_disable_iir failed");
  }
}

void KX122::selfTest()
{
  if(kx122_self_test(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_self_test failed");
  }
}

void KX122::setSensorStandby()
{
  if(kx122_set_sensor_standby(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_sensor_standby failed");
  }
}

void KX122::setSensorActive()
{
  if(kx122_set_sensor_active(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_sensor_active failed");
  }
}

void KX122::setODR(KX122_ODR_T odr)
{
  if(kx122_set_odr(m_kx122,odr)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_odr failed");
  }
}

void KX122::setGrange(KX122_RANGE_T grange)
{
  if(kx122_set_grange(m_kx122,grange)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_grange failed");
  }
}

void KX122::setResolution(KX122_RES_T res)
{
  if(kx122_set_resolution(m_kx122,res)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_resolution failed");
  }
}

void KX122::setBW(LPRO_STATE_T lpro)
{
  if(kx122_set_bw(m_kx122,lpro)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_bw failed");
  }
}

void KX122::setAverage(KX122_AVG_T avg)
{
  if(kx122_set_average(m_kx122,avg)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_average failed");
  }
}

void KX122::installISR(mraa::Edge edge, KX122_INTERRUPT_PIN_T intp, int pin, void(*isr)(void*),void *arg)
{
  if(kx122_install_isr(m_kx122,(mraa_gpio_edge_t)edge,intp,pin,isr,arg)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_install_isr failed");
  }
}

void KX122::uninstallISR(KX122_INTERRUPT_PIN_T intp)
{
  kx122_uninstall_isr(m_kx122,intp);
}

void KX122::enableInterrupt1(KX122_INTERRUPT_POLARITY_T polarity)
{
  if(kx122_enable_interrupt1(m_kx122,polarity)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_enable_interrupt1 failed");
  }
}

void KX122::enableInterrupt2(KX122_INTERRUPT_POLARITY_T polarity)
{
  if(kx122_enable_interrupt2(m_kx122,polarity)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_enable_interrupt2 failed");
  }
}

void KX122::disableInterrupt1()
{
  if(kx122_disable_interrupt1(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_disable_interrupt1 failed");
  }
}

void KX122::disableInterrupt2()
{
  if(kx122_disable_interrupt2(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_disable_interrupt2 failed");
  }
}

void KX122::routeInterrupt1(uint8_t bits)
{
  if(kx122_route_interrupt1(m_kx122,bits)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_route_interrupt1 failed");
  }
}

void KX122::routeInterrupt2(uint8_t bits)
{
  if(kx122_route_interrupt2(m_kx122,bits)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_route_interrupt2 failed");
  }
}

bool KX122::getInterruptStatus()
{
  return kx122_get_interrupt_status(m_kx122);
}

void KX122::getInterruptSource(uint8_t *data)
{
  if(kx122_get_interrupt_source(m_kx122,data)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_interrupt_source failed");
  }
}

void KX122::clearInterrupt()
{
  if(kx122_clear_interrupt(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_clear_interrupt failed");
  }
}

void KX122::enableDataReadyInterrupt()
{
  if(kx122_enable_data_ready_interrupt(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_enable_data_ready_interrupt failed");
  }
}

void KX122::disableDataReadyInterrupt()
{
  if(kx122_disable_data_ready_interrupt(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_disable_data_ready_interrupt failed");
  }
}

void KX122::enableBufferFullInterrupt()
{
  if(kx122_enable_buffer_full_interrupt(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_enable_buffer_full_interrupt failed");
  }
}

void KX122::disableBufferFullInterrupt()
{
  if(kx122_disable_buffer_full_interrupt(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_disable_buffer_full_interrupt failed");
  }
}

void KX122::enableBuffer()
{
  if(kx122_enable_buffer(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_enable_buffer failed");
  }
}

void KX122::disableBuffer()
{
  if(kx122_disable_buffer(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_disable_buffer failed");
  }
}

void KX122::bufferInit(uint samples, KX122_RES_T res, KX122_BUFFER_MODE_T mode)
{
  if(kx122_buffer_init(m_kx122,samples,res,mode)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_buffer_init failed");
  }
}

void KX122::setBufferResolution(KX122_RES_T res)
{
  if(kx122_set_buffer_resolution(m_kx122,res)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_buffer_resolution failed");
  }
}

void KX122::setBufferThreshold(uint samples)
{
  if(kx122_set_buffer_threshold(m_kx122,samples)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_buffer_threshold failed");
  }
}

void KX122::setBufferMode(KX122_BUFFER_MODE_T mode)
{
  if(kx122_set_buffer_mode(m_kx122,mode)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_set_buffer_mode failed");
  }
}

void KX122::getBufferStatus(uint *samples)
{
  if(kx122_get_buffer_status(m_kx122,samples)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_buffer_status failed");
  }
}

void KX122::getRawBufferSamples(uint len, float *x_array, float *y_array, float *z_array)
{
  if(kx122_read_buffer_samples_raw(m_kx122,len,x_array,y_array,z_array)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_read_buffer_samples_raw failed");
  }
}

void KX122::getBufferSamples(uint len, float *x_array, float *y_array, float *z_array)
{
  if(kx122_read_buffer_samples(m_kx122,len,x_array,y_array,z_array)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_read_buffer_samples failed");
  }
}

void KX122::clearBuffer()
{
  if(kx122_clear_buffer(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_clear_buffer failed");
  }
}
