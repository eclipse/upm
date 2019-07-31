/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/gpio.h>
#include <mraa/aio.h>

struct thresholdContext {
    long averageReading;
    unsigned long runningAverage;
    int averagedOver;
};

namespace upm {

/**
 * @brief Analog Microphone Library
 * @defgroup mic libupm-mic
 * @ingroup seeed pwm sound gsk
 */
/**
 * @library mic
 * @sensor microphone
 * @comname Simple Analog Microphone
 * @altname Grove Sound Sensor
 * @type sound
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_Sound_Sensor
 * @con analog
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
