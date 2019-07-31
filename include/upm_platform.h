/*
 * Authors: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
