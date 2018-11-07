API Changes                       {#apichanges}
===============

Here's a list of other API changes made to the library that break source/binary
compatibility between releases:

# v1.7.0
 * The interface of **kx122** has been modified to return values instead
 of receiving pointers. This applies to member functions: getWhoAmI,
 getInterruptSource, getBufferStatus, getRawBufferSamples, and
 getBufferSamples.
 * The **tm1637** constructor has been updated and the fast GPIO parameter
 has been removed with the deprecation of supporting MRAA functions. The
 GPIO lines will initialize by default in fast mode when available.
 * The generic **nmea_gps** library for GPS devices has been greatly enhanced
 with new APIs to offer better control over acquisition intervals and simplify
 parsing of the returned data.

# v1.6.0

 * Several C libraries had their init function pin type modified from uint8_t
 to int allowing usage with subplatforms
 * Our **led** class constructor has been overloaded with a string based
 variant that uses the new MRAA LED APIs
 * The **i2clcd** library has finally been renamed to **lcd** and is now
 mostly considered a bundle for ssd lcd display controllers only
 * The **zfm20** class constructor has been overloaded with a string variant
 that allows initialization using any UART device

# v1.5.0

 * **VEML6070** This sensor class no longer needs an I2C address when
 initialized, since they are fixed. Only the I2C bus number needs to
 be provided.

# v1.3.0

 * **The lsm303 driver has been renamed** There are a variety of
 LSM303 devices out there with various incompatibilities and differing
 capabilities.  The current lsm303 driver in UPM only supports the
 LSM303DLH variant, so it has been renamed to lsm303dlh to avoid
 confusion and to make it clear which variant is actually supported.

    All examples and source files have been renamed, including header
    files.  In addition, the class name, LSM303, has been renamed to
    LSM303DLH.  No other functionality or behavior has been changed.

# v1.2.0

 * **Note for all drivers ported to C** As a general note concerning
 all of the drivers that have been ported to C: **external constants
 have likely been renamed**.  Previously in C++, most of these constants
 were defined as enums in the *upm::classname* namespace.

    For drivers written in C, all of these constants are no longer in
    a class (or UPM) namespace, and instead have the driver name
    prefixed to the original value.

    The driver name is prefixed to these constants to avoid name
    collisions when using multiple drivers in a given application, and
    to make it clear which constants belong to which driver.

    For drivers that used *#define* for constants, only the prefix has
    been added if not already present.  In some cases, names that were
    not very descriptive were changed, for example *ADDR* renamed to
    *BMPX8X_DEFAULT_I2C_ADDR*.

    So for example, a constant that might once have been referred to
    in a C++ example as *upm::MMA7660::AUTOSLEEP_64*, would now be
    referenced as *MMA7660_AUTOSLEEP_64*.

    This holds true for most, if not all drivers that have been ported
    to C.  Not all of these changes are listed in this file due to the
    sheer number of them.

    If you run into problems with constants that were working
    previously, and now cannot be found, this is likely the reason.
    Check the driver documentation and the source code to see what the
    new name is.

    In C, constants are now usually implemented in a separate header
    file named *drivername_defs.h* or *drivername_regs.h*, for easier
    integration into the SWIG languages, and shared use between C++ and
    C implementations.

 * **bmpx8x** This driver has been rewritten from scratch in C, with a
 C++ wrapper.

    All exported symbols have been renamed for consistency and to
    avoid symbol collisions by having a *BMPX8X_* prefix. As an example,
    *ADDR* has been renamed to *BMPX8X_DEFAULT_I2C_ADDR*.  Most C
    ported drivers follow this rule.

    The *getPressureRaw()* and *getTemperatureRaw()* functions have
    been removed.  This functionality was only needed internally to
    the driver.

    The constructor no longer accepts a mode argument.  Only the I2C
    bus and I2C address are accepted.  By default, the device will be
    configured for it's maximum resolution *BMPX8X_OSS_ULTRAHIGHRES*,
    the previous default.  You can use the new method
    *setOversampling()* to change the mode to something else if
    desired.

    The methods related to calibration, like *computeB5()* are no
    longer exposed.

    New methods, *init()* and *reset()* have been added.  *reset()*
    resets the device to a freshly powered up state.  *init()* can be
    used to re-initialize the device after a reset (reload calibration
    data) and set a default oversampling mode.

    A new method, *update()* has been added.  This method will update
    all internal state from the device, and **must** be called before
    querying the pressure, temperature, sea level and altitude values.

    The *getSeaLevelPressure()* method has been split into two
    overloaded methods.  One which **requires** an argument in meters
    (previously, a default was provided), and another which does not
    accept arguments at all and computes the sea level pressure based
    on current altitude.

    The *i2cReadReg_16()*, *i2CWriteReg()* and *i2cReadReg_8()* have
    been replaced with *readReg()*, *readRegs()*, and *writeReg()*, in
    line with other I2C/SPI drivers of this type.  They are marked
    protected (in C++) now as well.  Please see the updated
    documentation and examples for this driver.

 * **mma7660** This driver has been rewritten in C.  Some exported
 symbols have been changed, for example, *MMA7660_I2C_BUS* was renamed
 to *MMA7660_DEFAULT_I2C_BUS*.

    See updated documentation and examples for other changes.

 * **bmx055, bmi055, bmc150, bma250e, bmg160, bmm150** This driver has
 been split up.  The *bma250e*, *bmg160*, *bmm150* drivers have been
 rewritten in C (with C++ wrappers) and now reside in their own
 libraries.  The versions of these drivers that used to be present in
 *bmx055* have been removed, and *bmx055* now uses the new libraries
 for it's functionality.  The other two composite devices, *bmi055*,
 and *bmc150* are still contained within the *bmx055* library, and
 also use the new libraries for their functionality.

    In addition, for all of these drivers some private methods are no
    longer exposed (such as the compensation routines).

    The C++ driver methods that once returned pointers to a floating
    point array now return *std::vectors* of the appropriate type.
    The SWIG language examples for these drivers have been modified to
    use these methods instead of the C pointer based SWIG methods
    previously used.

 * **sainsmartks** This driver has been renamed to *lcdks* (LCD Keypad
 Shield) and moved into it's own library.  It uses the *lcm1602*
 library to do most of it's work.  In addition, an additional argument
 was added to the constructor to optionally allow specifying a GPIO
 pin to be used to control the backlight.  This driver supports the
 SainsmartKS and DFRobot LCD Keypad Shields.  Similar devices from
 other manufacturers should also work with this driver.

 * **lcm1602/jhd1313m1** These drivers had been rewritten in C, with
 C++ wrappers and placed into their own libraries in the previous
 version of UPM, however, the original C++ implementation was kept in
 the lcd library for compatibility reasons with existing code.
 To avoid collisions with the header files, the new *lcm1602* and
 *jhd1313m1* drivers had their C++ headers renamed to use a **.hxx**
 suffix.

    In this version of UPM, the *lcm1602* and *jhd1313m1* drivers have
    been removed from the lcd library.  In addition, the header
    files for the new implementation have been renamed from their
    **.hxx** suffix to the normal **.hpp** suffix.

    A change was also made to the new *lcm1602* and *jhd1313m1* C++
    drivers.  The *createChar()* function now accepts a byte vector
    *std::vector<uint8_t>* rather than the *char ** pointer that was
    used previously.  This should make it easier to use with the SWIG
    language bindings (Python, JavaScript, and especially Java).

 * **bmp280/bme280** Some private methods are no longer exposed (such
 as the calibration and compensation routines).  In addition,
 the *getHumidity()* method no longer accepts an argument representing
 pressure at sea level.  A separate method is provided to set this now.

 * **bno055** This module no longer uses std::strings to pass around
 binary data (*read/writeCalibrationData()*).  Rather, now *std::vectors* of
 the appropriate type are used.  In addition, methods that previously
 returned certain data in the form of an array, like *getEulerAngles()*,
 now return a *std::vector* instead.  This simplifies the Python,
 JavaScript, and Java bindings considerably, and leads to more
 "natural" looking Python/JavaScript/Java code.  For JavaScript, Java,
 and Python, the examples have been modified to use these methods
 rather than the methods that return data in argument pointers or
 arrays.

 * **lpd8806** The constructor for this driver was updated to allow specifying
 a SPI bus number. This is now the first parameter, the number of LEDs on the
 strip is now the last (3rd) parameter instead.

 * **max31723** The constructor for this driver was updated to allow specifying
 a SPI bus number. This is now the first parameter, CS pin second.

 * **tcs3414cs** The constructor for this can now accept an I2C bus and
 address.

# v1.1.0 and prior

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
