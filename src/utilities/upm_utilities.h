/*
 * Authors:
 *          Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#ifndef UPM_UTILITIES_H_
#define UPM_UTILITIES_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(linux)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif /* linux */

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
#include <zephyr.h>
#include <device.h>
#include <sys_clock.h>

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT           printf
#else
#include <misc/printk.h>
#define PRINT           printk
#endif

#endif /* CONFIG_BOARD_ARDUINO_101 || CONFIG_BOARD_ARDUINO_101_SSS ||
          CONFIG_BOARD_QUARK_D2000_CRB */

/* Get filename w/o path */
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/**
 * Delay for a number of seconds
 *
 * @param time The number of seconds to delay for
 */
void upm_delay(int time);

/**
 * Delay for a number of milliseconds
 *
 * @param time The number of milliseconds to delay for
 */
void upm_delay_ms(int time);

/**
 * Delay for a number of microseconds
 *
 * @param time The number of microsenconds to delay for
 */
void upm_delay_us(int time);

#ifdef __cplusplus
}
#endif

#endif /* UPM_UTILITIES_H_ */
