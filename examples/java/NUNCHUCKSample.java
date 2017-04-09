/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

public class NUNCHUCKSample {

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        // Instantiate a nunchuck controller bus 0
        upm_nunchuck.NUNCHUCK nunchuck = new upm_nunchuck.NUNCHUCK(3);

        while (true)
        {
            nunchuck.update();
            System.out.println("stickX: "
                               + nunchuck.getStickX()
                               + ", stickY: "
                               + nunchuck.getStickY());
            System.out.println("accelX: "
                               + nunchuck.getAccelX()
                               + ", accelY: "
                               + nunchuck.getAccelY()
                               + ", accelZ: "
                               + nunchuck.getAccelZ());

            if (nunchuck.getButtonC())
                System.out.println("Button C pressed");
            else
                System.out.println("Button C not pressed");

            if (nunchuck.getButtonZ())
                System.out.println("Button Z pressed");
            else
                System.out.println("Button Z not pressed");

            Thread.sleep(100);
        }
        // ! [Interesting]
    }

}
