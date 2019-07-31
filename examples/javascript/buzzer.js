/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var upmBuzzer = require("jsupm_buzzer");
// Initialize on GPIO 5
var myBuzzer = new upmBuzzer.Buzzer(5);
var chords = [];
chords.push(upmBuzzer.BUZZER_DO);
chords.push(upmBuzzer.BUZZER_RE);
chords.push(upmBuzzer.BUZZER_MI);
chords.push(upmBuzzer.BUZZER_FA);
chords.push(upmBuzzer.BUZZER_SOL);
chords.push(upmBuzzer.BUZZER_LA);
chords.push(upmBuzzer.BUZZER_SI);
var chordIndex = 0;

// Print sensor name
console.log(myBuzzer.name());

function melody()
{
    if (chords.length != 0)
    {
        //Play sound for one half second
        console.log( myBuzzer.playSound(chords[chordIndex], 500000) );
        chordIndex++;
        //Reset the sound to start from the beginning. 
        if (chordIndex > chords.length - 1)
			chordIndex = 0;
    }
}
setInterval(melody, 100);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});
