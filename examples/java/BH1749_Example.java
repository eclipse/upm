/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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