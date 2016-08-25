/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */
/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

var upmBuzzer = require("jsupm_buzzer");
// Initialize on GPIO 5
var myBuzzer = new upmBuzzer.Buzzer(5);
var chords = [];
chords.push(upmBuzzer.DO);
chords.push(upmBuzzer.RE);
chords.push(upmBuzzer.MI);
chords.push(upmBuzzer.FA);
chords.push(upmBuzzer.SOL);
chords.push(upmBuzzer.LA);
chords.push(upmBuzzer.SI);
chords.push(upmBuzzer.DO);
chords.push(upmBuzzer.SI);
var chordIndex = 0;

// Print sensor name
console.log(myBuzzer.name());

function melody()
{
    if (chords.length != 0)
    {
        //Play sound for one second
        console.log( myBuzzer.playSound(chords[chordIndex], 1000000) );
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
