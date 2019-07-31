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
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "upm_utilities.h"
#include "hcsr04.h"

int main() {
    hcsr04_context dev = hcsr04_init(2,4);
    if(dev == NULL) {
        printf("Unable to intialize the sensor\n");
        return 0;
    }

    double distance;
    while(1) {
        distance = hcsr04_get_distance(dev, HCSR04_CM);
        printf("Distance detected: %f\n", distance);
        upm_delay(1);
    }

    return 0;
}
