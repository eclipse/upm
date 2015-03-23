
var GCL = require("jsupm_grovecircularled");
var circle = new GCL.GroveCircularLED(5, 4);
var level = 0;

var myInterval = setInterval(function()
{
	circle.setLevel(level);
	console.log("level: " + level);
	level = (level + 1) % 25;
}, 4);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	process.exit(0);
});
