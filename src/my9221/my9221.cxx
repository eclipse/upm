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
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>

#include "my9221.h"

using namespace upm;

MY9221::MY9221 (uint8_t di, uint8_t dcki)
                    : m_clkPinCtx(dcki), m_dataPinCtx(di) {
    mraa::Result error = mraa::SUCCESS;

    // set direction (out)
    error = m_clkPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }

    // set direction (out)
    error = m_dataPinCtx.dir(mraa::DIR_OUT);
    if (error != mraa::SUCCESS) {
        mraa::printError(error);
    }
}

mraa::Result
MY9221::setBarLevel (uint8_t level, bool direction) {
    if (level > 10) {
        return mraa::ERROR_INVALID_PARAMETER;
    }

    send16bitBlock (CMDMODE);
    if (direction) {
        level += 3;
        for(uint8_t block_idx = 12; block_idx > 0; block_idx--) {
            uint32_t state = (block_idx < level) ? BIT_HIGH : BIT_LOW;
            send16bitBlock (state);
        }
    } else {
        for(uint8_t block_idx = 0; block_idx < 12; block_idx++) {
            uint32_t state = (block_idx < level) ? BIT_HIGH : BIT_LOW;
            send16bitBlock (state);
        }
    }
    return lockData ();
}

mraa::Result
MY9221::lockData () {
    mraa::Result error = mraa::SUCCESS;
    error = m_dataPinCtx.write(LOW);
    usleep(100);

    for(int idx = 0; idx < 4; idx++) {
        error = m_dataPinCtx.write(HIGH);
        error = m_dataPinCtx.write(LOW);
    }
    return error;
}

mraa::Result
MY9221::send16bitBlock (short data) {
    mraa::Result error = mraa::SUCCESS;
    for (uint8_t bit_idx = 0; bit_idx < MAX_BIT_PER_BLOCK; bit_idx++) {
        uint32_t state = (data & 0x8000) ? HIGH : LOW;
        error = m_dataPinCtx.write(state);
        state = m_clkPinCtx.read();

        if (state) {
            state = LOW;
        } else {
            state = HIGH;
        }

        error = m_clkPinCtx.write(state);

        data <<= 1;
    }
    return error;
}
