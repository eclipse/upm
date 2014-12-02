/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */

//Load servo module.
var servoModule = require("jsupm_servo");

//Instantiate Servo module on digital port 5
var servo = new servoModule.Servo(5);
servo.setMinPulseWidth(600);
servo.setMaxPulseWidth(2200);

// function to initialize servo
    // timeOffset: how long after hitting "run" should we start this servo instance
    // timeInterval: how frequently should this instance run after timeOffset
    // angle: the angle for this instance
function startServo(timeOffset, timeInterval, angle)
{
    // Start running this instance after timeOffset milliseconds
    setTimeout(function()
    {
        console.log("Starting angle is " + angle);
        // run this instance every timeInterval milliseconds
        setInterval(function()
        {
            console.log(angle);
            servo.setAngle(angle);
        }, timeInterval);
    }, timeOffset);
    // timeOffset tells setTimeout when to execute the function specified in the first param
    // angle is passed as a param to the specified function
}

startServo(0, 3000, 0);		// start immediately, run every 3 seconds, go 0 degrees
startServo(1000, 3000, 90);	// start in 1 second, run every 3 seconds, go 90 degrees
startServo(2000, 3000, 180);// start in 2 seconds, run every 3 seconds, go 180 degrees
