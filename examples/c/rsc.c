/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "upm_utilities.h"
#include "rsc.h"

int main(void) {
//! [Interesting]
    rsc_context dev = rsc_init(0, 9, 8);

    rsc_set_data_rate(dev, N_DR_330_SPS);
    rsc_set_mode(dev, NORMAL_MODE);

    float pressure;
    float temp;
    while(1){
        pressure = rsc_get_pressure(dev);
        printf("Retrieved pressure: %f %d\n", pressure, dev->unit);
        temp = rsc_get_temperature(dev);
        printf("Retieved temperature: %f C\n", temp);
        upm_delay_ms(500);
    }
//! [Interesting]
    return 0;
}
