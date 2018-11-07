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
