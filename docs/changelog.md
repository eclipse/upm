Changelog                         {#changelog}
===============

Here's a list summarizing some of the key undergoing changes to our library
from earlier versions:

### v1.7.1

 * Bumped library compatibility to new MRAA 2.0 APIs
 * Added installation details for Fedora
 * Minor fixes based on static analysis vulnerability report

### v1.7.0

 * Added Cordova bindings based on Java packages
 * Complete rework of the nmea_gps library giving more control over data
 acquisition and parsing
 * Added GTest support for unit tests
 * Removed calls to deprecated MRAA fast GPIO functions
 * Several bug fixes and improvements around SWIG type maps and gcc8 support
 * New sensors: kx122, kxtj3, bh1792, bh1749

### v1.6.0

 * Extended LED library to support the new MRAA gpio-leds APIs
 * Many CMake changes around SWIG wrapper generation and improved FindNodejs
 detection module
 * Several code fixes based on static code analysis
 * Improved documentation generation and Travis CI builds
 * Cleaned-up doxygen tags in headers and class names in JSON library files
 * New sensor: lis3dh

### v1.5.0

 * Introduced a flexible JSON format for technical sensor specifications, notes
 and classification. This is also used by our [new UPM website](http://upm.mraa.io)
 * Revised all C++ sensor examples and switched to stack allocations where
 possible along with other code and formatting clean-up
 * Significantly improved docker workflow, CI integration, sanity and
 consistency tests, static code scans and documentation generation
 * Several improvements to a couple of existing sensor drivers and better
 compatibility with subplatforms
 * Added new std::vector to AbstractList<> typemap for Java bindings and
 examples
 * New sensors: lis2ds12, lsm6ds3h, lsm6dsl, lidarlitev3

### v1.3.0

 * Finalized all required build system and JAVA binding changes to release the
 UPM libraries for Android Things targets
 * Enhanced the modules for lsm303 and tmp006 to support newer variants of the
 chips
 * Fixed Node.js detection on Ubuntu when installed with apt using official
 packages
 * Minor documentation changes
 * New sensors: lsm303agr, lsm303d, veml6070, tca9548a, rn2903

### v1.2.0

 * Improved JAVA binding compiler compatibility and added JAVA interfaces that
 match existing C++ interfaces
 * Ported Bosch drivers to C thus now they can be used on supported MCUs
 * Completely redesigned C/C++ example handling by cmake to avoid maintaining
 a list by hand
 * Fixed several existing drivers, mostly based on static code analysis reports
 * Added a sensor driver template plus bash script that can be easily used by
 developers to start writing their own sensor driver
 * Numerous documentation improvements
 * New sensors: p9813, abp, rsc, mmc35240, tcs37727, tmp006, mma8x6x, mag3110,
 hdc1000

### v1.1.0

 * Reworked cmake handling of internal and external dependencies
 * Documentation improvements for sensor names, sensor descriptions, external
 contributor guides and added new section on installing UPM on a supported OS
 * Fixed issues with C11 builds on some WRLinux gateways and Debian systems
 * New examples and functionality for speaker and ads1x15 modules
 * Modified JAVA builds to ensure the SWIG generated wrappers include all
 functionality and typemaps
 * New sensors: mcp2515, max30100, uartat, le910

### v1.0.2

 * Minor cmake changes for Java builds and Java examples are now toggled with
 the same BUILDEXAMPLES cmake switch
 * Made some improvements in the utilities class for our C drivers
 * Fixed issue with some drivers not being usable on subplatforms due to pin
 numbers using uint8 type
 * New sensors: ims, ecezo, mb704x, rf22

### v1.0.1

 * Warnings as errors enabled for C/CXX (fixed warnings from -Wall and others)
 * Fix for C/CXX compile flag append issue #485
 * C sensor libraries for: my9221, otp538u, guvas12d, ppd42ns
 * New sensor: ms5803

### v1.0.0

 * Added approximately 50 C sources for UPM drivers that can be used on both
 Unix and RTOS boards
 * Provided generic alternatives to the Grove set of classes as they will be
 deprecated over time and eventually removed
 * UPM modules are now being packed and published on NPM as sources rather than
 binaries to enhance compatibility with various boards and systems
 * Build both python2 and python3 modules if the corresponding python libs are
 available and removed cmake BUILDPYTHON3 switch
 * Converted python examples to run on both python2 and python3
 * Added 'upm' hierarchy to python module install path which changes the way
 UPM modules are imported
    Example:
        old: import pyupm_dfrph
        new: from upm import pyupm_dfrph
             or
             import upm.pyupm_dfrph
 * Added ctests for consistancy checking UPM repo as well as sanity checking
 for python2/3 modules and examples
 * Various documentation and driver fixes throughout
 * New sensors: nmea_gps, mma7361, bh1750, hka5, dfrorp, dfrec, sht1x

### v0.8.0

 * Extended l3gd20 driver to support I2C connections in addition to IIO
 * Updated ads1x15, mcp9808 and ssd1306 I2C drivers to not throw a fatal
 exception if they fail to set a different I2C speed
 * Added extra functionality to apa102 driver, backlight control to jhd1313m1
 LCDs and fixed htu21d
 * Improved documentation with build dependencies, refreshed the list of known
 limitations and simplified the component name for a few sensors

### v0.7.3

 * Fixed several existing drivers and updated Grove Temperature sensor to use
 new formula for v1.1+ by default
 * Separated codebase for upm_grove module that bundles some of the starter kit
 sensors into individual source files
 * Documentation and doxygen tag updates with correct spelling for Celsius in
 several places, this also affects API compatibility
 * Removed superfluous BUILDSWIG option, some build recipes might be impacted
 * New sensor: ms5611

### v0.7.2

 * Mraa 1.1.1 required changes to UPM drivers and examples for IIO core
 kernel support
 * Rev'ed libmraa requirement to 1.1.1

### v0.7.1

 * Fix for building python2 vs python3 bindings
 * Fixes for jhd1313m1 row/font size and reset
 * Added bacnet utility class
 * New sensors: bmx055, bmi055, bmc160, bma250e, bmg150, bmm150, t8100, tb7300

### v0.7.0

 * C++ header files have been renamed from *.h to *.hpp along with all Intel
 provided examples and code samples to allow for native C drivers
 * Updated driver compatibility to new MRAA 1.0 API
 * First sensor implementations for our newly added Dallas One Wire support
 * Several fixes based on issues reported on Github
 * Changed SWIG linking strategy, more robust Travis CI build checks
 * New sensors: ds2413, ds18b20, bmp280, bno055, l3gd20

### v0.6.2

 * Added a generic driver for taking snapshots from an USB camera device
 * New API changes section in documentation to let users know when the UPM API
 gets modified
 * Fixed some spelling errors and improved JavaScript documentation builds with
 newer versions of YUI and Node
 * Enhanced Cmake scripts and the build process
 * New sensors: vcap, e50hx

### v0.6.1

 * Fixed library build process for different configurations across multiple
 environments reported by users.
 * Fixed multiple GFX library header clash and improved SSD1351 compatibility
 by changing SPI mode
 * Added Node and Python array typemaps for nrf24l01
 * Added 2 new JAVA examples

### v0.6.0

 * Introduced C++ Interfaces for several sensor categories that will help
 consolidate the sensor APIs with uniform function names upon implementation
 * Greatly improved Node.js detection and handling during build and install,
 and should be now compatible with all major versions
 * Added a driver for the Curie IMU (accelerometer + gyroscope) that works with
 the new Firmata support implemented by MRAA
 * Continued to provide several JAVA fixes and examples
 * Many new industrial and maker sensors: cwlsxxa, teams, tex00, h803x, apa102,
 smartdrive, bmi160, bme280, ds1808lc, hlg150, lp8860, max44009, si1132, t6713,
 si7005, ssd1351

### v0.5.1

 * Updated the API and added new functionality for the MY9221 class and derived
 sensors: grovecircularled and groveledbar, along with new code samples that
 show these changes
 * Provided a fix for one JAVA example that uses interrupts and updated library
 install paths
 * Several documentation improvements and pictures were added for new sensors
 * A few other sensors received minor patches

### v0.5.0

 * First implementations for industrial grade Modbus sensors, along with first
 sensor examples that use the new iio system driver in MRAA
 * Significant improvements to the stepmotor driver that will now allow precise
 control using any GPIO pin and extended the API with new functionality
 * Further improved JAVA bindings and sensor ISRs have been modified to
 implement Runnable objects
 * Improved Python typemaps and error checking, documentation now includes
 detailed function descriptions
 * Simplified the build process by eliminating the need to define redundant
 MRAA related environment variables
 * New sensors: nlgpio16, ads1x15, t3311, hdxxvxta, hwxpxx, rhusb, ili9341,
 apds9930, kxcjk1013

### v0.4.1

 * Over 150 supported sensor drivers including some industrial grade devices
 * Improved the JAVA bindings, added a large number of examples for sensors,
 enabled JAVA builds in Travis CI, and introduced automatic loading for the
 JAVA shared libraries
 * Continued to improve existing sensor drivers and to enhance them with the
 new C++ coding style and exception types
 * Several cmake improvements to allow building on various Linux distributions
 and with a newer version of Node.JS
 * Simplified adding and building of C++ sensor examples via cmake
 * New sensors: micsv89, xbee, urm37, adxrs610, bma220, dfrph, mcp9808, sx1276,
 groveultrasonic, ozw

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
 maintaining backwards compatibility for existing lcd displays
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
