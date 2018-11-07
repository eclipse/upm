UPM (Useful Packages & Modules) Sensor/Actuator repository for MRAA
==============

The UPM repository provides software drivers for a wide variety of commonly
used sensors and actuators. These software drivers interact with the underlying
hardware platform (or microcontroller), as well as with the attached sensors,
through calls to [MRAA](https://github.com/intel-iot-devkit/mraa) APIs.

These packages contain sources for the respective UPM modules and will compile
locally on the target using node-gyp when installed. They support Node.js 0.10
or newer.

### Installing
The UPM modules have a hard dependency on MRAA, thus you will have to install
it first. Also, make sure Node can find MRAA by updating the `NODE_PATH`
variable if necessary. A global installation is preferred:

`npm install mraa -g`

After this, to install a UPM module using npm simply type:

`npm install jsupm_<module-name> -g`

### Usage
Unfortunately, due to the way node-gyp builds MRAA and UPM, the platform
initialization hooks are lost. This means you will have to do it manually,
and require MRAA *before* you load a UPM module:

```
require('mraa');
var lib = require('jsupm_<module-name>');
```

### Supported Sensors
The full list is available on [Intel® IoT Developer Zone](https://software.intel.com/en-us/iot/hardware/sensors).

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
