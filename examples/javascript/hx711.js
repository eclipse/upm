var hx711 = require('jsupm_hx711');
var scale = new hx711.HX711(3, 2);

setTimeout(function(){
	scale.setScale(2837);
	scale.tare(2);
	setInterval(function(){
		console.log(scale.getUnits());
	}, 1000);
}, 1000);
