/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */

var upmBuzzer = require("jsupm_buzzer");
var myBuzzer = new upmBuzzer.Buzzer(5);
var chord = [upmBuzzer.DO, upmBuzzer.RE, upmBuzzer.MI, upmBuzzer.FA, upmBuzzer.SOL, upmBuzzer.LA, upmBuzzer.SI, upmBuzzer.DO, upmBuzzer.SI];
var chordIndex = 0;
function melody()
{
    if (chord.length != 0)
    {
        //Play sound
        myBuzzer.playSound(chord[chordIndex], 100000);
        chordIndex++;
        //Reset the sound to start from the beginning. 
        if (chordIndex > chord.length - 1) chordIndex = 0;
    }
}
setInterval(melody, 100);
