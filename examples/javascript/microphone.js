/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */

var upmMicrophone = require("jsupm_mic");

var myMic = new upmMicrophone.Microphone(0);
var myRealMic = myMic.getCPtr();
console.log(myRealMic);
console.log(" ");

upmMicrophone.thresholdContext.averageReading = 0;
upmMicrophone.thresholdContext.runningAverage = 0;
upmMicrophone.thresholdContext.averagedOver = 2;

for (var x in global)
	console.log(x);
console.log(" ");
for (var x in myMic)
	console.log(x);
console.log(" Moo ");
//var upmMicrophone = require("uint16Array");
//console.log(upmMicrophone.uint16Array);
var is_running = false;
while (!is_running)
{
    var buffer = new upmMicrophone.uint16Array(128);//upmMicrophone.uint16Array; //new Uint16Array(128);
    console.log("moo");
    //console.log(buffer);
//	for (var y in buffer)
//		console.log(y);
//	console.log(typeof buffer);
    //console.log(util.inspect(myMic.getSampledWindow, true, 7, true));
    var len = myMic.getSampledWindow(2, 128, buffer);
    console.log("moose");
//    var len = myMic.getSampledWindow(2, 128);
    if (len)
    {
        console.log("Len " + len);
        var thresh = myMic.findThreshold (upmMicrophone.thresholdContext, 30, buffer, len);
        myMic.printGraph(upmMicrophone.thresholdContext);
        if (thresh)
        {
            // do something ....
            console.log("Threash " + thresh);
        }
    }
}




//ctx.glyphOrientationVertical
//var ctx = new upmMicrophone.Microphone
//thresholdContext
/*
var chord = [upmBuzzer.DO, upmBuzzer.RE, upmBuzzer.MI, upmBuzzer.FA, upmBuzzer.SOL, upmBuzzer.LA, upmBuzzer.SI, upmBuzzer.DO, upmBuzzer.SI];
var chordIndex = 0;
function melody() {
    if (chord.length != 0) {
        //Play sound
        myBuzzer.playSound(chord[chordIndex], 100000);
        chordIndex++;
        //Reset the sound to start from the beginning. 
        if (chordIndex > chord.length - 1) chordIndex = 0;
        setTimeout(melody, 100); 
    }
}
melody();
*/
