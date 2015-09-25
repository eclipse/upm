UPM (Useful Packages & Modules) Sensor/Actuator repository for MRAA
==============

UPM is a high level repository for sensors that use MRAA. Each sensor links
to MRAA and are not meant to be interlinked although some groups of sensors
may be. Each sensor contains a header which allows to interface with it.
Typically a sensor is represented as a class and instantiated.

The constructor is expected to initialise the sensor and parameters may be used
to provide identification/pin location on the board.

Typically an update() function will be called in order to get new data from the
sensor in order to reduce load when doing multiple reads to sensor data.

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

However implementation and API design is completely up to the developer, some
enumerable sensors for example may provide much clever instantiation. Displays
may also create more complex structures in order to interface with them.

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

### Making your own UPM module

Porting [link](docs/porting.md) has more information on making new UPM modules.

There is also an example available for max31855 [sensor](docs/max31855.md).

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

### Changelog
Version changelog [here](docs/changelog.md).

### Known Limitations
List of known limitations [here](docs/knownlimitations.md).
