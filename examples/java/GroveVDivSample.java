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
public class GroveVDivSample {
	private static final short gain3 = 3;
	private static final short gain10 = 10;

	static {
		try {
			System.loadLibrary("javaupm_grovevdiv");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Grove Voltage Divider sensor on analog pin A0
		upm_grovevdiv.GroveVDiv vDiv = new upm_grovevdiv.GroveVDiv(0);

		// collect data and output measured voltage according to the setting
		// of the scaling switch (3 or 10)
		while (true) {
			long val = vDiv.value(100);
			float gain3val = vDiv.computedValue(gain3, val);
			float gain10val = vDiv.computedValue(gain10, val);

			System.out.println("ADC value: " + val + ", Gain 3: " + gain3val + "v, Gain 10: "
					+ gain10val);

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}