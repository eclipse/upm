
var ultrasonic = require("jsupm_groveultrasonic");
var sensor = new ultrasonic.GroveUltraSonic(2);

var myInterval = setInterval(function()
{
    var travelTime = sensor.getDistance();
    if (travelTime > 0) {
        var distance = (travelTime / 29 / 2).toFixed(3);
        console.log("distance: " + distance + " [cm]");
    }
}, 200);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});
