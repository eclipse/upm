/*
 * Author: Yannick Adam <yannick.adam@gmail.com>
 * Copyright (c) 2016 Yannick Adam
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

public class APA102Sample {

  public static void main(String[] args) throws InterruptedException {
    // ! [Interesting]
    // Instantiate a strip of 30 LEDs on SPI bus 0
    upm_apa102.APA102 ledStrip =
        new upm_apa102.APA102(30, (short)0, false, (byte)-1);

    System.out.println("Set all LEDs to blue");
    ledStrip.setAllLeds((short)31, (short)0, (short)0, (short)255);

    System.out.println("Set LEDs between 10 and 20 to green");
    ledStrip.setLeds(10, 20, (short)31, (short)0, (short)255, (short)0);

    System.out.println("Set a single LED to red at index 15");
    ledStrip.setLed(15, (short)31, (short)255, (short)0, (short)0);

    // ! [Interesting]
  }
}
