UPM (Useful Packages & Modules) Sensor/Actuator repository for MRAA
==============

The UPM repository provides software drivers for a wide variety of commonly
used sensors and actuators. These software drivers interact with the
underlying hardware platform (or microcontroller), as well as with the attached
sensors, through calls to [MRAA](https://github.com/intel-iot-devkit/mraa) APIs.

Programmers can access the interfaces for each sensor by including the sensor’s
corresponding header file and instantiating the associated sensor class. In the
typical use case, a constructor initializes the sensor based on parameters that
identify the sensor, the I/O protocol used and the pin location of the sensor.

C++ interfaces have been defined for the following sensor/actuator types, but
they are subject to change:

* Light controller 
* Light sensor
* Temperature sensor
* Humidity sensor
* Pressure sensor
* Gas sensor
* Analog to digital converter

The developer community is encouraged to help expand the list of supported
sensors and actuators and provide feedback on interface design.

### Example

A sensor/actuator is expected to work as such (here is the MMA7660 accelerometer API):
```C++
  // Instantiate an MMA7660 on I2C bus 0
  upm::MMA7660 *accel = new upm::MMA7660(MMA7660_I2C_BUS,
                                         MMA7660_DEFAULT_I2C_ADDR);

  // place device in standby mode so we can write registers
  accel->setModeStandby();

  // enable 64 samples per second
  accel->setSampleRate(upm::MMA7660::AUTOSLEEP_64);
  
  // place device into active mode
  accel->setModeActive();

  while (shouldRun)
    {
      int x, y, z;
      
      accel->getRawValues(&x, &y, &z);
      cout << "Raw values: x = " << x 
           << " y = " << y
           << " z = " << z
           << endl;
      
      float ax, ay, az;
      
      accel->getAcceleration(&ax, &ay, &az);
      cout << "Acceleration: x = " << ax 
           << "g y = " << ay
           << "g z = " << az
           << "g" << endl;
      
      usleep(500000);
    }
```

Browse through the list of all [examples](https://github.com/intel-iot-devkit/upm/tree/master/examples).

Multi-sensor samples for the starter and specialized kits can be found in the
[iot-devkit-samples](https://github.com/intel-iot-devkit/iot-devkit-samples) repository.

### Supported Sensors

Supported [sensor list](http://iotdk.intel.com/docs/master/upm/modules.html) from API documentation.

You can also refer to the [Intel® IoT Developer Zone](https://software.intel.com/iot/sensors).

### IDE Integration

If you would like to create projects and run the UPM samples using an Intel recommended IDE,
please refer to the Intel Developer Zone IDE page.

<a href="https://software.intel.com/iot/software/ide"><img src="docs/icons/allides.png"/></a>

### Building UPM

See building documentation [here](docs/building.md).

[![Build Status](https://travis-ci.org/intel-iot-devkit/upm.svg?branch=master)](https://travis-ci.org/intel-iot-devkit/upm)

### Making your own UPM module

Porting [link](docs/porting.md) has more information on making new UPM modules.

There is also an example available gfor max31855 [sensor](docs/max31855.md).

Guide on creating Java [bindings](docs/creating_java_bindings.md).

### Naming conventions and rules for new UPM contributions

Before you begin development, take a look at our naming [conventions](docs/naming.md).

Also, please read the guidelines for contributions [to UPM](docs/contributions.md).

Don't forget to check the documentation [section](docs/documentation.md).

Make sure you add yourself as an author on every new code file submitted.
If you are providing a fix with significant changes, feel free to add yourself
as a contributor. Signing-off your commits is mandatory.

API Documentation
==============

<a href="http://iotdk.intel.com/docs/master/upm"><img src="docs/icons/c++.png"/></a>
<a href="http://iotdk.intel.com/docs/master/upm/java"><img src="docs/icons/java.png"/></a>
<a href="http://iotdk.intel.com/docs/master/upm/python"><img src="docs/icons/python.png"/></a>
<a href="http://iotdk.intel.com/docs/master/upm/node"><img src="docs/icons/node.png"/></a>

### API Compatibility
Even if we try our best not to, every once in a while we are forced to modify
our API in a way that will break backwards compatibility. If you find yourself
unable to compile code that was working fine before a library update, make sure
you check the [API changes](docs/apichanges.md) section first.

**NOTE** - Our **C++ header files** changed extension from *.h* to *.hpp*!

### Changelog
Version changelog [here](docs/changelog.md).

### Known Limitations
List of known limitations [here](docs/knownlimitations.md).
