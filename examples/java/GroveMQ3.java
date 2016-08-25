/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
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
 
import upm_gas.Gas;
import upm_gas.MQ2;
import upm_gas.MQ3;
import upm_gas.MQ4;
import upm_gas.MQ5;
import upm_gas.MQ6;
import upm_gas.MQ7;
import upm_gas.MQ8;
import upm_gas.MQ9;
import upm_gas.TP401;
import upm_gas.thresholdContext;

public class GroveMQ3 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		// initialize the sensor on A0
		MQ3 mq3 = new MQ3(0);

		short[] buffer = new short[128];

		thresholdContext context = new thresholdContext();

		context.setAverageReading(0);
		context.setRunningAverage(0);
		context.setAveragedOver(2);

		int len;
		int thres;
		while(true){
			len = mq3.getSampledWindow((long)2, buffer);

			if(len != 0){
				thres = mq3.findThreshold(context, 30, buffer);
				mq3.printGraph(context, (short)5);
				if(thres != 0){
					// do something
					System.out.println("threshold is crossed");
				}
			}
		}
		//! [Interesting]
	}

}
