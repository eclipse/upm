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
 * @brief Analog Microphone library
 * @defgroup mic libupm-mic
 * @ingroup seeed pwm sound gsk
 */
/**
 * @library mic
 * @sensor mic
 * @comname Analog Microphone
 * @type sound
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Sound_Sensors
 * @con pwm
 * @kit gsk
 *
 * @brief C++ API for Microphone
 *
 * This file defines the Microphone Analog sensor
 *
 * @image html mic.jpg
 * @snippet mic.cxx Interesting
 */
class Microphone {
    public:
        /**
         * Instanciates a Microphone object
         *
         * @param micPin pin where microphone is connected
         */
        Microphone(int micPin);

        /**
         * MAX31723 object destructor
         */
        ~Microphone();

        /**
         * Get samples from microphone according to provided window and
         * number of samples
         *
         * @param freqMS time between each sample (in microseconds)
         * @param numberOfSamples number of sample to sample for this window
         * @param buffer bufer with sampled data
         */
        int getSampledWindow (unsigned int freqMS, unsigned int numberOfSamples, uint16_t * buffer);

        /**
         * Given sampled buffer this method will return TRUE/FALSE if threshold
         * was reached
         *
         * @param ctx threshold context
         * @param threshold sample threshold
         * @param buffer buffer with samples
         * @param len bufer len
         */
        int findThreshold (thresholdContext* ctx, unsigned int threshold, uint16_t * buffer, unsigned int len);

        /**
         *
         * Print running average of threshold context
         *
         * @param ctx threshold context
         */
        void printGraph (thresholdContext* ctx);

    private:
        mraa_aio_context    m_micCtx;
};

}
