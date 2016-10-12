UPM (Useful Packages & Modules) Sensor/Actuator repository for MRAA
==============

The UPM repository provides software drivers for a wide variety of commonly
used sensors and actuators. These software drivers interact with the underlying
hardware platform (or microcontroller), as well as with the attached sensors,
through calls to [MRAA](https://github.com/intel-iot-devkit/mraa) APIs.

These bindings are precompiled with Node.js 4.4.

### Installing
To install a UPM module using npm simply type:

`npm install jsupm_<module-name>`

### Supported Sensors
The full list is available on [Intel® IoT Developer Zone](https://software.intel.com/iot/sensors).

### IDE Integration
Intel® XDK IoT Edition is the recommended IDE for Node.js development. Visit
the Intel® Developer Zone [IDE page](https://software.intel.com/iot/software/ide)
for more information on how to get started.

![Intel XDK](http://iotdk.intel.com/docs/master/upm/docs/icons/xdk.png)

### Building UPM
See building documentation [here](https://github.com/intel-iot-devkit/upm/blob/master/docs/building.md).

### Making your own UPM module
Our porting [link](https://github.com/intel-iot-devkit/upm/blob/master/docs/porting.md)
has more information on making new UPM modules. Also check out our main Github page as
we do have some guidelines if you would like to contribute your modules to the library.

### API Documentation
Our Node.js documentation pages are hosted [here](http://iotdk.intel.com/docs/master/upm/node/).
For other supported languages and their APIs please visit our main Github page.

### API Compatibility
Even if we try our best not to, every once in a while we are forced to modify
our API in a way that will break backwards compatibility. If you find yourself
unable to run code that was working fine before a library update, make sure
you check the [API changes](https://github.com/intel-iot-devkit/upm/blob/master/docs/apichanges.md) section first.

### Changelog
Version changelog [here](https://github.com/intel-iot-devkit/upm/blob/master/docs/changelog.md).

### Known Limitations
List of known limitations [here](https://github.com/intel-iot-devkit/upm/blob/master/docs/knownlimitations.md).
