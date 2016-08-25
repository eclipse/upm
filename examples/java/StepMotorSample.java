/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//NOT TESTED!!!
public class StepMotorSample {

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
