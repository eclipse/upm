/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>

#include <md.h>

namespace upm {
    /**
     * @brief I2C Motor Driver
     * @defgroup md libupm-md
     * @ingroup seeed i2c motor robok
     */

    /**
     * @library md
     * @sensor md
     * @comname I2C Motor Driver
     * @altname Grove Motor Driver
     * @type motor
     * @man seeed
     * @con i2c
     * @web http://wiki.seeed.cc/Grove-I2C_Motor_Driver_V1.3/
     * @kit robok
     *
     * @brief API for the I2C Motor Driver
     *
     * This class implements support for the I2C Motor Driver.
     * This device can support a single 4-wire stepper motor, or two
     * 2-wire DC motors. The device contains an Atmel* ATmega8L
     * microcontroller that manages an L298N H-bridge driver chip.
     *
     * This device supports an I2C bus speed of 100Khz only.
     *
     * The module does not provide any telemetry or status - it only
     * accepts I2C commands for its various operations.
     *
     * This module was tested with version 1.3 of the I2C Motor
     * Driver.
     *
     * For stepper operation, this driver can run in one of two modes -
     * Mode 1, where this driver handles the stepping operation, and
     * Mode 2, where this driver simply sends commands to the
     * Motor Driver, and it handles the stepping operation.  Mode2
     * requires updated (and working) firmware to be loaded onto the
     * device.
     *
     * The default stepper operation mode is Mode1, which is generally
     * more flexible and is supported on all firmware revisions.
     *
     * @image html md.jpg
     * An example showing the use of a DC motor
     * @snippet md.cxx Interesting
     * An example showing the use of a 4-wire stepper
     * @snippet md-stepper.cxx Interesting
     */
    class MD {

    public:
        /**
         * MD constructor
         *
         * @param bus I2C bus to use
         * @param address I2C address to use
         */
        MD(int bus=MD_I2C_BUS, uint8_t address=MD_DEFAULT_I2C_ADDR);

        /**
         * MD destructor
         */
        ~MD();

        /**
         * Composes and writes a 3-byte packet to the controller
         *
         * @param reg Register location
         * @param data1 First byte of data
         * @param data2 Second byte of data
         * @return True if successful
         */
        bool writePacket(MD_REG_T reg, uint8_t data1, uint8_t data2);

        /**
         * To control DC motors, sets the speed of motors A & B.
         * Valid values are 0-255.
         *
         * @param speedA Speed of motor A
         * @param speedB Speed of motor B
         * @return True if successful
         */
        bool setMotorSpeeds(uint8_t speedA, uint8_t speedB);

        /**
         * To control DC motors, sets the PWM frequency prescale
         * factor. Note: this register is not ducumented other than to say
         * the default value is 0x03. Presumably, this is the timer
         * prescale factor used on the ATMega MCU timer driving the PWM.
         *
         * @param freq PWM prescale frequency; default is 0x03
         * @return True if successful
         */
        bool setPWMFrequencyPrescale(uint8_t freq=0x03);

        /**
         * To control DC motors, sets the directions of motors A & B
         *
         * @param dirA Direction for motor A, DIR_CW or DIR_CCW
         * @param dirB Direction for motor B, DIR_CW or DIR_CCW
         * @return True if successful
         */
        bool setMotorDirections(MD_DC_DIRECTION_T dirA, MD_DC_DIRECTION_T dirB);

        /**
         * To control a stepper motor, sets its direction and speed, and
         * then starts operation.  For Mode2, this method will return
         * immediately.  For Mode1 (the default) this method returns when
         * the number of steps specified by setStepperSteps() has
         * completed.
         *
         * @param dir Direction, STEP_DIR_CW or STEP_DIR_CCW
         * @param speed Motor speed. Valid range is 1-255. For Mode 1
         * (default), this specifies the speed in RPM's.  For Mode 2,
         * speed is multiplied by 4ms by the board, so higher numbers
         * will mean a slower speed.
         * @return True if successful
         */
        bool enableStepper(MD_STEP_DIRECTION_T dir, uint8_t speed);

        /**
         * To control a stepper motor, stops the stepper motor.
         *
         * @return True if successful
         */
        bool disableStepper();

        /**
         * To control a stepper motor, specifies the number of steps to
         * execute. For Mode2, valid values are between 1-255, 255 means
         * continuous rotation.
         *
         * For Mode1 (the default) steps can be any positive integer.
         *
         * @param steps Number of steps to execute. 255 (only in Mode2)
         * means continuous rotation.
         * @return True if successful
         */
        bool setStepperSteps(unsigned int steps);

        /**
         * Configure the initial Stepper parameters.  This should be
         * called before any other stepper method.
         *
         * @param stepsPerRev The number of steps required to complete one
         * full revolution.
         * @param mode The stepper operating mode, default STEP_MODE1
         * @return Elapsed milliseconds
         */
        void configStepper(unsigned int stepsPerRev,
                           MD_STEP_MODE_T mode=MD_STEP_MODE1);

    protected:
        md_context m_md;

    private:
        /* Disable implicit copy and assignment operators */
        MD(const MD&) = delete;
        MD &operator=(const MD&) = delete;
    };
}


