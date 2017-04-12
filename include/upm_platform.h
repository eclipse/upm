/*
 * Authors: Jon Trulson <jtrulson@ics.com>
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

#ifndef UPM_PLATFORM_H_
#define UPM_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__)
# define UPM_PLATFORM_LINUX (1)
#elif defined(CONFIG_BOARD_ARDUINO_101) || \
    defined(CONFIG_BOARD_ARDUINO_101_SSS) || \
    defined(CONFIG_BOARD_QUARK_D2000_CRB)
# define UPM_PLATFORM_ZEPHYR (1)
#else
# error "UPM: Unknown Platform!"
#endif

#ifdef __cplusplus
}
#endif

#endif /* UPM_PLATFORM_H_ */
