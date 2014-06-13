UPM - Sensor/Actuator repository for Maa
==============

UPM is a high level repository for sensors that use maa. Each sensor links to
libmaa and are not meant to be interlinked although some groups of sensors may
be. Each sensor contains a header which allows to interface with it. Typically
a sensor is represented as a class and instanciated.

The constructor is expected to initialise the sensor and paramters may be used
to provide identification/pin location on the board.

Typically an update() function will be called in order to get new data from the
sensor in order to reduce load when doing multiple reads to sensor data.

### Example

A sensor/acturo is expected to work as such (here is the servo ES08A api):
@snippet es08a.cxx Interesting

However implementation and API design is compeltely up to the developer, some
enumerable sensors for example may provide much clever instanciation. Displays
may also create more complex structures in order to interface with them.

### Building UPM

See @ref building

### Making your own UPM module

@ref porting has more information on making new UPM modules

