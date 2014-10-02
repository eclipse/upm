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

/// @cond DEV
/**
 * This file only serves to describe sensor groups based on libmraa groups.
 * Sensors may belong to multiple groups. This is purely a documentation header
 * and is not meant to be installed anywhere.
 */
/// @endcond DEV

/**
 * @brief Sensors requiring an ADC value to be read
 * @defgroup analog Analog Sensors
 */

/**
 * @brief Modules using the i2c bus
 * @defgroup i2c I2c
 */

/**
 * @brief Modules using the SPI bus
 * @defgroup spi SPI
 */

/**
 * @brief Modules using GPIOs directly
 * @defgroup gpio GPIO
 */

/**
 * @brief Modules using a PWM capable GPIO pin
 * @defgroup pwm PWM
 */
