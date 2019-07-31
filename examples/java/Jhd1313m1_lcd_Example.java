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

public class Jhd1313m1_lcd_Example{

    public static void main(String[] args) throws InterruptedException {
        //! [Interesting]
        upm_jhd1313m1.Jhd1313m1 lcd =
            new upm_jhd1313m1.Jhd1313m1(1, 0x3E, 0x62);

        lcd.setCursor(0,0);
        lcd.write("Hello World");
        lcd.setCursor(1,2);
        lcd.write("Hello World");

        System.out.println("Sleeping for 5 seconds");
        Thread.sleep(5000);
        lcd.clear();
        //! [Interesting]
    }

}
