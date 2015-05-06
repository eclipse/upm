Known Issues                        {#knownissues}
===============

Every once in a while we come across sensors that will not pair well with
a specific board. This means their are either incompatible or will work only
under specific settings. This is a list of such sensors and known workarounds
if they exist.

#### Grove Sensors

 * **Grove I2C Touch Sensor** v1.3 is incompatible with the Intel Edison using
 the Arduino board, but will work with the Mini-breakout if supplied with at
 least 4V. Revision v1.2 works well on all Intel boards.
 * **Grove Nunchuck** only works with the Intel Galileo boards and is unusable
 on the Intel Edison boards.
 * **Grove 3-Axis Digital Gyroscope** (ITG-3200) is not compatible with the
 Intel Edison Arduino board but will work with the Mini-breakout.
 * **Grove 3-Axis Digital Accelerometer** (ADXL345) only works with the Intel
 Edison Arduino board when powered from the 3.3V line.
 * **Grove 96x96 OLED Display** will not work on the Intel Edison with Arduino
 breakout.
 * **Grove 128x64 OLED Display** will not work on the Intel Edison with Arduino
 breakout.
 * **Grove Barometer** (BMP085) has an unstable connection on the Intel Edison
 using the Arduino breakout and does not run properly.
 * **Grove 6-Axis Accelerometer & Compass** (LSM303) fails to write to the
 configuration register properly and returns invalid data.
 * **Grove I2C ADC Converter** does not show up on the I2C bus on the Intel
 Edison with the Arduino board.
 * **Grove I2C Motor Driver** is not compatible with the Intel Galileo due to
 the inability to change the I2C bus speed to 100 KHz.
 * **Grove CO2 Sensor** will return zeroed data and is unusable on the Intel
 Galileo.
 * **Grove BLE** (HM-11) does not return data on the Intel Galileo board.

#### Adafruit Sensors

 * **Adafruit Motor Shield** (1438) will not get detected on the I2C bus when
 connected to the Intel Edison with an Arduino breakout.

#### Other Sensors

 * **MPU9150** might return inaccurate data.

Some I2C sensors seem to add too much capacitance to the SDA line of the Intel
Edison Arduino breakout board, thus the signal essentially becomes stuck to a
logic 1. When this happens, any other sensors connected to the I2C bus will be
rendered useless too. There is no universal solution for this issue, but under
most cases the same sensor will work just fine on the Intel Edison Mini. Using
the same sensor but from a different vendor can also work in some cases.
