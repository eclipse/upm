Changelog                         {#changelog}
===============

Here's a list summarizing some of the key undergoing changes to our library
from earlier versions:

### v0.4.0

 * Fully functional and documented JAVA language bindings for sensors with
 several examples also available
 * Added standardized exception handling support across all language bindings
 and updated sensors to use this feature
 * Sensor drivers are now available and can be installed individually using NPM
 for Node.js users
 * Continuous Integration with Travis CI ensures new contributions are built
 automatically once they are received
 * API rewrite and improvements for hcsr04, sm130, eboled, loudness
 * Documentation improvements and code style changes from C to C++
 * New sensors: hyld9767, mg811, wheelencoder, grovegprs, mpu9250, ssd1306,
 es9257, lm35, mq4, mq6, mq7, mq8

### v0.3.2

 * Introduced RPM package generation feature from cmake via cpack
 * Performed a thorough documentation review and merged resulting changes,
 includes last remaining images and grouping sensors by kits
 * Added support for Sparkfun Sensor Blocks (OLED & IMU)
 * Fully reworked API for at42qt1070, mpu9150 and lcd classes, while
 maintaining backwards compatibility for existing lcd displays.
 * Extended doxygen tags for better integration with automated documentation
 generators
 * New sensors: rgbringcoder, hp20x, pn532, sainsmartks, lsm9ds0, eboled

### v0.3.1

 * Updated index pages for API documentation and cross-linked them
 * More images were added for Grove, Adafruit and Sparkfun sensors along with
 licensing information
 * Applied a set of patches to improve code robustness
 * Added support for Grove NFC Tag sensor
 * Fixed several typos in library and sensor names
 * New sensor: m24lr64e

### v0.3.0

 * Provided automated, updated and comprehensive sensor API documentation for
 C++, Python and JavaScript
 * Edited sensor names, library descriptions and doxygen tags for consistency
 * Fixed the TH02 sensor driver and made major improvements to i2clcd driver
 * Added new and updated images for numerous new and existing sensors
 * Started this changelog
 * New sensors: sx6119, si114x, maxsonarez, hm11, ht9170, h3lis331dl, ad8232,
 grovescam

### v0.2.0

 * Passed 100 supported sensor drivers
 * Updated header files with new doxygen tags for improved API documentation
 generation
 * Added python examples for sensors
 * Fixed the buzzer and servo drivers for the Intel Edison
 * Renamed bmp and gp2y sensor classes to be more generic
 * Completely revamped implementation for tm1637
 * New sensors: mma7660, cjq4435, adxl335, hmtrp, nunchuck, otp538u, l298,
 groveelectromagnet, grovecollision, adis16448, at42qt1070, grovemd, ina132,
 grovegsr, groveo2, groveemg, pca9685, pca9685ss, adafruitms1438, hx711, flex,
 groveeldriver, mhz16, apds9002, waterlevel, zfm20, uln200xa, grovewfs, isd1820

### v0.1.9

 * Passed 50 supported sensor drivers
 * Starter Kit and Additional Roadshow and Hackaton sensors added
 * Several examples and documentation updates submitted
 * Wrote new proper SWIG typemaps for arrays
 * Fixed the i2clcd write function, improved several other sensors
 * New sensors: rpr220, rotaryencoder, biss0001, rfr359f, grovespeaker, mq303a,
 ppd42ns, wt5001, yg1006, ublox6, mpr121, groveloudness, guvas12d, grovewater,
 grovevdiv, grovelinefinder, ta12200, groveehr, grovemoisture, gp2y0a, a110x,
 ds1307, adc121c021, enc03r, ttp223, itg3200, am2315, tp401, ldt0028, htu21d,
 mpl3115a2, lsm303, joystick12, tsl2561, groverotary, groveslider, adxl345,
 grovebutton, lol, groverelay

### v0.1.8

 * Added MQ series gas sensors and a few others
 * Generated Doxygen layout file to improve API look and feel
 * Several sensor documentation updates

### v0.1.7

 * Fixed SWIG node build issues
 * Added lpd8806 digital led strip

### v0.1.6

 * Fixed SWIG python builds
 * Extended i2clcd devices with new functions
 * Made SWIG interface improvements and added extra types
 * Added nrf8001 BLE support
 * Few other sensor additions and fixes

### v0.1.5

 * New drivers for microphone and mpu9150
 * Added generic driver for stepper motors
 * Updated all code to use MRAA and version dependency

### v0.1.4

 * Added new bmp/gy pressure sensor and updated servo
 * Started UPM documentation and related pages
 * Updated MRAA dependencies

### v0.1.3

 * Updated SWIG support for some sensors
 * New sensor support for mma7455 accelerometer

### v0.1.2

 * Several new sensors added
 * Documentation fixes
 * IPK generation

### v0.1.1

 * Updated MRAA dependency

### v0.1.0

 * Initial release with some basic grove sensors and hmc5883
 * Doxygen and SWIG support implemented
