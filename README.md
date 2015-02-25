UPM - Sensor/Actuator repository for libmraa
==============

UPM is a high level repository for sensors that use libmraa. Each sensor links
to libmraa and are not meant to be interlinked although some groups of sensors
may be. Each sensor contains a header which allows to interface with it.
Typically a sensor is represented as a class and instantiated.

The constructor is expected to initialise the sensor and parameters may be used
to provide identification/pin location on the board.

Typically an update() function will be called in order to get new data from the
sensor in order to reduce load when doing multiple reads to sensor data.

### Example

A sensor/actuator is expected to work as such (here is the servo ES08A API):
@snippet es08a.cxx Interesting

However implementation and API design is completely up to the developer, some
enumerable sensors for example may provide much clever instantiation. Displays
may also create more complex structures in order to interface with them.

Browse through the list of all [examples](https://github.com/intel-iot-devkit/upm/tree/master/examples).

### Supported Sensors

Supported [sensor list](http://iotdk.intel.com/docs/master/upm/modules.html) from API documentation.

You can also refer to the [Intel® IoT Developer Zone](https://software.intel.com/iot/sensors).

### Building UPM

See @ref building [here](docs/building.md).

### Making your own UPM module

@ref porting [link](docs/porting.md) has more information on making new UPM modules.

There is also an example available for @ref max31855 [sensor](docs/max31855.md).

### Naming conventions and rules for new UPM contributions

Before you begin development, take a look at our @ref naming [conventions](docs/naming.md).

Also, please read the guidelines for @ref contributions [to UPM](docs/contributions.md).

Make sure you add yourself as an author on every new code file submitted.
If you are providing a fix with significant changes, feel free to add yourself
as a contributor. Signing-off your commits is mandatory.

C/C++ API Documentation
==============

The C/C++ documentation is available [here](http://iotdk.intel.com/docs/master/upm/).
