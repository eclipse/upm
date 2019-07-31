/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_bno055.BNO055;

public class BNO055_Example
{
    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        System.out.println("Initializing...");

        // Instantiate an BNO055 using default parameters (bus 0, addr
        // 0x28).  The default running mode is NDOF absolute orientation
        // mode.
        BNO055 sensor = new BNO055();

        System.out.println("First we need to calibrate.  4 numbers will be output every");
        System.out.println("second for each sensor.  0 means uncalibrated, and 3 means");
        System.out.println("fully calibrated.");
        System.out.println("See the UPM documentation on this sensor for instructions on");
        System.out.println("what actions are required to calibrate.");
        System.out.println("");

        while (!sensor.isFullyCalibrated())
            {
                upm_bno055.intVector calData = sensor.getCalibrationStatus();

                System.out.println("Magnetometer: " + calData.get(0)
                                   + " Accelerometer: " + calData.get(1)
                                   + " Gyroscope: " + calData.get(2)
                                   + " System: " + calData.get(3));

                Thread.sleep(1000);

            }

        System.out.println("");
        System.out.println("Calibration complete.");
        System.out.println("");

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                upm_bno055.floatVector data = sensor.getEulerAngles();

                System.out.println("Euler: Heading: " + data.get(0)
                                   + " Roll: " + data.get(1)
                                   + " Pitch: " + data.get(2)
                                   + " degrees");

                data = sensor.getQuaternions();
                System.out.println("Quaternion: W: " + data.get(0)
                                   + " X: " + data.get(1)
                                   + " Y: " + data.get(2)
                                   + " Z: " + data.get(3));

                data = sensor.getLinearAcceleration();
                System.out.println("Linear Acceleration: X: " + data.get(0)
                                   + " Y: " + data.get(1)
                                   + " Z: " + data.get(2)
                                   + " m/s^2");

                data = sensor.getGravityVectors();
                System.out.println("Gravity Vector: X: " + data.get(0)
                                   + " Y: " + data.get(1)
                                   + " Z: " + data.get(2)
                                   + " m/s^2");

                System.out.println();
                Thread.sleep(250);
            }

// ! [Interesting]
    }
}
