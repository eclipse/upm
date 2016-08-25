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
#include <mraa/aio.hpp>

struct thresholdContext {
    long averageReading;
    long runningAverage;
    int averagedOver;
};

namespace upm {
/**
 * @brief Gas Sensor library
 *
 * Library for air quality and gas detecting sensors. Base class Gas provides buffered
 * sampling, threshold checking, basic printing function, and standard read function.
 *
 * @defgroup gas libupm-gas
 * @ingroup seeed analog gaseous eak hak
 */
class Gas {
    public:
        /**
         * Instantiates a Gas object
         *
         * @param gasPin Pin where gas is connected
         */
        Gas(int gasPin);

        /**
         * Gas object destructor
         */
        ~Gas();

        /**
         * Gets samples from the gas sensor according to the provided window and
         * number of samples
         *
         * @param freqMS Time between each sample (in milliseconds)
         * @param numberOfSamples Number of sample to sample for this window
         * @param buffer Buffer with sampled data
         */
        virtual int getSampledWindow (unsigned int freqMS, int numberOfSamples, uint16_t * buffer);

        /**
         * Given the sampled buffer, this method returns TRUE/FALSE if the threshold
         * is reached
         *
         * @param ctx Threshold context
         * @param threshold Sample threshold
         * @param buffer Buffer with samples
         * @param len Buffer length
         */
        virtual int findThreshold (thresholdContext* ctx, unsigned int threshold, uint16_t * buffer, int len);

        /**
         * Returns average data for the sampled window
         *
         * @param ctx Threshold context
         */
        virtual int getSampledData (thresholdContext* ctx);

        /**
         * Returns one sample from the sensor
         */
        virtual int getSample ();

        /**
         *
         * Prints a running average of the threshold context
         *
         * @param ctx Threshold context
         */
        virtual void printGraph (thresholdContext* ctx, uint8_t resolution);

    protected:
        mraa::Aio    m_aio;
};

}
