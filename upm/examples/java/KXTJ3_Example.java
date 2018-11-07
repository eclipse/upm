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
