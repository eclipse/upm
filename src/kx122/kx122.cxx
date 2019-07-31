/*
* The MIT License (MIT)
*
* Author: Samuli Rissanen <samuli.rissanen@hotmail.com>
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

#include <unistd.h>

#include "kx122.hpp"

using namespace upm;

KX122::KX122(int bus, int addr, int chip_select, int spi_bus_frequency)
  : m_kx122(kx122_init(bus, addr, chip_select, spi_bus_frequency))
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

uint8_t KX122::getWhoAmI()
{
  uint8_t data;

  if(kx122_get_who_am_i(m_kx122, &data)){
	throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_who_am_i failed");
  }

  return data;
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

std::vector<float> KX122::getAccelerationDataVector()
{
    std::vector<float> xyz(3);
    getAccelerationData(&xyz[0], &xyz[1], &xyz[2]);

    return xyz;
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

uint8_t KX122::getInterruptSource()
{
  uint8_t data;
  if(kx122_get_interrupt_source(m_kx122, &data)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_interrupt_source failed");
  }

  return data;
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

uint KX122::getBufferStatus()
{
  uint nb_samples = 0;
  if(kx122_get_buffer_status(m_kx122, &nb_samples)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_get_buffer_status failed");
  }

  return nb_samples;
}

//Maximum number of samples that can be stored in the buffer of the KX122
#define MAX_SAMPLES_IN_BUFFER 681

std::vector<float> KX122::getRawBufferSamples(uint len)
{
  float bufferx[MAX_SAMPLES_IN_BUFFER], buffery[MAX_SAMPLES_IN_BUFFER], bufferz[MAX_SAMPLES_IN_BUFFER];
  if(kx122_read_buffer_samples_raw(m_kx122,len,bufferx,buffery,bufferz)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_read_buffer_samples_raw failed");
  }

  std::vector<float> xyz_array(len * 3);
  for (uint i = 0; i < len; i++)
  {
	  xyz_array[i * 3 + 0] = bufferx[i];
	  xyz_array[i * 3 + 1] = buffery[i];
	  xyz_array[i * 3 + 2] = bufferz[i];
  }

  return xyz_array;
}

std::vector<float> KX122::getBufferSamples(uint len)
{
  float bufferx[MAX_SAMPLES_IN_BUFFER], buffery[MAX_SAMPLES_IN_BUFFER], bufferz[MAX_SAMPLES_IN_BUFFER];
  if(kx122_read_buffer_samples(m_kx122,len,bufferx,buffery,bufferz)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_read_buffer_samples failed");
  }

  std::vector<float> xyz_array(len * 3);
  for (uint i = 0; i < len; i++)
  {
	  xyz_array[i * 3 + 0] = bufferx[i];
	  xyz_array[i * 3 + 1] = buffery[i];
	  xyz_array[i * 3 + 2] = bufferz[i];
  }

  return xyz_array;
}

void KX122::clearBuffer()
{
  if(kx122_clear_buffer(m_kx122)){
    throw std::runtime_error(std::string(__FUNCTION__) + "kx122_clear_buffer failed");
  }
}
