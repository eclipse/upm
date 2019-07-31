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
#include "veml6070.h"

#include "upm_utilities.h"

int main()
{
    veml6070_context dev = veml6070_init(0);
    if(dev == NULL) {
        printf("Unable to initialize sensor\n");
        return 0;
    }
    upm_delay(1);

    while(1){
        printf("Retrieved UV Value: %d\n", veml6070_get_uv_intensity(dev));
        upm_delay(1);
    }

    return 0;
}
