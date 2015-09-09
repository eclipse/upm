/*
 * Author: Alexander Komarov <alexander.komarov@intel.com>
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
#include <functional>
#include <string.h>
#include "joystick12.h"

using namespace upm;

// FIXME AK: to make configurable if needed
const int Joystick12::X_left = 100;
const int Joystick12::X_center = 1610;
const int Joystick12::X_right= 4070;

const int Joystick12::Y_left = 2;
const int Joystick12::Y_center = 1610;
const int Joystick12::Y_right= 4070;


Joystick12::Joystick12(int pinX, int pinY) {

    if ( !(m_joystickCtxX = mraa_aio_init(pinX)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init(pinX) failed, invalid pin?");
        return;
      }

    if ( !(m_joystickCtxY = mraa_aio_init(pinY)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_aio_init(pinY) failed, invalid pin?");
        return;
      }
}

Joystick12::~Joystick12() {
    // close inputs
    mraa_result_t error;
    error = mraa_aio_close(m_joystickCtxX);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_aio_close(m_joystickCtxY);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
}

float Joystick12::getXInput() {
    float in = mraa_aio_read (m_joystickCtxX);
    if (in < X_left) return -1;
    if (in < X_center) return -(X_center - in) / (X_center - X_left);
    if (in == X_center) return 0;
    if (in < X_right) return (in - X_center) / (X_right - X_center);
    if (in >= X_right) return 1;
    mraa_result_print(MRAA_ERROR_UNSPECIFIED);
    return 0;
}

float Joystick12::getYInput() {
    float  in = mraa_aio_read (m_joystickCtxY);
    if (in < Y_left) return -1;
    if (in < Y_center) return -(Y_center - in) / (Y_center - Y_left);
    if (in == Y_center) return 0;
    if (in < Y_right) return (in - Y_center) / (Y_right - Y_center);
    if (in >= Y_right) return 1;
    mraa_result_print(MRAA_ERROR_UNSPECIFIED);
    return 0;
}


