/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 *         Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

public class GUVAS12D_Example {
    // analog voltage, usually 3.3 or 5.0
    private static final float GUVAS12D_AREF = 5;

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        // Instantiate a GUVAS12D on analog pin A0
        upm_guvas12d.GUVAS12D volts = new upm_guvas12d.GUVAS12D(0,
                                                                GUVAS12D_AREF);

        while (true)
        {
            System.out.println("Volts: "
                               + volts.volts()
                               + ", Intensity: "
                               + volts.intensity()
                               + " mW/m^2");

            Thread.sleep(1000);
        }
        // ! [Interesting]
    }

}
