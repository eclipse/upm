/*
 * Author: Noel Eck <noel.eck@intel.com>
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

public class KX122_Example {
	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		// Instantiate a KX122
		upm_kx122.KX122 kx122 = new upm_kx122.KX122(0, -1, 24);

		// Reset the device
		kx122.softwareReset();

		// Initialize the device
		kx122.deviceInit(upm_kx122.KX122_ODR_T.KX122_ODR_50,
				upm_kx122.KX122_RES_T.HIGH_RES,
				upm_kx122.KX122_RANGE_T.KX122_RANGE_2G);

		// Print the acceleration
		while(true){
			upm_kx122.floatVector xyz = kx122.getAccelerationDataVector();
			System.out.println("Acceleration: x = " + xyz.get(0)
				+ " y = " + xyz.get(1)
				+ " z = " + xyz.get(2));

			Thread.sleep(1000);
		}
		//! [Interesting]
	}
}
