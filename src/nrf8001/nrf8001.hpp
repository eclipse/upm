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

/**
 * @brief NRF8001 Bluetooth Low Energy library
 * @defgroup nrf8001 libupm-nrf8001
 * @ingroup adafruit wifi analog gpio spi
 */
/**
 * @library nrf8001
 * @sensor nrf8001
 * @comname NRF8001 Bluetooth Low Energy
 * @type wifi
 * @man adafruit
 * @web https://learn.adafruit.com/getting-started-with-the-nrf8001-bluefruit-le-breakout/introduction
 * @con spi gpio analog
 *
 * @brief NRF8001 Bluetooth Low Energy library
 *
 * These files define the NRF8001 interface for lib-nrf8001. Interaction
 * with this device is done through the ACI routines included with the library.
 * You can use the nRF UART app in Apple* App Store and Google* Play* on
 * Samsung* Galaxy S*4 running Android* 4.3 to interact with a microcontroller running
 * the 'hello world' example.
 *
 * @image html nrf8001.jpg
 * @snippet nrf8001-helloworld.cxx Interesting
 * @snippet nrf8001-broadcast.cxx Interesting
 */
#pragma once

#include <string>
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <mraa/spi.h>

#include <lib_aci.h>
#include <aci_setup.h>

void init_local_interfaces (aci_state_t* aci, uint8_t reqn, uint8_t rdyn, uint8_t rst);
void close_local_interfaces (aci_state_t* aci);
