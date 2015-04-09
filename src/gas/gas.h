/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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
#pragma once

#include <string>
#include <mraa/gpio.h>
#include <mraa/aio.h>

struct thresholdContext {
    long averageReading;
    long runningAverage;
    int averagedOver;
};

namespace upm {
/**
 * @brief Gas Sensor Library
 *
 * Sensor Library for air quality and gas detecting sensors. Base class Gas provides buffered
 * sampling, threshold checking, a basic printing function and a standard read function.
 *
 * @defgroup gas libupm-gas
 * @ingroup seeed analog gaseous
 */
class Gas {
    public:
        /**
         * Instantiates a Gas object
         *
         * @param gasPin pin where gas is connected
         */
        Gas(int gasPin);

        /**
         * Gas object destructor
         */
        ~Gas();

        /**
         * Get samples from gas sensor according to provided window and
         * number of samples
         *
         * @param freqMS time between each sample (in milliseconds)
         * @param numberOfSamples number of sample to sample for this window
         * @param buffer buffer with sampled data
         */
        virtual int getSampledWindow (unsigned int freqMS, unsigned int numberOfSamples, uint16_t * buffer);

        /**
         * Given sampled buffer this method will return TRUE/FALSE if threshold
         * was reached
         *
         * @param ctx threshold context
         * @param threshold sample threshold
         * @param buffer buffer with samples
         * @param len buffer length
         */
        virtual int findThreshold (thresholdContext* ctx, unsigned int threshold, uint16_t * buffer, unsigned int len);

        /**
         * Return average data for the sampled window
         *
         * @param ctx threshold context
         */
        virtual int getSampledData (thresholdContext* ctx);

        /**
         * Return one sample from the sensor
         *
         * @param ctx threshold context
         */
        virtual int getSample ();

        /**
         *
         * Print running average of threshold context
         *
         * @param ctx threshold context
         */
        virtual void printGraph (thresholdContext* ctx, uint8_t resolution);

    protected:
        mraa_aio_context    m_gasCtx;
};

}
