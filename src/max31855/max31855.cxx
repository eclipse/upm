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

#include "max31855.hpp"

using namespace upm;

//! [Constructor]
MAX31855::MAX31855(int bus, int cs)
{
    // initialise chip select as a normal gpio
    if ( !(m_gpio = mraa_gpio_init(cs)) ) 
      {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": mraa_gpio_init(cs) failed, invalid pin?");
        return;
      }
    mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);

    // initialise the spi bus with a 2Mhz clock
    m_sensor = mraa_spi_init(bus);
    mraa_spi_frequency(m_sensor, 2000000);
}
//! [Constructor]

//! [Destructor]
MAX31855::~MAX31855()
{
    // close both m_sensor & m_gpio cleanly
    mraa_result_t error;
    error = mraa_spi_stop(m_sensor);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
    error = mraa_gpio_close(m_gpio);
    if (error != MRAA_SUCCESS) {
        mraa_result_print(error);
    }
}
//! [Destructor]

double
MAX31855::getTemp()
{
//! [spi]
    // set chip select low
    mraa_gpio_write(m_gpio, 0);

    uint8_t buf[4];

    // set our input buffer to 0, this is clean but not required
    memset(buf, 0, sizeof(uint8_t)*4);

    // Write buffer to the spi slave
    uint8_t* x = mraa_spi_write_buf(m_sensor, buf, 4);
//! [spi]

//! [conversion]
    // Endian correct way of making our char array into an 32bit int
    int32_t temp = (x[0] << 24) | (x[1] << 16) | (x[2] << 8) | x[3];;

    // mraa_spi_write_buf does not free the return buffer
    free(x);

    if (temp & 0x7) {
        std::cerr << "Something went very wrong!" << std::endl;
    }

    // scrap all the data we dont care about
    temp >>= 18;

    // LSB = 0.25 degrees C
    double c = (double) temp;
    c *= 0.25;
//! [conversion]

    // set chip select high
    mraa_gpio_write(m_gpio, 1);

    return c;
}
