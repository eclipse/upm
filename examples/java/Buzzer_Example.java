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

import upm_buzzer.Buzzer;
public class Buzzer_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		int chord[] = {
				upm_buzzer.javaupm_buzzer.BUZZER_DO,
				upm_buzzer.javaupm_buzzer.BUZZER_RE,
				upm_buzzer.javaupm_buzzer.BUZZER_MI,
				upm_buzzer.javaupm_buzzer.BUZZER_FA,
				upm_buzzer.javaupm_buzzer.BUZZER_SOL,
				upm_buzzer.javaupm_buzzer.BUZZER_LA,
				upm_buzzer.javaupm_buzzer.BUZZER_SI};

		// Instantiate a buzzer on digital pin D5
		Buzzer sound = new Buzzer(5);

		// print sensor name
		System.out.println(sound.name());

		for (int i = 0; i < chord.length; i++) {
			// play each note for one half second
			int note = sound.playSound(chord[i], 500000);
			System.out.println(note);

			Thread.sleep(100);
		}
		// ! [Interesting]
		sound.stopSound();
	}

}
