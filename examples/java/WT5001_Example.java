import java.io.IOException;

/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//NOT TESTED!!!
public class WT5001_Example {

	static private void printUsage() {
		System.out.println("Usage: java WT5001_Example <command>");
		System.out.println("Commands:");
		System.out.println("0  - stop playing");
		System.out.println("1  - start playing track 1");
		System.out.println("2  - pause/un-pause playback");
		System.out.println("3  - next track");
		System.out.println("4  - previous track");
	}

	public static void main(String[] args) {
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
		try {
			short vol;
			vol = mp3.getVolume();
			System.out.println("The current volume is: " + vol);
		} catch (IOException e) {
			System.out.println(e.toString());
		}

		try {
			short ps;
			ps = mp3.getPlayState();
			System.out.println("The current play state is: " + ps);
		} catch (IOException e) {
			System.out.println(e.toString());
		}

		try {
			int numf;
			numf = mp3.getNumFiles(upm_wt5001.WT5001.WT5001_PLAYSOURCE_T.SD);
			System.out.println("The number of files on the SD card is: " + numf);
		} catch (IOException e) {
			System.out.println(e.toString());
		}

		try {
			int curf;
			curf = mp3.getCurrentFile();
			System.out.println("The current file is: " + curf);
		} catch (IOException e) {
			System.out.println(e.toString());
		}

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