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

import upm_ims.IMS;

public class IMS_Example
{
    public static void main(String[] args) throws InterruptedException
    {
        // ! [Interesting]

        // Instantiate a IMS instance using bus 0 and default i2c address
        IMS sensor = new IMS((short)0);

        while (true)
        {
            System.out.println("Version: "
                    + sensor.get_version()
                    + " light: "
                    + sensor.get_light()
                    + " moisture: "
                    + sensor.get_moisture()
                    + " temp: "
                    + sensor.get_temperature()
                    + " C");

            Thread.sleep(1000);
        }

        // ! [Interesting]
    }
}
