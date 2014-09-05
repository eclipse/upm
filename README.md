UPM - Sensor/Actuator repository for Mraa
==============

UPM is a high level repository for sensors that use mraa. Each sensor links to
libmraa and are not meant to be interlinked although some groups of sensors may
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

### Supported Sensors

Temperature Sensors:
  * upm::MAX31723
  * upm::MAX31855
  * upm::TH02
  * upm::GroveTemp

Compass/Gyro/Magnometer Sensors:
  * upm::Hmc5883l
  * upm::MPU9150
  * upm::LSM303

Atmospheric Pressure Sensors:
  * upm::GY65

Light/Proximity Sensors:
  * upm::MAXDS3231M
  * upm::MAX44000
  * upm::HCSR04
  * upm::GroveLight

Gas Sensors:
  * upm::MQ2
  * upm::MQ3
  * upm::MQ5
  * upm::MQ9

Displays:
  * upm::Jhd1313m1
  * upm::Lcm1602
  * upm::ST7735
  * upm::SSD1308
  * upm::SSD1327

LED controllers/segment displays:
  * upm::TM1637
  * upm::MY9221

RFID:
  * upm::SM130

Wireless Communication:
  * upm::NRF24l01

Servo/motors:
  * upm::ES08A
  * upm::StepMotor

Digital potentiometer
  * upm::MAX5487

### Building UPM

See @ref building

### Making your own UPM module

@ref porting has more information on making new UPM modules

