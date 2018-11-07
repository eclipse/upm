/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
 * Based on the apa102 driver writen by Yannick Adam <yannick.adam@gmail.com>
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

public class P9813_Example {

  public static void main(String[] args) throws InterruptedException {
    // ! [Interesting]
    // Instantiate a chain of 2 LEDs using pin 2 for clock and pin 3 for data
    upm_p9813.P9813 ledChain =
        new upm_p9813.P9813(2, (short)2, (short)3, false);

    System.out.println("Set all LEDs to blue");
    ledChain.setAllLeds((short)0, (short)0, (short)255);

    System.out.println("Set a single LED to red at index 1");
    ledChain.setLed(1, (short)255, (short)0, (short)0);

    // ! [Interesting]
  }
}
