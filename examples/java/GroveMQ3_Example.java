/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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

public class GroveMQ3_Example {

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
