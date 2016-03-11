/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

import upm_h803x.H803X;

public class H803X_Example 
{
    private static String defaultDev = "/dev/ttyUSB0";

    public static void main(String[] args) throws InterruptedException
    {
// ! [Interesting]
        if (args.length > 0)
            defaultDev = args[0];
        System.out.println("Using device " + defaultDev);
        System.out.println("Initializing...");

        // Instantiate an H803X instance, using MODBUS slave address 1, and
        // default comm parameters (9600, 8, N, 2)
        H803X sensor = new H803X(defaultDev, 1);
        
        // output the Slave ID (manufacturer, model, serno)
        System.out.println("Slave ID: " + sensor.getSlaveID());
        System.out.println();

        while (true)
            {
                // update our values from the sensor
                sensor.update();

                // H8035 / H8036
                System.out.println("Consumption (kWh): " 
                                   + sensor.getConsumption());
                System.out.println("Real Power (kW): " 
                                   + sensor.getRealPower());

                if (sensor.isH8036())
                    {
                        // The H8036 has much more data available...
                        
                        System.out.println("Reactive Power (kVAR): "
                                           + sensor.getReactivePower());
                        System.out.println("Apparent Power (kVA): "
                                           + sensor.getApparentPower());
                        System.out.println("Power Factor: "
                                           + sensor.getPowerFactor());
                        System.out.println("Volts Line to Line: "
                                           + sensor.getVoltsLineToLine());
                        System.out.println("Volts Line to Neutral: "
                                           + sensor.getVoltsLineToNeutral());
                        
                        System.out.println("Current: " + sensor.getCurrent());
                        
                        System.out.println("Real Power Phase A (kW): "
                                           + sensor.getRealPowerPhaseA());
                        System.out.println("Real Power Phase B (kW): "
                                           + sensor.getRealPowerPhaseB());
                        System.out.println("Real Power Phase C (kW): "
                                           + sensor.getRealPowerPhaseC());
                        
                        System.out.println("Power Factor Phase A: "
                                           + sensor.getPowerFactorPhaseA());
                        System.out.println("Power Factor Phase B: "
                                           + sensor.getPowerFactorPhaseB());
                        System.out.println("Power Factor Phase C: "
                                           + sensor.getPowerFactorPhaseC());
                        
                        System.out.println("Volts Phase A to B: "
                                           + sensor.getVoltsPhaseAToB());
                        System.out.println("Volts Phase B to C: "
                                           + sensor.getVoltsPhaseBToC());
                        System.out.println("Volts Phase A to C: "
                                           + sensor.getVoltsPhaseAToC());
                        System.out.println("Volts Phase A to Neutral: "
                                           + sensor.getVoltsPhaseAToNeutral());
                        System.out.println("Volts Phase B to Neutral: "
                                           + sensor.getVoltsPhaseBToNeutral());
                        System.out.println("Volts Phase C to Neutral: "
                                           + sensor.getVoltsPhaseCToNeutral());
                        
                        System.out.println("Current Phase A: "
                                           + sensor.getCurrentPhaseA());
                        System.out.println("Current Phase B: "
                                           + sensor.getCurrentPhaseB());
                        System.out.println("Current Phase C: "
                                           + sensor.getCurrentPhaseC());
                        
                        System.out.println("Avg Real Power (kW): "
                                           + sensor.getAvgRealPower());
                        System.out.println("Min Real Power (kW): "
                                           + sensor.getMinRealPower());
                        System.out.println("Max Real Power (kW): "
                                           + sensor.getMaxRealPower());
                    }

                System.out.println();
                Thread.sleep(2000);
            }

// ! [Interesting]
    }
}
