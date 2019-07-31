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
public class StepMotor_Example {

    public static void main(String[] args) throws InterruptedException {
        // ! [Interesting]
        upm_stepmotor.StepMotor sensor = new upm_stepmotor.StepMotor(2, 3);

        while (true) {
            System.out.println("One complete rotation forward and back at 60 rpm.");
            sensor.setSpeed(60);
            sensor.stepForward(200);
            Thread.sleep(1000);
            sensor.stepBackward(200);
            Thread.sleep(1000);

            System.out.println("One complete rotation forward and back at 150 rpm.");
            sensor.setSpeed(150);
            sensor.stepForward(200);
            Thread.sleep(1000);
            sensor.stepBackward(200);
            Thread.sleep(1000);

            System.out.println("One complete rotation forward and back at 300 rpm.");
            sensor.setSpeed(300);
            sensor.stepForward(200);
            Thread.sleep(1000);
            sensor.stepBackward(200);
            Thread.sleep(1000);
        }
        // ! [Interesting]
    }
}
