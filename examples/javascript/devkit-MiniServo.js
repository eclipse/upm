/*
 * Copyright (c) 2014 Intel Corporation.
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

/*
We use jsupm_servo module in JavaScript to manipulate the servo. 
Here, we are not using ES08A, instead we will initialize our Servo with our own metadata. 
*/

//Load servo module.
var servoModule = require("jsupm_servo");

//Instantiate Servo module on digital port 5
var servo = new servoModule.Servo(5);
servo.setMinPulseWidth(600);
servo.setMaxPulseWidth(2200) 

function moveServo() 
{
  //set angle of the servo to 180 degree after one seconds
  setInterval(function () {
      servo.setAngle(180);
  }, 1000);

  //set angle of the servo to 90 degree after two seconds
  setInterval(function () {
      servo.setAngle(90);
  }, 2000);

  //set angle of the servo to 0 degree after three seconds
  setInterval(function () {
      servo.setAngle(0);
  }, 3000);
}

moveServo();