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

// require file system library
var fs = require("fs");
var fileName = "/home/root/test.jpg";

// OV528 protocol commands
var cmd = {
  sync: [0xaa, 0x0d, 0x00, 0x00, 0x00, 0x00],
  init: [0xaa, 0x01, 0x00, 0x07, 0x00, 0x03], /* JPEG, no preview, 640x480 */
  pkg:  [0xaa, 0x06, 0x08, 0x00, 0x00, 0x00], /* low byte, high byte, don't care */
  snap: [0xaa, 0x05, 0x00, 0x00, 0x00, 0x00], /* compressed, no skip frame, don't care */
  get:  [0xaa, 0x04, 0x01, 0x00, 0x00, 0x00],
  data: [0xaa, 0x08, 0x00, 0x00, 0x00, 0x00],
  ackEnd: [0xaa, 0x0e, 0x00, 0x00, 0xf0, 0xf0]
};

//--------------------------------------------------------------
// open the specified port
serial.open(function (error) {
  if (error) {
    console.log('Failed to open: '+error);
  } else {
    console.log('Opened port: '+port);

    //fs.open(fileName, 'w', function(error, fd) {
      /*
      var ws = fs.createWriteStream(fileName, {
        fd: fd
      });
      */

      // discard existing responses
      // and initialize the connected module
      serial.flush(function() {
        sync(function(result) {
          console.log('SYNC:', result);
          if (!result) {
            serial.close();
            //ws.end(); fd.close();
            return;
          }

          initialize(function(result) {
            console.log('INIT:', result);
            if (!result) {
              serial.close();
              //ws.end(); fd.close();
              return;
            }

            setPackageSize(512, function(result) {
              console.log('PKG:', result);
              if (!result) {
                serial.close();
                //ws.end(); fd.close();
                return;
              }

              snapshot(function(result) {
                console.log('SNAP:', result);
                if (!result) {
                  serial.close();
                  //ws.end(); fd.close();
                  return;
                }

                var read = 0;
                var len = 0;
                getPicture(
                  /* init */
                  function(length) {
                    console.log('GET:', length);
                    if (length < 0) {
                      serial.close();
                      //ws.end(); fd.close();
                      return;
                    }
                    len = length;
                  },
                  /* write */
                  function(buf) {
                    read += buf.length;
                    //ws.write(buf);
                  },
                  /* end */
                  function() {
                    //ws.end(); fd.close();
                  });
              });
            });
          });
        });
      });
    //});
  }
});

//--------------------------------------------------------------
// synchronize the connection
var numTrials = 0, maxTrials = 100;
function sync(callback) {

  // check response
  var ack = false;
  var sync = false;
  var checkResp = function(resp) {

    // wait for an ACK response
    if (checkAck(resp, cmd.sync[1])) {
      ack = true;
      console.log('Got ACK response.');
      setTimeout(function() {
        if (sync) return;
        serial.removeListener('data', checkResp);
        callback(false);
        console.log('No SYNC response.');
      }, 500);
      return;
    }

    // then, wait for the following SYNC response
    if (checkSync(resp)) {
      serial.removeListener('data', checkResp);
      sendAck(resp);
      serial.flush(function() {
        callback(true);
      });
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
      console.log('Reached maximum SYNC trials.');
      callback(false);
      return;
    }
    numTrials++;
    serial.write(syncBuf);
    setTimeout(sync, 10); // retry
  }
  sync();
}

//--------------------------------------------------------------
// initialize camera configuration
function initialize(callback) {

  // check response
  var resp = false;
  var checkResp = function(resp) {
    if (checkAck(resp, cmd.init[1])) {
      serial.removeListener('data', checkResp);
      resp = true;
      serial.flush(function() {
        callback(true);
      });
      console.log('Got Initial response.');
    }
  };
  serial.addListener('data', checkResp);

  // send Initial
  serial.write(new Buffer(cmd.init));

  // set timeout for this request
  setTimeout(function() {
    if (resp) return;
    serial.removeListener('data', checkResp);
    console.log('No Initial response.');
    callback(false);
  }, 500);
}

//--------------------------------------------------------------
// set JPEG package size
function setPackageSize(packageSize, callback) {

  // check response
  var resp = false;
  var checkResp = function(resp) {
    if (checkAck(resp, cmd.pkg[1])) {
      serial.removeListener('data', checkResp);
      resp = true;
      serial.flush(function() {
        callback(true);
      });
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
  setTimeout(function() {
    if (resp) return;
    serial.removeListener('data', checkResp);
    console.log('No Set Package Size response.');
    callback(false);
  }, 500);
}

//--------------------------------------------------------------
// take a snapshot
function snapshot(callback) {

  // check response
  var resp = false;
  var checkResp = function(resp) {
    if (checkAck(resp, cmd.snap[1])) {
      serial.removeListener('data', checkResp);
      resp = true;
      serial.flush(function() {
        callback(true);
      });
      console.log('Got Snapshot response.');
    }
  };
  serial.addListener('data', checkResp);

  // send Snapshot
  serial.write(new Buffer(cmd.snap));

  // set timeout for this request
  setTimeout(function() {
    if (resp) return;
    serial.removeListener('data', checkResp);
    console.log('No Snapshot response.');
    callback(false);
  }, 500);
}

//--------------------------------------------------------------
// get JPEG data
function getPicture(callback, onDataRead, onDataFinished) {

  // check response
  var resp = false;
  var data = false;
  var length = 0;
  var read = 0;
  var checkResp = function(resp) {

    // wait for an ACK response
    if (!resp) {
      if (checkAck(resp, cmd.get[1])) {
        resp = true;
        console.log('Got Get Picture response.');
      }
      return;
    }

    // then, wait for the following Data response
    if (!data) {
      if (checkAck(resp, cmd.data[1])) {
        serial.removeListener('data', checkResp);
        var data = true;
        console.log('Got Data response.');
        length = resp[3] | (resp[4] << 8) | (resp[5] << 16);
        callback(length);
      }
    }

    read += resp.length;
    onDataRead(resp);
    if (read >= length) {
      serial.write(cmd.ackEnd);
      onDataFinished();
    }
  };
  serial.addListener('data', checkResp);

  // send Snapshot
  serial.write(new Buffer(cmd.get));

  // set timeout for this request
  setTimeout(function() {
    if (resp && data) return;
    serial.removeListener('data', checkResp);
    if (!resp) console.log('No Get Picture response.');
    else console.log('No Data response after the Get Picture response.');
    callback(-1);
  }, 500);
}

//--------------------------------------------------------------
// utility methods
function checkAck(resp, cmd) {
  for (var i = 0; i + 5 < resp.length; i ++)
    if (resp[i] == 0xaa && resp[i + 1] == 0x0e && resp[i + 2] == cmd)
      return true;
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
