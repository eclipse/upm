/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
 * Based on the apa102 driver writen by Yannick Adam <yannick.adam@gmail.com>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
