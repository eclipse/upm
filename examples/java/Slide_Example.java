/*
 * Author: Andrei Vasiliu <andrei.vasiliu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class Slide_Example {
    public static void main (String args[]) throws InterruptedException {
        //! [Interesting]
        // Instantiate new grove slide potentiometer on  analog pin A0
        upm_slide.Slide slide = new upm_slide.Slide(0);

        while (true) {
            float raw_value = slide.raw_value();
            float value = slide.voltage_value();

            System.out.println("raw value: " + raw_value);
            System.out.println("value: " + value);

            Thread.sleep(2500);
        }
        //! [Interesting]
    }
}
