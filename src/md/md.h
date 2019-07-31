/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <upm.h>
#include <mraa/i2c.h>

#include "md_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file md.h
     * @library md
     * @brief C API for the md driver
     *
     * @include md.c
     */

    /**
     * Device context
     */
    typedef struct _md_context {
        mraa_i2c_context i2c;

        // steps per revolution
        int stepsPerRev;
        int currentStep;
        uint32_t stepDelay;
        uint32_t totalSteps;
        MD_STEP_MODE_T stepMode;

        // step direction: - 1 = forward, -1 = backward
        int stepDirection;

        // initialized?
        bool initialized;

    } *md_context;

    /**
     * MD initializer.
     *
     * @param bus I2C bus to use
     * @param address I2C address to use
     * @return Device context.
     */
    md_context md_init(int bus, uint8_t address);

    /**
     * MD close.
     *
     * @param dev Device context.
     */
    void md_close(md_context dev);

    /**
     * Composes and writes a 3-byte packet to the controller
     *
     * @param dev Device context.
     * @param reg Register location
     * @param data1 First byte of data
     * @param data2 Second byte of data
     * @return True if successful
     */
    bool md_write_packet(const md_context dev, MD_REG_T reg, uint8_t data1,
                         uint8_t data2);

    /**
     * To control DC motors, sets the speed of motors A & B.
     * Valid values are 0-255.
     *
     * @param dev Device context.
     * @param speedA Speed of motor A
     * @param speedB Speed of motor B
     * @return True if successful
     */
    bool md_set_motor_speeds(const md_context dev, uint8_t speedA,
                             uint8_t speedB);

    /**
     * To control DC motors, sets the PWM frequency prescale
     * factor. Note: this register is not ducumented other than to say
     * the default value is 0x03. Presumably, this is the timer
     * prescale factor used on the ATMega MCU timer driving the PWM.
     *
     * @param dev Device context.
     * @param freq PWM prescale frequency; default should be 0x03
     * @return True if successful
     */
    bool md_set_pwm_frequency_prescale(const md_context dev, uint8_t freq);

    /**
     * To control DC motors, sets the directions of motors A & B
     *
     * @param dev Device context.
     * @param dirA Direction for motor A, MD_DIR_CW or MD_DIR_CCW
     * @param dirB Direction for motor B, MD_DIR_CW or MD_DIR_CCW
     * @return True if successful
     */
    bool md_set_motor_directions(const md_context dev, MD_DC_DIRECTION_T dirA,
                                 MD_DC_DIRECTION_T dirB);

    /**
     * To control a stepper motor, sets its direction and speed, and
     * then starts operation.  For Mode2, this method will return
     * immediately.  For Mode1 (the default) this method returns when
     * the number of steps specified by md_set_stepper_steps() has
     * completed.
     *
     * @param dev Device context.
     * @param dir Direction, MD_STEP_DIR_CW or MD_STEP_DIR_CCW
     * @param speed Motor speed. Valid range is 1-255. For Mode 1
     * (default), this specifies the speed in RPM's.  For Mode 2,
     * speed is multiplied by 4ms by the board, so higher numbers
     * will mean a slower speed.
     * @return True if successful
     */
    bool md_enable_stepper(const md_context dev, MD_STEP_DIRECTION_T dir,
                           uint8_t speed);

    /**
     * To control a stepper motor, stops the stepper motor.
     *
     * @param dev Device context.
     * @return True if successful
     */
    bool md_disable_stepper(const md_context dev);

    /**
     * To control a stepper motor, specifies the number of steps to
     * execute. For Mode2, valid values are between 1-255, 255 means
     * continuous rotation.
     *
     * For Mode1 (the default) steps can be any positive integer.
     *
     * @param dev Device context.
     * @param steps Number of steps to execute. 255 (only in Mode2)
     * means continuous rotation.
     * @return True if successful
     */
    bool md_set_stepper_steps(const md_context dev, unsigned int steps);

    /**
     * Configure the initial Stepper parameters.  This should be
     * called before any other stepper method.
     *
     * @param dev Device context.
     * @param stepsPerRev The number of steps required to complete one
     * full revolution.
     * @param mode The stepper operating mode.  MD_STEP_MODE1 is more
     * flexible, and is handled directly by this driver.
     * MD_STEP_MODE2 is handled completely by the MCU on board the
     * motor driver, but is much more restrictive and requires updated
     * firmware.  We generally recommend MODE1.
     */
    void md_config_stepper(const md_context dev, unsigned int stepsPerRev,
                           MD_STEP_MODE_T mode);


#ifdef __cplusplus
}
#endif
