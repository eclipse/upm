/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "ims.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    ims_context* sensor = ims_init(0, IMS_ADDRESS_DEFAULT);

    if (!sensor)
    {
        printf("ims_init() failed\n");
        return 1;
    }

    int i2c_addr_cur = IMS_ADDRESS_DEFAULT + 1;

    // Every second, sample the sensor outputs
    while (shouldRun)
    {
        uint16_t version, light, moisture, temp;

        if (ims_get_version(sensor, &version) == UPM_SUCCESS &&
            ims_get_light(sensor, &light) == UPM_SUCCESS &&
            ims_get_moisture(sensor, &moisture) == UPM_SUCCESS &&
            ims_get_temperature(sensor, &temp) == UPM_SUCCESS)
            printf("Version: %d light: 0x%04x moisture: 0x%04x temp: %3.2f C\n",
                    version, light, moisture, temp/10.0);
        else
        {
            printf("Failed to read IMS sensor data...\n");
            break;
        }

        // Change the address and continue
        if (i2c_addr_cur >= 128) i2c_addr_cur = 1;
        printf("Changing device address to 0x%02x\n", i2c_addr_cur);
        if (ims_reset_i2c_address(sensor, i2c_addr_cur++) != UPM_SUCCESS)
        {
            printf("Failed to change IMS I2C address...\n");
            break;
        }
        upm_delay(1);
    }

    //! [Interesting]
    printf("Exiting\n");
    ims_close(sensor);

    return 0;
}
