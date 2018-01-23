<p align="center">
  <img src="https://github.com/intel-iot-devkit/upm/blob/master/docs/icons/upm_logo.png" height="150px" width="auto" algt="UPM Logo"/>
</p>

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

We endorse additions that implement the generic C and C++ interfaces provided
with the libraries. Multiple sensor and actuator types have been defined, for
instance:

* Light controller 
* Light sensor
* Temperature sensor
* Humidity sensor
* Pressure sensor
* Gas sensor
* Analog to digital converter

The developer community is welcome to submit feedback on existing categories or
suggest new ones.

### Example

A sensor/actuator is expected to work as such (here is the MMA7660 accelerometer API):
```C++
  // Instantiate an MMA7660 on I2C bus 0
  upm::MMA7660 *accel = new upm::MMA7660(MMA7660_DEFAULT_I2C_BUS,
                                         MMA7660_DEFAULT_I2C_ADDR);

  // place device in standby mode so we can write registers
  accel->setModeStandby();

  // enable 64 samples per second
  accel->setSampleRate(MMA7660_AUTOSLEEP_64);

  // place device into active mode
  accel->setModeActive();

  while (shouldRun)
    {
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

You can also refer to the [Intel® IoT Developer Zone](https://software.intel.com/iot/hardware/sensors).

### IDE Support

The UPM sensor libraries are directly supported by the IDEs listed on the Intel®
Developer Zone Tools & IDEs page.

<a href="https://software.intel.com/iot/tools"><img src="docs/icons/iss.png"/></a>

Intel® System Studio integration offers IoT specific features such as a sensor explorer,
library sync tools and the ability to easily import existing projects and samples that
use the UPM libraries. For further details please refer to the IoT User Guides on IDZ.

### Installing UPM

Find notes on how to install UPM on various OS'es on this [page](docs/installing.md).

### Building UPM

See building documentation [here](docs/building.md).

[![Build Status](https://travis-ci.org/intel-iot-devkit/upm.svg?branch=master)](https://travis-ci.org/intel-iot-devkit/upm)

### Making your own UPM module

A quick way to add a new sensor driver is to port existing code from another
platform (e.g. Arduino) and swap the IO calls to the MRAA API. This of course
assumes either ownership of the original code or licensing that allows
unrestricted redistribution.

The [porting](docs/porting.md) section has more information on this process,
and there is an example available based on the max31855 [sensor](docs/max31855.md).

Read more on creating Java [bindings](docs/creating_java_bindings.md) for your
new driver.

### Guidelines and rules for new UPM contributions

Before you begin development, take a look at our naming [conventions](docs/naming.md).
The name you pick for a newly added sensor needs to be unique in the UPM library.

Then, please go over this short set of rules for new [contributions](docs/contributions.md).
Make sure you add yourself as an author on every new code file submitted.
If you are providing a fix with significant changes, feel free to add yourself
as a contributor. Signing-off your commits is mandatory.

Documenting your code is also a big part of the task. We have a strict set of
tags used to classify our sensors and their capabilities. You can find out more
about this in our [section](docs/documentation.md) on documenting a sensor API.
Finally, if you really want to ensure consistency with the rest of the library,
and the intel-iot-devkit repositories in general, take a look at our extensive
[author guide](docs/guidelines.md).

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

**NOTE** - Several important API changes are currently underway for some of our
widely used libraries including `libupm-grove`

### Changelog
Version changelog [here](docs/changelog.md).

### Known Limitations
List of known limitations [here](docs/knownlimitations.md).
