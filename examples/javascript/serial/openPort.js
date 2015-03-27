// require mraa library
var mraa = require('mraa');
mraa.init();
var port = new mraa.Uart(0).getDevicePath();

// require serial port library
var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var serial = new SerialPort(port, { baudrate: 115200 }, false);

// open the port
serial.open(function (error) {
  if (error) {
    console.log('Failed to open: '+error);
  } else {
    console.log('Opened port: '+port);
    serial.close();
  }
});
