/*
 * Author: Henry Bruce <henry.bruce@intel.com>
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

import java.io.*;
import upm_si1132.SI1132;
import upm_interfaces.ILightSensor;

public class LightSensorSample {
   
   static {
      try {
         System.loadLibrary("javaupm_si1132");
      } catch (UnsatisfiedLinkError e) {
         System.err.println("error in loading native library: " + e.getMessage());
         System.exit(-1);
      }
   }

/*
   private ILightSensor getLightSensor() {

   }
*/
   public static void main(String[] args) throws InterruptedException {
      //! [Interesting]
      // Instantiate an A110X sensor on digital pin D2
      // upm_si1132.SI1132 lightSensor = new upm_si1132.SI1132(512);
      ILightSensor lightSensor = new upm_si1132.SI1132(512);      
      while(true) {
         System.out.println("Illuminance: " + lightSensor.getVisibleLux() + "lux");
         Thread.sleep(1000);
      }
      //! [Interesting]
   }

}


