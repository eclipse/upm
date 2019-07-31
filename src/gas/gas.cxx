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
#include <unistd.h>
#include <stdlib.h>
#include <functional>
#include <string.h>
#include <stdexcept>
#include "gas.hpp"

using namespace upm;

Gas::Gas(int gasPin) : m_aio(gasPin) {
}

Gas::~Gas() {
}

int
Gas::getSampledWindow (unsigned int freqMS, int numberOfSamples,
                            uint16_t * buffer) {
    int sampleIdx = 0;

    // must have freq
    if (!freqMS) {
        return 0;
    }

    // too many samples
    if (numberOfSamples > 0xFFFFFF) {
        return 0;
    }

    while (sampleIdx < numberOfSamples) {
        buffer[sampleIdx++] = getSample();
        usleep(freqMS * 1000);
    }

    return sampleIdx;
}

int
Gas::findThreshold (thresholdContext* ctx, unsigned int threshold,
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

int
Gas::getSampledData (thresholdContext* ctx) {
    return ctx->runningAverage;
}

int
Gas::getSample () {
    return m_aio.read();
}

float
Gas::getConcentration() {
    return m_aio.read();
}

void
Gas::printGraph (thresholdContext* ctx, uint8_t resolution) {
    std::cout << "(" << ctx->runningAverage << ") | ";
    for (unsigned int i = 0; i < ctx->runningAverage / resolution; i++)
        std::cout << "*";
    std::cout << std::endl;
}
