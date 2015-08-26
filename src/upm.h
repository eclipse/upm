/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

/// @cond DEV
/**
 * This file only serves to describe sensor groups based on libmraa groups.
 * Sensors may belong to multiple groups. This is purely a documentation header
 * and is not meant to be installed anywhere.
 */
/// @endcond DEV

////////////////////////////////////////////////////////////////// @cond HEA
/// Main group place holders.
////////////////////////////////////////////////////////////////// @endcond HEA

/**
 * @brief Sensors grouped by category
 * @defgroup bycat Sensor Categories
 */

/**
 * @brief Sensors grouped by connection type
 * @defgroup bycon Connection Type
 */

/**
 * @brief Sensors grouped by manufacturer
 * @defgroup byman Manufacturer
 */

/**
 * @brief Sensors grouped into starter kits
 * @defgroup bykit Starter Kits
 */

////////////////////////////////////////////////////////////////// @cond CAT
/// Groups for the various Sensor Categories.
////////////////////////////////////////////////////////////////// @endcond CAT

/**
 * @brief Measure acceleration & tilt or collision detection
 * @defgroup accelerometer Accelerometer
 * @ingroup bycat
 */

/**
 * @brief Sensors with a variable voltage output
 * @defgroup ainput Analog Inputs
 * @ingroup bycat
 */

/**
 * @brief Measure pressure and atmospheric conditions
 * @defgroup pressure Atmospheric Pressure
 * @ingroup bycat
 */

/**
 * @brief Button, Switch or Toggle
 * @defgroup button Button
 * @ingroup bycat
 */

/**
 * @brief Light sensors with special function: Color recognition
 * @defgroup color Color Sensor
 * @ingroup bycat
 */

/**
 * @brief Measure magnetic field to give rotation or heading
 * @defgroup compass Compass/Gyro/Magnometers
 * @ingroup bycat
 */

/**
 * @brief Resistive digital to analog converters (DAC)
 * @defgroup digipot Digital Potentiometer
 * @ingroup bycat
 */

/**
 * @brief TFT, LCD, LED display elements
 * @defgroup display Displays
 * @ingroup bycat
 */

/**
 * @brief Measure electric current and ADC converters
 * @defgroup electric Electricity
 * @ingroup bycat
 */

/**
 * @brief Measure bending or detect vibration
 * @defgroup flexfor Flex/Force
 * @ingroup bycat
 */

/**
 * @brief Measure substance concentrations in gases
 * @defgroup gaseous Gas
 * @ingroup bycat
 */

/**
 * @brief Provide positioning capabilities
 * @defgroup gps GPS
 * @ingroup bycat
 */

/**
 * @brief LEDs, LED strips, LED matrix displays & controllers
 * @defgroup led LEDs
 * @ingroup bycat
 */

/**
 * @brief Measure light intensity or distances
 * @defgroup light Light/Proximity/IR
 * @ingroup bycat
 */

/**
 * @brief Measure liquid flow rates or levels
 * @defgroup liquid Liquid Flow
 * @ingroup bycat
 */

/**
 * @brief Sensors with specific medical application
 * @defgroup medical Medical
 * @ingroup bycat
 */

/**
 * @brief Various motors & controllers to get things moving
 * @defgroup motor Motor
 * @ingroup bycat
 */

/**
 * @brief Other types of supported sensors
 * @defgroup other Other
 * @ingroup bycat
 */

/**
 * @brief Different low and high power relays
 * @defgroup relay Relay
 * @ingroup bycat
 */

/**
 * @brief Wireless sensors using RFID tags
 * @defgroup rfid RFID
 * @ingroup bycat
 */

/**
 * @brief Various servo motors & controllers
 * @defgroup servos Servo
 * @ingroup bycat
 */

/**
 * @brief Provide sound recording or playback
 * @defgroup sound Sound
 * @ingroup bycat
 */

/**
 * @brief Measure temperature & humidity
 * @defgroup temp Temperature/Humidity
 * @ingroup bycat
 */

/**
 * @brief Sensors using serial communication
 * @defgroup serial Serial
 * @ingroup bycat
 */

/**
 * @brief Real time clocks & time measurement
 * @defgroup time Time
 * @ingroup bycat
 */

/**
 * @brief Capacitive touch sensors
 * @defgroup touch Touch Sensor
 * @ingroup bycat
 */

/**
 * @brief Provide WiFi, Bluetooth, RF communication
 * @defgroup wifi Wireless Communication
 * @ingroup bycat
 */

////////////////////////////////////////////////////////////////// @cond CON
/// Groups for the various Connection Types.
////////////////////////////////////////////////////////////////// @endcond CON

/**
 * @brief Sensors requiring an ADC value to be read
 * @defgroup analog AIO
 * @ingroup bycon
 */

/**
 * @brief Modules using the i2c bus
 * @defgroup i2c I2C
 * @ingroup bycon
 */

/**
 * @brief Modules using the SPI bus
 * @defgroup spi SPI
 * @ingroup bycon
 */

/**
 * @brief Modules using GPIOs directly
 * @defgroup gpio GPIO
 * @ingroup bycon
 */

/**
 * @brief Modules using a PWM capable GPIO pin
 * @defgroup pwm PWM
 * @ingroup bycon
 */

/**
 * @brief Modules using a serial connection (RX/TX)
 * @defgroup uart UART
 * @ingroup bycon
 */

////////////////////////////////////////////////////////////////// @cond MAN
/// Groups for the various Manufacturers.
////////////////////////////////////////////////////////////////// @endcond MAN

/**
 * @brief Adafruit Industries
 * @defgroup adafruit Adafruit
 * @ingroup byman
 */

/**
 * @brief EMax
 * @defgroup emax EMax
 * @ingroup byman
 */

/**
 * @brief DFRobot
 * @defgroup dfrobot DFRobot
 * @ingroup byman
 */

/**
 * @brief EpicTinker
 * @defgroup epict EpicTinker
 * @ingroup byman
 */

/**
 * @brief Freescale
 * @defgroup freescale Freescale
 * @ingroup byman
 */
 
/**
 * @brief Generic Brands
 * @defgroup generic Generic
 * @ingroup byman
 */

/**
 * @brief Honeywell
 * @defgroup honeywell Honeywell
 * @ingroup byman
 */

/**
 * @brief Maxim Integrated
 * @defgroup maxim Maxim Integrated
 * @ingroup byman
 */

/**
 * @brief SeeedStudio - Grove Sensors
 * @defgroup seeed SeeedStudio
 * @ingroup byman
 */

/**
 * @brief Sparkfun
 * @defgroup sparkfun Sparkfun
 * @ingroup byman
 */

/**
 * @brief Texas Instruments
 * @defgroup ti Texas Instruments
 * @ingroup byman
 */

////////////////////////////////////////////////////////////////// @cond KIT
/// Groups for the various Starter Kits.
////////////////////////////////////////////////////////////////// @endcond KIT

/**
 * @brief Grove Starter Kit - Contains basic sensors
 * @defgroup gsk Grove Starter Kit
 * @ingroup bykit
 */
 
/**
 * @brief Home Automation Kit - For projects around the house
 * @defgroup hak Home Automation Kit
 * @ingroup bykit
 */ 

/**
 * @brief Environment & Agriculture Kit - For outdoor projects
 * @defgroup eak Environment & Agriculture Kit
 * @ingroup bykit
 */
 
/**
 * @brief Transportation & Safety Kit - Used mostly on vehicles 
 * @defgroup tsk Transportation & Safety Kit
 * @ingroup bykit
 */
 
/**
 * @brief Robotics Kit - Sensors for your robot
 * @defgroup robok Robotics Kit
 * @ingroup bykit
 */
