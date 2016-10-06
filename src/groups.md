Authors: Brendan Le Foll <brendan.le.foll@intel.com>
         Mihai Tudor Panu <mihai.tudor.panu@intel.com>
Copyright (c) 2014-2016 Intel Corporation.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


### Notice: ###


This file only serves to describe sensor groups based on various filters.
Sensors may belong to multiple groups. This is purely a documentation stub
and is not meant to be installed anywhere.


### Main group place holders ###


@defgroup bycat Sensor Categories
@brief Sensors grouped by category


@defgroup byif C++ Interface
@brief Sensors grouped by C++ interface


@defgroup bycon Connection Type
@brief Sensors grouped by connection type


@defgroup byman Manufacturer
@brief Sensors grouped by manufacturer


@defgroup bykit Starter Kits
@brief Sensors grouped into starter kits


### Groups for the various Sensor Categories ###


@defgroup accelerometer Accelerometer
@ingroup bycat
@brief Measure acceleration & tilt or collision detection


@defgroup ainput Analog Inputs
@ingroup bycat
@brief Sensors with a variable voltage output


@defgroup pressure Atmospheric Pressure
@ingroup bycat
@brief Measure pressure and atmospheric conditions


@defgroup button Button
@ingroup bycat
@brief Button, Switch or Toggle


@defgroup color Color Sensor
@ingroup bycat
@brief Light sensors with special function: Color recognition


@defgroup compass Compass/Gyro/Magnometers
@ingroup bycat
@brief Measure magnetic field to give rotation or heading


@defgroup digipot Digital Potentiometer
@ingroup bycat
@brief Resistive digital to analog converters (DAC)


@defgroup display Displays
@ingroup bycat
@brief TFT, LCD, LED display elements


@defgroup electric Electricity
@ingroup bycat
@brief Measure electric current and ADC converters


@defgroup flexfor Flex/Force
@ingroup bycat
@brief Measure bending or detect vibration


@defgroup gaseous Gas
@ingroup bycat
@brief Measure substance concentrations in gases


@defgroup gps GPS
@ingroup bycat
@brief Provide positioning capabilities


@defgroup led LEDs
@ingroup bycat
@brief LEDs, LED strips, LED matrix displays & controllers


@defgroup light Light/Proximity/IR
@ingroup bycat
@brief Measure light intensity or distances


@defgroup liquid Liquid Flow
@ingroup bycat
@brief Measure liquid flow rates or levels


@defgroup medical Medical
@ingroup bycat
@brief Sensors with specific medical application


@defgroup motor Motor
@ingroup bycat
@brief Various motors & controllers to get things moving


@defgroup other Other
@ingroup bycat
@brief Other types of supported sensors


@defgroup relay Relay
@ingroup bycat
@brief Different low and high power relays


@defgroup rfid RFID
@ingroup bycat
@brief Wireless sensors using RFID tags


@defgroup servos Servo
@ingroup bycat
@brief Various servo motors & controllers


@defgroup sound Sound
@ingroup bycat
@brief Provide sound recording or playback


@defgroup temp Temperature/Humidity
@ingroup bycat
@brief Measure temperature & humidity


@defgroup serial Serial
@ingroup bycat
@brief Sensors using serial communication


@defgroup time Time
@ingroup bycat
@brief Real time clocks & time measurement


@defgroup touch Touch Sensor
@ingroup bycat
@brief Capacitive touch sensors


@defgroup video Video
@ingroup bycat
@brief Provide video or video camera access


@defgroup wifi Wireless Communication
@ingroup bycat
@brief Provide WiFi, Bluetooth, RF communication


### Groups for the various Sensor C++ Interfaces ###


@defgroup ilightsensor ILightSensor
@ingroup byif
@brief Implements ILightSensor


@defgroup ilightcontroller ILightController
@ingroup byif
@brief Implements ILightController


@defgroup ipressuresensor IPressureSensor
@ingroup byif
@brief Implements IPressureSensor


@defgroup itemperaturesensor ITemperatureSensor
@ingroup byif
@brief Implements ITemperatureSensor


@defgroup iadc IADC
@ingroup byif
@brief Implements IADC


@defgroup ico2sensor ICOSensor
@ingroup byif
@brief Implements ICOSensor


### Groups for the various Connection Types ###


@defgroup analog AIO
@ingroup bycon
@brief Sensors requiring an ADC value to be read


@defgroup i2c I2C
@ingroup bycon
@brief Modules using the i2c bus


@defgroup spi SPI
@ingroup bycon
@brief Modules using the SPI bus


@defgroup gpio GPIO
@ingroup bycon
@brief Modules using GPIOs directly


@defgroup pwm PWM
@ingroup bycon
@brief Modules using a PWM capable GPIO pin


@defgroup uart UART
@ingroup bycon
@brief Modules using a serial connection (RX/TX)


### Groups for the various Manufacturers ###


@defgroup adafruit Adafruit
@ingroup byman
@brief Adafruit Industries


@defgroup amphenol Amphenol
@ingroup byman
@brief Amphenol


@defgroup bosch Bosch
@ingroup byman
@brief Bosch


@defgroup comet Comet
@ingroup byman
@brief Comet System


@defgroup dfrobot DFRobot
@ingroup byman
@brief DFRobot


@defgroup emax EMax
@ingroup byman
@brief EMax


@defgroup epict EpicTinker
@ingroup byman
@brief EpicTinker


@defgroup freescale Freescale
@ingroup byman
@brief Freescale


@defgroup generic Generic
@ingroup byman
@brief Generic Brands


@defgroup honeywell Honeywell
@ingroup byman
@brief Honeywell


@defgroup maxim Maxim Integrated
@ingroup byman
@brief Maxim Integrated


@defgroup meas Measurement Specialties
@ingroup byman
@brief Measurement Specialties


@defgroup mouser Mouser
@ingroup byman
@brief Mouser


@defgroup numatolabs Numato Labs
@ingroup byman
@brief Numato Labs


@defgroup omega Omega
@ingroup byman
@brief Omega


@defgroup openelectrons Open Electrons
@ingroup byman
@brief Open Electrons


@defgroup seeed SeeedStudio
@ingroup byman
@brief SeeedStudio - Grove Sensors


@defgroup silabs Silicon Labs
@ingroup byman
@brief Silicon Labs


@defgroup sparkfun Sparkfun
@ingroup byman
@brief Sparkfun


@defgroup stmicro STMicroelectronics
@ingroup byman
@brief STMicroelectronics


@defgroup ti Texas Instruments
@ingroup byman
@brief Texas Instruments


@defgroup veris Veris Industries
@ingroup byman
@brief Veris Industries


### Groups for the various Starter Kits ###


@defgroup gsk Grove Starter Kit
@ingroup bykit
@brief Grove Starter Kit - Contains basic sensors


@defgroup hak Home Automation Kit
@ingroup bykit
@brief Home Automation Kit - For projects around the house


@defgroup eak Environment & Agriculture Kit
@ingroup bykit
@brief Environment & Agriculture Kit - For outdoor projects


@defgroup tsk Transportation & Safety Kit
@ingroup bykit
@brief Transportation & Safety Kit - Used mostly on vehicles


@defgroup robok Robotics Kit
@ingroup bykit
@brief Robotics Kit - Sensors for your robot
