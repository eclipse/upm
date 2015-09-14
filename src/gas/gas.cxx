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

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <functional>
#include <string.h>
#include <stdexcept>
#include "gas.h"

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
    for (unsigned int i = 0; i < len; i++) {
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

void
Gas::printGraph (thresholdContext* ctx, uint8_t resolution) {
    std::cout << "(" << ctx->runningAverage << ") | ";
    for (int i = 0; i < ctx->runningAverage / resolution; i++)
        std::cout << "*";
    std::cout << std::endl;
}
