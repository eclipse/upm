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

//! [Interesting]
public class BH1792_Example {

    public static void main(String[] args) throws InterruptedException {
        int sampleCounter = 20;
        float waitTime;
        upm_bh1792.BH1792 bh1792;

        bh1792 = new upm_bh1792.BH1792();
        BH1792ISR callback = new BH1792ISR(bh1792);

        bh1792.SoftReset();
        bh1792.EnableSyncMode(256, 32);

        bh1792.InstallISR(33, callback);

        waitTime = bh1792.GetMeasurementTimeMS();
        bh1792.StartMeasurement();

        System.out.println("Heart rate sensor readings");
        while (true) {
            Thread.sleep((long) waitTime);
        }
    }
}

class BH1792ISR implements Runnable {

    upm_bh1792.intVector2D result;
    upm_bh1792.BH1792 bh1792;

    public BH1792ISR(upm_bh1792.BH1792 bh1792) {
        super();
        this.bh1792 = bh1792;
    }

    public void run() {
        result = this.bh1792.GetFifoData();
        for(int i = 0; i < 32; i++)
            System.out.println(i + ": off: " + result.get(i).get(0) +
                                ", on: " + result.get(i).get(1));
        System.out.println("Timestamp: " + System.nanoTime() + " nsec\n");
    }
}
//! [Interesting]
