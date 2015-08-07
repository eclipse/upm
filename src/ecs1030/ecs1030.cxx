/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>

#include "ecs1030.h"

using namespace upm;

ECS1030::ECS1030 (uint8_t pinNumber) {
    m_dataPinCtx = mraa_aio_init(pinNumber);
    if (m_dataPinCtx == NULL) {
      throw std::invalid_argument(std::string(__FUNCTION__) + 
                                  ": mraa_aio_init() failed");
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
    float   rLoad        = 0;
    float   volt         = 0;
    float   rms          = 0;

    for (int i = 0; i < NUMBER_OF_SAMPLES; i++) {
        sensorValue = mraa_aio_read (m_dataPinCtx);
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
