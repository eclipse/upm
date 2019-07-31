/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/* This example demonstrates how to use one the ADS1015 ADC on the Grove Joule
 * Shield or the Sparkfun ADC Block for Edison with devices that output a small
 * differential voltage (e.g. geophones, piezoelectric bands or pads,
 * thermocouples).
 */
import java.io.*;
import java.util.concurrent.*;
import upm_ads1x15.*;

public class Ads1015_Example
{
    static boolean running = true;
    static int id = 0; // Sample number
    static String fileName = "./ads1015.data"; // Output filename

    public static void main(String[] args) throws InterruptedException
    {
        // Open the output file
        FileWriter fw = null;
        BufferedWriter bw = null;
        try {
            fw = new FileWriter(fileName);
            bw = new BufferedWriter(fw);
        } catch (IOException e) {
            System.out.println("Failed to open output file for writing: " + e.toString());
            System.exit(1);
        }

        // Initialize and configure the ADS1015
        ADS1015 ads1015 = new ADS1015(0, (short)0x48);

        // Put the ADC into differential mode for pins A0 and A1
        ads1015.getSample(ADS1X15.ADSMUXMODE.DIFF_0_1);

        // Set the gain based on expected VIN range to -/+ 2.048 V
        // Can be adjusted based on application to as low as -/+ 0.256 V, see API
        // documentation for details
        ads1015.setGain(ADS1X15.ADSGAIN.GAIN_TWO);

        // Set the sample rate to 3300 samples per second (max) and turn on continuous
        // sampling
        ads1015.setSPS(ADS1015.ADSSAMPLERATE.SPS_3300);
        ads1015.setContinuous(true);

        // Schedule a task to stop logging after 10 seconds
        Executors.newSingleThreadScheduledExecutor().schedule(new Runnable() {
            @Override
            public void run() {
                running = false;
            }
        }, 10, TimeUnit.SECONDS);

        // Read from sensor and write to file every ms
        while(running){
            try {
                bw.write(id + " " + String.format("%.7f", ads1015.getLastSample()) + "\n");
            } catch (IOException e) {
                System.out.println("Failed to write sample " + id + " to file: "+ e.toString());
                System.exit(1);
            }
            id++;
            Thread.sleep(1);
        }

        // Close and exit
        try {
            bw.close();
            fw.close();
        } catch (IOException e) {
            System.out.println("Failed to close output file cleanly: " + e.toString());
            System.exit(1);
        }
        System.out.println("Wrote " + id + " samples to file: " + fileName);
        System.exit(0);
    }
}
