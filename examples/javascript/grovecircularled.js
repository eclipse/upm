// Grove Circular LED module usage example
var GCL = require("jsupm_grovecircularled");
var circle = new GCL.GroveCircularLED(5, 4);
var level = 0;

// Show a spinner.
var myInterval = setInterval(function()
{
	circle.setSpinner(level);
	console.log("spinner position: " + level);
	level = (level + 1) % 24;
	//circle.setLevel(level);
	//console.log("level: " + level);
	//level = (level + 1) % 25;
}, 60);

// When exiting, clear interval and print message.
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	console.log("Exiting...");
	circle.setLevel(0);
	process.exit(0);
});
