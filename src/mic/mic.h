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
 * @altname Grove Sound Sensor
 * @type sound
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Sound_Sensors
 * @con pwm
 * @kit gsk
 *
 * @brief API for the Analog Microphone
 *
 * This module defines the Analog Microphone sensor
 *
 * @image html mic.jpg
 * @snippet mic.cxx Interesting
 */
class Microphone {
    public:
        /**
         * Instantiates a Microphone object
         *
         * @param micPin Pin where the microphone is connected
         */
        Microphone(int micPin);

        /**
         * Microphone object destructor
         */
        ~Microphone();

        /**
         * Gets samples from the microphone according to the provided window and
         * number of samples
         *
         * @param freqMS Time between each sample (in microseconds)
         * @param numberOfSamples Number of sample to sample for this window
         * @param buffer Buffer with sampled data
         */
        int getSampledWindow (unsigned int freqMS, int numberOfSamples, uint16_t * buffer);

        /**
         * Given the sampled buffer, this method returns TRUE/FALSE if threshold
         * is reached
         *
         * @param ctx Threshold context
         * @param threshold Sample threshold
         * @param buffer Buffer with samples
         * @param len Buffer length
         */
        int findThreshold (thresholdContext* ctx, unsigned int threshold, uint16_t * buffer, int len);

        /**
         *
         * Prints a running average of the threshold context
         *
         * @param ctx Threshold context
         */
        void printGraph (thresholdContext* ctx);

    private:
        mraa_aio_context    m_micCtx;
};

}
