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
public class WT5001Sample {

	static {
		try {
			System.loadLibrary("javaupm_wt5001");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	static private void printUsage() {
		System.out.println("Usage: java WT5001Sample <command>");
		System.out.println("Commands:");
		System.out.println("0  - stop playing");
		System.out.println("1  - start playing track 1");
		System.out.println("2  - pause/un-pause playback");
		System.out.println("3  - next track");
		System.out.println("4  - previous track");
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a WT5001 serial MP3 player on uart 0
		upm_wt5001.WT5001 mp3 = new upm_wt5001.WT5001(0);

		int cmd = -1;
		if (args.length > 0)
			cmd = Integer.parseInt(args[0]);

		// make sure port is initialized properly. 9600 baud is the default
		if (!mp3.setupTty()) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}

		switch (cmd) {
			case 0 :
				mp3.stop();
				break;

			case 1 :
				mp3.play(upm_wt5001.WT5001.WT5001_PLAYSOURCE_T.SD, 1);
				break;

			case 2 :
				mp3.pause();
				break;

			case 3 :
				mp3.next();
				break;

			case 4 :
				mp3.previous();
				break;

			default :
				// nothing, just output usage, and info below
				printUsage();
				break;
		}

		// print out some information
		short vol[] = new short[1];
		if (mp3.getVolume(vol))
			System.out.println("The current volume is: " + vol[0]);

		short ps[] = new short[1];
		if (mp3.getPlayState(ps))
			System.out.println("The current play state is: " + ps[0]);

		int numf[] = new int[1];
		if (mp3.getNumFiles(upm_wt5001.WT5001.WT5001_PLAYSOURCE_T.SD, numf))
			System.out.println("The number of files on the SD card is: " + numf[0]);

		int curf[] = new int[1];
		if (mp3.getCurrentFile(curf))
			System.out.println("The current file is: " + curf[0]);

		int year[] = new int[1];
		short month[] = new short[1];
		short day[] = new short[1];
		if (mp3.getDate(year, month, day))
			System.out.println("The device date is: " + year[0] + "/" + month[0] + "/" + day[0]);

		short hour[] = new short[1];
		short minute[] = new short[1];
		short second[] = new short[1];
		if (mp3.getTime(hour, minute, second))
			System.out
					.println("The device time is: " + hour[0] + ":" + minute[0] + ":" + second[0]);
		// ! [Interesting]
	}

}