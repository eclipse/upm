/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Contributions: Rex Tsai <rex.cc.tsai@gmail.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Based on original C++ driver written by:
 * Author: Zion Orent <sorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    // This is a data struct used by the ppd42ns (dust sensor) driver.
    typedef struct
    {
        unsigned int lowPulseOccupancy;
        double ratio;
        double concentration;
        double ugm3;
        int aqi;
    } ppd42ns_dust_data;

#ifdef __cplusplus
}
#endif
