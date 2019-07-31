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

public class GroveLEDBar_Example {

    public static void main(String[] args) throws InterruptedException {
        //! [Interesting]
        // Instantiate a Grove LED Bar, with Data pin D8 and Clock pin D9
        upm_my9221.GroveLEDBar bar = new upm_my9221.GroveLEDBar((short) 8, (short) 9);
        
        while (true) {
            for (short idx = 1; idx < 11; idx++) {
                bar.setBarLevel(idx);
                Thread.sleep(100);
            }
        }
        //! [Interesting]
    }
	
}
