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
#include "abp.h"

#include "upm_utilities.h"

int main()
{
    abp_context dev = abp_init(0, ABP_DEFAULT_ADDRESS);
    if(dev == NULL) {
        printf("Unable to initialize sensor\n");
        return 0;
    }

    while(1){
        abp_update(dev);
        printf("Retrieved pressure value: %f\n", abp_get_pressure(dev));
        printf("Retrieved temperature value: %f\n", abp_get_temperature(dev));
        upm_delay(1);
    }

    return 0;
}
