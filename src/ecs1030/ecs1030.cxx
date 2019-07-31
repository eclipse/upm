/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>

#include "ecs1030.hpp"

using namespace upm;

ECS1030::ECS1030 (int pinNumber) {
    m_dataPinCtx = mraa_aio_init(pinNumber);
    if (m_dataPinCtx == NULL) {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed");
    }

    m_calibration = 111.1;
}

ECS1030::ECS1030 (std::string initStr) : mraaIo(initStr) {

  mraa_io_descriptor* descs = mraaIo.getMraaDescriptors();
  if(!descs->aios) {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed");
  } else {
    m_dataPinCtx = descs->aios[0];
  }

  m_calibration = 111.1;
}

ECS1030::~ECS1030 () {
    mraa_result_t error = MRAA_SUCCESS;

    error = mraa_aio_close (m_dataPinCtx);
    if (error != MRAA_SUCCESS) {
    }
}

double
ECS1030::getCurrency_A () {
    int     sensorValue  = 0;
    float   volt         = 0;
    float   rms          = 0;

    for (int i = 0; i < NUMBER_OF_SAMPLES; i++) {
        sensorValue = mraa_aio_read (m_dataPinCtx);
        if (sensorValue == -1) throw std::runtime_error(std::string(__FUNCTION__) +
                                                        ": Failed to do an aio read.");
        volt = (VOLT_M * sensorValue) - 2.5;
        volt = volt * volt;
        rms = rms + volt;
        usleep (DELAY_MS);
    }

    rms = rms / (float)NUMBER_OF_SAMPLES;
    rms = sqrt(rms);
    return rms / R_LOAD;
}

double
ECS1030::getCurrency_B () {
    double sumCurrency    = 0;

    for (int i = 0; i < NUMBER_OF_SAMPLES; i++) {
        m_lastSample = m_sample;
        m_sample = mraa_aio_read (m_dataPinCtx);
        if (m_sample == -1) throw std::runtime_error(std::string(__FUNCTION__) +
                                                     ": Failed to do an aio read.");
        m_lastFilter = m_filteredSample;
        m_filteredSample = 0.996 * (m_lastFilter + m_sample - m_lastSample);
        sumCurrency += (m_filteredSample * m_filteredSample);
    }

    double ratio = m_calibration * ((SUPPLYVOLTAGE / 1000.0) / (ADC_RESOLUTION));
    return ( ratio * sqrt(sumCurrency / NUMBER_OF_SAMPLES) );
}

double
ECS1030::getPower_A () {
    return 220.0 * getCurrency_A ();
}

double
ECS1030::getPower_B () {
    return 220.0 * getCurrency_B ();
}
