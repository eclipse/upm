/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

public class KXTJ3_Example {

    public static void main(String[] args) throws InterruptedException {

        upm_kxtj3.KXTJ3 kxtj3 = new upm_kxtj3.KXTJ3(0);

        kxtj3.SensorInit(upm_kxtj3.KXTJ3_ODR_T.KXTJ3_ODR_25,
            upm_kxtj3.KXTJ3_RESOLUTION_T.HIGH_RES,
            upm_kxtj3.KXTJ3_G_RANGE_T.KXTJ3_RANGE_16G_14);

        float waitTime = kxtj3.GetAccelerationSamplePeriod() * 1000;
        int sampleCounter = 10;
        System.out.println("Setting settings:\nODR: 25 Hz\nResolution: " +
                            "High\nAcceleration range: 16g with 14bits");
        System.out.println("Acceleration");
        upm_kxtj3.floatVector xyz;
        while (sampleCounter-- > 0) {
            xyz = kxtj3.GetAccelerationVector();
            System.out.println("x = " + xyz.get(0) + " y = " + xyz.get(1) +
                            " z = " + xyz.get(2));

            Thread.sleep((long) waitTime);
        }
    }
}
