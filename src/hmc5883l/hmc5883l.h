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

#include <mraa/i2c.h>

#define MAX_BUFFER_LENGTH 6

namespace upm {

/**
 * @brief C++ API for HMC5883l (3-axis digital compass)
 *
 * This file defines the HMC5883l C++ interface for libhmc5883l
 *
 * @snippet hmc5883l.cxx Interesting
 *
 */

class Hmc5883l {
public:
    /**
     * Creates a Hmc5883l object
     *
     * @param bus number of used i2c bus
     *
     */
    Hmc5883l(int bus);

    /*
     * Returns the direction
     */
    float direction();

    /*
     * Returns the heading
     */
    float heading();

    /**
     * Returns a pointer to an int[3] that contains the coordinates as ints
     *
     * @return *int to an int[3]
     */
    int* coordinates();

    /**
     * Updates the values by reading from i2c
     *
     * @return 0 for success
     */
    int update();
private:
    int m_coor[3];
    uint8_t m_rx_tx_buf[MAX_BUFFER_LENGTH];
    mraa_i2c_context m_i2c;
};

}
