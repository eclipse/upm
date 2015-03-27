// require mraa library
var mraa = require('mraa');
mraa.init();
var port = new mraa.Uart(0).getDevicePath();

// require serial port library
var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var serial = new SerialPort(port, {
  baudrate: 115200
}, false);

// require async
var async = require("async");

// require file system library
var fs = require("fs");
var fileName = "/home/root/test.jpg";

// OV528 protocol commands
var cmd = {
  sync: [0xaa, 0x0d, 0x00, 0x00, 0x00, 0x00],
  init: [0xaa, 0x01, 0x00, 0x07, 0x00, 0x07],
  pkg:  [0xaa, 0x06, 0x08, 0x00, 0x00, 0x00],
  snap: [0xaa, 0x05, 0x00, 0x00, 0x00, 0x00],
  get:  [0xaa, 0x04, 0x01, 0x00, 0x00, 0x00],
  data: [0xaa, 0x0a, 0x00, 0x00, 0x00, 0x00],
  dataAck: [0xaa, 0x0e, 0x00, 0x00, 0xf0, 0xf0]
};

// main routine: open the specified port and take a picture
var packageSize = 256;
async.series([
    function(c) { fs.open(fileName, 'w', c); },
    function(c) { serial.open(c); },
    sync,
    initialize,
    function(c){ setPackageSize(packageSize, c); },
    snapshot,
    getPicture
  ], writeResults);

// check results
function writeResults(err, results) {
  var fd = results[0];
  var ws = fs.createWriteStream(fileName, { fd: fd });
  var size = results[results.length - 1], read = 0, packageId = 0;
  var dataAckBuf = new Buffer(cmd.dataAck);

  var checkResp = function(resp) {
    var pid = resp[0] | (resp[1] << 8);
    if (pid != packageId) return;
    ws.write(resp.slice(4, resp.length - 2));
    packageId ++;

    read += resp.length - 6;
    if (read >= size) {
      dataAckBuf[4] = dataAckBuf[5] = 0xf0;
      serial.write(dataAckBuf);
      serial.removeListener('data', checkResp);
      ws.end();
      serial.close();
      return;
    }
    sendDataAck();
  };
  serial.addListener('data', checkResp);

  // send Ack for the received package
  var sendDataAck = function() {
    dataAckBuf[4] = packageId & 0xff;
    dataAckBuf[5] = (packageId >> 8) & 0xff;
    //console.log("Req", dataAckBuf);
    serial.write(dataAckBuf);
    var pId = packageId;
    setTimeout(function() {
      if (packageId > pId) return;
      sendDataAck();
    }, 100);
  }
  sendDataAck();
}

// sync connection
function sync(callback) {
  var numTrials = 0, maxTrials = 100;

  // check response
  var ack = false;
  var sync = false;
  var t;
  var checkResp = function(resp) {

    // wait for ACK
    if (checkAck(resp, cmd.sync[1])) {
      ack = true;
      console.log('Got ACK response.');
      t = setTimeout(function() {
        if (sync) return;
        serial.removeListener('data', checkResp);
        callback('No SYNC response.', false);
      }, 500);
      return;
    }

    // then, wait for the following SYNC response
    if (checkSync(resp)) {
      serial.removeListener('data', checkResp);
      sendAck(resp);
      serial.flush(function() {
        callback(null, true);
      });
      clearTimeout(t);
      console.log('Got SYNC response.');
    }
  };
  serial.addListener('data', checkResp);

  // send SYNC
  var syncBuf = new Buffer(cmd.sync);
  var sync = function() {
    if (ack) {
      return;
    }
    if (numTrials >= maxTrials) {
      callback('Reached maximum SYNC trials.', false);
      return;
    }
    numTrials++;
    serial.write(syncBuf);
    setTimeout(sync, 10); // retry
  }
  sync();
}

// setup camera
function initialize(callback) {

  // check response
  var recv = false;
  var checkResp = function(resp) {
    if (checkAck(resp, cmd.init[1])) {
      serial.removeListener('data', checkResp);
      recv = true;
      serial.flush(function() {
        callback(null, true);
      });
      clearTimeout(t);
      console.log('Got Initial response.');
    }
  };
  serial.addListener('data', checkResp);

  // send Initial
  serial.write(new Buffer(cmd.init));

  // set timeout for this request
  var t = setTimeout(function() {
    if (recv) return;
    serial.removeListener('data', checkResp);
    callback('No Initial response.', false);
  }, 1000);
}

// set JPEG package size
function setPackageSize(packageSize, callback) {

  // check response
  var recv = false;
  var checkResp = function(resp) {
    if (checkAck(resp, cmd.pkg[1])) {
      serial.removeListener('data', checkResp);
      recv = true;
      serial.flush(function() {
        callback(null, true);
      });
      clearTimeout(t);
      console.log('Got Set Package Size response.');
    }
  };
  serial.addListener('data', checkResp);

  // send Set Package Size
  var c = cmd.pkg.slice();
  c[3] = packageSize & 0xff;
  c[4] = (packageSize>>8) & 0xff;
  serial.write(new Buffer(c));

  // set timeout for this request
  var t = setTimeout(function() {
    if (recv) return;
    serial.removeListener('data', checkResp);
    callback('No Set Package Size response.', false);
  }, 1000);
}

// take a snapshot
function snapshot(callback) {

  // check response
  var recv = false;
  var checkResp = function(resp) {
    if (checkAck(resp, cmd.snap[1])) {
      serial.removeListener('data', checkResp);
      recv = true;
      serial.flush(function() {
        callback(null, true);
      });
      clearTimeout(t);
      console.log('Got Snapshot response.');
    }
  };
  serial.addListener('data', checkResp);

  // send Snapshot
  serial.write(new Buffer(cmd.snap));

  // set timeout for this request
  var t = setTimeout(function() {
    if (recv) return;
    serial.removeListener('data', checkResp);
    callback('No Snapshot response.', false);
  }, 1000);
}

// get JPEG data
function getPicture(callback) {
  var numGetPictureTrials = 0, maxGetPictureTrials = 1000;

  // check response
  var recv = false;
  var data = false;
  var t = -1;
  var checkResp = function(resp) {

    // wait for an ACK recvonse
    if (checkAck(resp, cmd.get[1])) {
      recv = true;
      console.log('Got Get Picture response.');
      if (t < 0) {
        t = setTimeout(function() {
          if (data) return;
          console.log('No Data response after the Get Picture response.');
          forceGet();
          t = -1;
        }, 1000);
      }
      return;
    }

    // then, wait for the following Data response
    for (var i = 0; i + 5 < resp.length; i ++)
      if (resp[i] == 0xaa && resp[i + 1] == cmd.data[1]) {
        serial.removeListener('data', checkResp);
        data = true;
        console.log('Got Data response.');
        length = resp[i+3] | (resp[i+4] << 8) | (resp[i+5] << 16);
        clearTimeout(t);
        callback(null, length);
        return;
      }
  };
  serial.addListener('data', checkResp);

  // send Get Picture
  var getBuf = new Buffer(cmd.get);
  var forceGet = function() {
    if (numGetPictureTrials >= maxGetPictureTrials) {
      callback('Reached maximum Get Picture trials.', false);
      return;
    }
    numGetPictureTrials++;
    serial.write(getBuf);
  };
  var get = function() {
    if (recv) {
      return;
    }
    forceGet();
    setTimeout(get, 10); // retry
  };
  get();
}

// utility methods
function checkAck(resp, cmd) {
  for (var i = 0; i + 5 < resp.length; i ++)
    if (resp[i] == 0xaa && resp[i + 1] == 0x0e && resp[i + 2] == cmd) {
      return true;
    }
  return false;
}

function checkSync(resp) {
  for (var i = 0; i + 5 < resp.length; i ++)
    for (var j = 0; j < 7; j ++) {
      if (j == 6) return true;
      if (resp[i + j] != cmd.sync[j]) break;
    }
  return false;
}

function sendAck(resp) {
  serial.write(new Buffer([0xaa, 0x0e, resp[1], 0x00, 0x00, 0x00]));
}
