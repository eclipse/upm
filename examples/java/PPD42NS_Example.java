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

public class PPD42NS_Example {

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        // Instantiate a dust sensor on GPIO pin D8
        upm_ppd42ns.PPD42NS dust = new upm_ppd42ns.PPD42NS(8);
        upm_ppd42ns.ppd42ns_dust_data data;

        System.out.println("This program will give readings every 30 seconds until you stop it");

        while (true) {
            data = dust.getData();

            System.out.println("Low pulse occupancy: " + data.getLowPulseOccupancy());
            System.out.println("Ratio: " + data.getRatio());
            System.out.println("Concentration: " + data.getConcentration());
        }
        // ! [Interesting]
    }

}
