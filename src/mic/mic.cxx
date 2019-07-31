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

#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <functional>
#include <string.h>
#include "mic.hpp"

using namespace upm;

Microphone::Microphone(int micPin) {
    // initialise analog mic input
    
    if ( !(m_micCtx = mraa_aio_init(micPin)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init() failed, invalid pin?");
        return;
      }
}

Microphone::~Microphone() {
    // close analog input
    mraa_result_t error;
    error = mraa_aio_close(m_micCtx);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
}

int
Microphone::getSampledWindow (unsigned int freqMS, int numberOfSamples,
                            uint16_t * buffer) {
    int sampleIdx = 0;

    // must have freq
    if (!freqMS) {
        return 0;
    }

    // too much samples
    if (numberOfSamples > 0xFFFFFF) {
        return 0;
    }

    while (sampleIdx < numberOfSamples) {
        int x = mraa_aio_read (m_micCtx);
        if (x == -1) {
            return 0;
        }
        buffer[sampleIdx++] = x;
        usleep(freqMS * 1000);
    }

    return sampleIdx;
}

int
Microphone::findThreshold (thresholdContext* ctx, unsigned int threshold,
                                uint16_t * buffer, int len) {
    long sum = 0;
    for (int i = 0; i < len; i++) {
        sum += buffer[i];
    }

    ctx->averageReading = sum / len;
    ctx->runningAverage = (((ctx->averagedOver-1) * ctx->runningAverage) + ctx->averageReading) / ctx->averagedOver;

    if (ctx->runningAverage > threshold) {
        return ctx->runningAverage;
    } else {
        return 0;
    }
}

void
Microphone::printGraph (thresholdContext* ctx) {
    for (unsigned int i = 0; i < ctx->runningAverage; i++)
        std::cout << ".";
    std::cout << std::endl;
}
