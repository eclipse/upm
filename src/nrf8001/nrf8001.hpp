/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @brief NRF8001 Bluetooth Low Energy Module
 * @defgroup nrf8001 libupm-nrf8001
 * @ingroup adafruit wifi analog gpio spi
 */
/**
 * @library nrf8001
 * @sensor nrf8001
 * @comname Bluetooth Low Energy (BLE) Module
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

#include "lib_aci.h"
#include "aci_setup.h"

void init_local_interfaces (aci_state_t* aci, uint8_t reqn, uint8_t rdyn, uint8_t rst);
void close_local_interfaces (aci_state_t* aci);
