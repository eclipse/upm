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
#include <mraa/aio.hpp>
#include <interfaces/iGas.hpp>

struct thresholdContext {
    long averageReading;
    unsigned long runningAverage;
    int averagedOver;
};

namespace upm {
/**
 * @brief Gas Sensors Library
 *
 * Library for air quality and gas detecting sensors. Base class Gas provides buffered
 * sampling, threshold checking, basic printing function, and standard read function.
 *
 * @defgroup gas libupm-gas
 * @ingroup seeed analog gaseous eak hak
 */
class Gas: virtual public iGas {
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
        virtual ~Gas();

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
         * Returns one sample from the sensor
         */
        virtual float getConcentration ();

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
