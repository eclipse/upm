/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */

var upmled = require("jsupm_grove");
var myled = new upmled.GroveLed(2);
var flag = true;
function blink()
{
    if (flag)
        myled.on();
    else
        myled.off();
    flag = !flag; //toggle between states.
}
setInterval(blink, 1000);
