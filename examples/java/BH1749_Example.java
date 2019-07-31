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

public class BH1749_Example {

    public static void main(String[] args) throws InterruptedException {
    //! [Interesting]
        int sampleCounter = 10;
        long waitTime;
        upm_bh1749.BH1749 bh1749;
        upm_bh1749.uint16Vector result;

        bh1749 = new upm_bh1749.BH1749();
        bh1749.SensorInit(upm_bh1749.OPERATING_MODES.INT_JUDGE_1,
            upm_bh1749.MEAS_TIMES.MEAS_240MS, upm_bh1749.RGB_GAINS.RGB_GAIN_1X,
            upm_bh1749.IR_GAINS.IR_GAIN_1X, upm_bh1749.INT_SOURCES.RED);
        bh1749.SetThresholdHigh(511);
        bh1749.Enable();

        waitTime = bh1749.GetMeasurementTime();
        System.out.println("Color readings");
        while (sampleCounter-- > 0) {
            result = bh1749.GetMeasurements();
            System.out.println("R: " + result.get(0) + ", G: " + result.get(1) +
                            ", B: " + result.get(2) + ", IR: " + result.get(3) +
                            ", G2: " + result.get(4));

            Thread.sleep((long) waitTime);
        }
    //! [Interesting]
    }
}