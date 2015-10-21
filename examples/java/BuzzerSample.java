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

public class BuzzerSample {

	static {
		try {
			System.loadLibrary("javaupm_buzzer");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		int chord[] = {
				upm_buzzer.javaupm_buzzer.DO, 
				upm_buzzer.javaupm_buzzer.RE,
				upm_buzzer.javaupm_buzzer.MI, 
				upm_buzzer.javaupm_buzzer.FA,
				upm_buzzer.javaupm_buzzer.SOL, 
				upm_buzzer.javaupm_buzzer.LA,
				upm_buzzer.javaupm_buzzer.SI};

		// Instantiate a buzzer on digital pin D5
		upm_buzzer.Buzzer sound = new upm_buzzer.Buzzer(5);

		// print sensor name
		System.out.println(sound.name());

		for (int i = 0; i < chord.length; i++) {
			// play each note for one second
			int note = sound.playSound(chord[i], 1000000);
			System.out.println(note);

			Thread.sleep(100);
		}
		// ! [Interesting]
		sound.stopSound();
	}

}