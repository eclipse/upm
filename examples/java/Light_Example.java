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

public class Light_Example {
    public static void main(String args[]) throws InterruptedException {
        // ! [Interesting]
        upm_light.Light gl = new upm_light.Light(0);

        while (true) {
            float raw_value = gl.getNormalized();
            float value = gl.value();

            System.out.println("normalized: " + raw_value);
            System.out.println("value: " + value);

            Thread.sleep(1000);
        }
        // ! [Interesting]
    }
}
