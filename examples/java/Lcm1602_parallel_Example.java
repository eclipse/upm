/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//NOT TESTED!!!
public class Lcm1602_parallel_Example {

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        // LCD connection:
        // LCD RS pin to digital pin 8
        // LCD Enable pin to digital pin 13
        // LCD D4 pin to digital pin 2
        // LCD D5 pin to digital pin 3
        // LCD D6 pin to digital pin 4
        // LCD D7 pin to digital pin 5
        // LCD R/W pin to ground
        // 10K trimmer potentiometer:
        // ends to +5V and ground
        // wiper to LCD VO pin (pin 3)
        upm_lcm1602.Lcm1602 lcd =
            new upm_lcm1602.Lcm1602(8, 13, 2, 3, 4, 5, (short) 16, (short) 2);

        lcd.setCursor(0, 0);
        lcd.write("Hello World");
        lcd.setCursor(1, 2);
        lcd.write("Hello World");

        System.out.println("Sleeping for 5 seconds");
        Thread.sleep(5000);
        // ! [Interesting]
    }
}
