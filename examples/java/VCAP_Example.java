/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_vcap.VCAP;

public class VCAP_Example 
{
    private static String defaultDev = "/dev/video0";

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        if (args.length > 0)
            defaultDev = args[0];

        System.out.println("Using device " + defaultDev);
        System.out.println("Initializing...");

        // Instantiate an VCAP instance, using the specified video device
        VCAP sensor = new VCAP(defaultDev);
        
        // enable some debug/verbose output
        sensor.setDebug(true);

        // This is just a hint.  The kernel can change this to a lower
        // resolution that the hardware supports.  Use getWidth() and
        // getHeight() methods to see what the kernel actually chose if you
        // care.
        sensor.setResolution(1920, 1080);

        // capture an image
        sensor.captureImage();
        
        // convert and save it as a jpeg
        sensor.saveImage("video-img1.jpg");

// ! [Interesting]
    }
}
