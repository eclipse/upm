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
public class HTU21DSample {

	static {
		try {
			System.loadLibrary("javaupm_htu21d");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		float humidity = 0;
		float temperature = 0;
		float compRH = 0;

		upm_htu21d.HTU21D sensor = new upm_htu21d.HTU21D(0);
		sensor.testSensor();

		while (true) {
			compRH = sensor.getCompRH();
			humidity = sensor.getHumidity();
			temperature = sensor.getTemperature();

			System.out.println("Humidity: " + humidity + ", Temperature: " + temperature
					+ ", compensated RH: " + compRH);

			Thread.sleep(5000);
		}
		// ! [Interesting]
	}

}