API Changes                       {#apichanges}
===============

Here's a list of other API changes made to the library that break source/binary
compatibility between releases:

# current master

 * **bno055** This module no longer uses std::strings to pass around
 binary data (*read/writeCalibrationData()*).  Rather, now *std::vectors* of
 the appropriate type are used.  In addition, methods that previously
 returned certain data in the form of an array, like *getEulerAngles()*,
 now return a *std::vector* instead.  This simplifies the Python,
 Javascript, and Java bindings considerably, and leads to more
 "natural" looking Python/Javascript/Java code.  For Javascript, Java,
 and Python, the examples have been modified to use these methods
 rather than the methods that return data in argument pointers or
 arrays.  The "old style" C pointer API's are still present for
 backwards compatibility, but may be removed in the future.

# v1.1.0

 * **i2clcd/jhd1313m1/lcm1602** LCD devices supported by the i2clcd module are
 being separated into individual libraries. The APIs will be preserved, but
 we recommend changing your code to use the new libraries as they become
 available. Once this transition is complete, the i2clcd module will be
 deprecated. So far, the following libraries have been made available
 separately: jhd1313m1, lcm1602.

 * **nunchuck** This driver no longer supports the init() function.
 All initialization is now done in the C nunchuck_init() function,
 or the C++ constructor.  In addition, the *NUNCHUCK_I2C_ADDR*
 define is no longer exposed, as it is not possible to use any other
 I2C address than *0x52*.  The readBytes() and writeByte() functions
 are also no longer exposed, since aside from what the driver does
 to initialize and read data, there are no other options available.

 * **enc03r** This driver no longer supports the value() function.  In
 addition, an update() function has been added.  This function must be
 called prior to calling angularVelocity().  angularVelocity() no
 longer accepts an argument.  Additional functions have been added,
 however they do not affect compatibility with previous versions.

 * **ds18b20** The C++ interface init() function has been deprecated.
 It is still present, but currently does nothing.  It will be removed
 in a future release.

 * **grove<name>** Starting with UPM 1.0 the Grove libraries have been renamed
 from *upm-grove<name>* to simply *upm-<name>*. Class names also match this new
 format, with old classes marked as deprecated throughout the documentation.
 List of affected libraries: grovecollision, groveehr, groveeldriver,
 groveelectromagnet, groveemg, grovegprs, grovegsr, grovelinefinder, grovemd,
 grovemoisture, groveo2, grovescam, grovespeaker, groveultrasonic, grovevdiv,
 grovewater, grovewfs.

 * **grove** As of UPM 1.0 the Grove classes for sensors in the starter kit are
 being separated into individual libraries. The old classes will be deprecated
 over time and eventually removed. Corresponding libraries have the grove
 prefix removed. Affected classes are GroveButton, GroveLed, GroveLight,
 GroveRelay, GroveRotary, GroveSlide and GroveTemp.

 * The **ublox6** driver has been replaced with a generic implementation called
 nmea_gps as of UPM 1.0. This driver should handle all generic serial GPS
 devices that output NMEA data going forward.  This new driver has been tested
 with ublox6, DFRobot VK2828U7 (ublox7) and ublox LEA-6H GPS devices.

 * **grove** Binary compatibility was broken for the GroveTemp class as of UPM
 v0.7.3. C++ code using this class has to be recompiled.

 * There were frequent misspellings of the word *Celsius* in the UPM
 code.  In some cases, these were in method names, which will cause
 some API compatibility issues.  These have all been corrected for UPM
 versions after v.0.7.2.

 * Our **C++ header files** changed their extension from *.h* to *.hpp* in
 version 0.7.0, Intel provided examples and code samples also reflect this
 change but you will need to modify your `#include` directives in existing
 code.

 * **my9221**, **groveledbar** and **grovecircularled** are now all part of the
 same library (my9221) and new functionality was added going to v.0.5.1.

 * **stepmotor** driver API was changed significantly from v.0.4.1 to v.0.5.0.

 * **eboled** library was greatly improved in version 0.4.0 and the `draw()`
 function was removed in favor of a more complete GFX library implementation.
