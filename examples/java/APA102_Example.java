/*
 * Author: Yannick Adam <yannick.adam@gmail.com>
 * Copyright (c) 2016 Yannick Adam
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class APA102_Example {

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
