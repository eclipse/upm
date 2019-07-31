/*
 * Authors: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

int counter;

void button_isr(void *arg){
    counter++;
}

int main()
{
    counter = 0;
    if (mraa_init() != MRAA_SUCCESS)
    {
        perror("Failed to initialize mraa\n");
        return -1;
    }

    button_context dev = button_init(2);

    button_install_isr(dev, MRAA_GPIO_EDGE_RISING, button_isr, NULL);
    while(1){
        printf("Interrupts: %d\n", counter);
        upm_delay(3);
    }

    return 0;
}
