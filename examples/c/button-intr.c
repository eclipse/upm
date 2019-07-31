/*
 * Authors: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 - 2016 Intel Corporation.
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
#include "button.h"

#include "upm_utilities.h"
#include "mraa.h"

int main()
{
    if (mraa_init() != MRAA_SUCCESS)
    {
        perror("Failed to initialize mraa\n");
        return -1;
    }

    button_context dev = button_init(2);
    bool abc = 0;
    while(1){
        if(button_get_value(dev, &abc) != UPM_SUCCESS){
            printf("an error has occurred\n");
        }
        upm_delay(1);
        printf("value retrieved: %d\n", abc);
    }

    return 0;
}
