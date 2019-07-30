<p align="center">
  <img src="https://github.com/eclipse/upm/blob/master/docs/icons/upm_logo.png" height="150px" width="auto" algt="UPM Logo"/>
</p>

Eclipse UPM Sensor and Actuator Repository
==============

The Eclipse UPM repository provides software drivers for a wide variety of
commonly used sensors and actuators. These software drivers interact with the
underlying hardware platform (or microcontroller), as well as with the attached
sensors, through calls to [Eclipse MRAA](https://github.com/eclipse/mraa) APIs.

Programmers can access the interfaces for each sensor by including the sensor's
corresponding header file and instantiating the associated sensor class. In the
typical use case, a constructor initializes the sensor based on parameters that
identify the sensor, the I/O protocol used and the pin location of the sensor.
As of UPM 2.0, sensor initialization can also be done, in most cases, via
overloaded constructors that accept string identifiers.

We endorse additions that implement the generic C and C++ interfaces provided
with the libraries. With the 2.0 release, UPM introduces the following sensor
interfaces:
```
iAcceleration, iAngle, iButton, iClock, iCollision, iDistance,
iDistanceInterrupter, iEC, iElectromagnet, iEmg, iGas, iGps, iGyroscope,
iHallEffect, iHeartRate, iHumidity, iLight, iLineFinder, iMagnetometer,
iMoisture, iMotion, iOrp, iPH, iPressure, iProximity, iTemperature, iVDiv,
iWater.
```
The developer community is invited to propose new interfaces for actuator types.

The UPM project is joining the Eclipse Foundation as an Eclipse IoT project.
You can read more about this [here](https://projects.eclipse.org/proposals/eclipse-upm).

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

Browse through the list of all [examples](https://github.com/eclipse/upm/tree/master/examples).

Multi-sensor samples for starter and specialized kits can be found in the
[iot-devkit-samples](https://github.com/intel-iot-devkit/iot-devkit-samples) repository.

### Supported Sensors

Supported [sensor list](http://iotdk.intel.com/docs/master/upm/modules.html) from API documentation.

### IDE Support and More

The UPM project includes support for multiple industrial-grade sensors, actuators, radios,
protocols and standards in use today. It is also highly integrated with the Eclipse IDE 
through the help of the Foundation's partners.
Learn more about [tools](https://software.intel.com/en-us/tools-by-segment/systems-iot).

### Installing UPM

Find notes on how to install UPM on various OS'es on this [page](docs/installing.md).

### Building UPM

See building documentation [here](docs/building.md).

[![Build Status](https://travis-ci.org/intel-iot-devkit/upm.svg?branch=master)](https://travis-ci.org/intel-iot-devkit/upm)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=upm-master&metric=alert_status)](https://sonarcloud.io/dashboard?id=upm-master)

### Guidelines and rules for new UPM contributions

Before you begin development, take a look at our naming [conventions](docs/naming.md).
The name you pick for a newly added sensor needs to be unique in the UPM library.

Next, review the project's [contribution guide](docs/contributions.md).

Make sure you add yourself as an author on every new code file submitted.
If you are providing a fix with significant changes, feel free to add yourself
as a contributor. Signing-off your commits and accepting the ECA is mandatory
for making new contributions to this project.

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

### Changelog
Version changelog [here](docs/changelog.md).

### Known Limitations
List of known limitations [here](docs/knownlimitations.md).
