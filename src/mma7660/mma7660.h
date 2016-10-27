/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/i2c.h>
#include <mraa/gpio.h>

#include <mma7660_regs.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file mma7660.h
     * @library mma7660
     * @brief C API for the mma7660 driver
     *
     * @include mma7660.c
     */

    /**
     * Device context
     */
    typedef struct _mma7660_context {
        mraa_i2c_context  i2c;
        mraa_gpio_context gpio;

        bool              isrInstalled;
    } *mma7660_context;

    /**
     * MMA7660 initialization.
     *
     * @param bus I2C bus to use
     * @param address Address for this sensor
     */
    mma7660_context mma7660_init(int bus, uint8_t address);

    /**
     * MMA7660 destructor
     *
     * @param dev Device context.
     */
    void mma7660_close(mma7660_context dev);

    /**
     * Writes a byte value into a register
     *
     * @param dev Device context.
     * @param reg Register location to write into
     * @param byte Byte to write
     * @return UPM result
     */
    upm_result_t mma7660_write_byte(const mma7660_context dev,
                                    uint8_t reg, uint8_t byte);

    /**
     * Reads a byte value from a register
     *
     * @param dev Device context.
     * @param reg Register location to read from
     * @param byte A pointer to hold the value that was read
     * @return UPM result
     */
    upm_result_t mma7660_read_byte(const mma7660_context dev, uint8_t reg,
                                   uint8_t *byte);

    /**
     * Reads the current value of conversion
     *
     * @param dev Device context.
     * @param x Returned x value
     * @param y Returned y value
     * @param z Returned z value
     * @return UPM result
     */
    upm_result_t mma7660_get_raw_values(const mma7660_context dev,
                                        int *x, int *y, int *z);

    /**
     * Gets the computed acceleration
     *
     * @param dev Device context.
     * @param ax Returned computed acceleration of the X-axis
     * @param ay Returned computed acceleration of the Y-axis
     * @param az Returned computed acceleration of the Z-axis
     * @return UPM result
     */
    upm_result_t mma7660_get_acceleration(const mma7660_context dev,
                                          float *ax, float *ay, float *az);

    /**
     * Reads an axis, verifying its validity. The value passed must be
     * one of MMA7660_REG_XOUT, MMA7660_REG_YOUT, or MMA7660_REG_ZOUT.
     *
     * @param dev Device context.
     * @param axis Axis to read
     * @param val pointer containing returned value
     * @return UPM result
     */
    upm_result_t mma7660_get_verified_axis(const mma7660_context dev,
                                           MMA7660_REG_T axis, int *val);

    /**
     * Reads the tilt register, verifying its validity
     *
     * @param dev Device context.
     * @param val Pointer to returned value
     * @return UPM result
     */
    upm_result_t mma7660_get_verified_tilt(const mma7660_context dev,
                                           uint8_t *val);

    /**
     * Puts the device in the active mode. In this mode, register
     * writes are not allowed. Place the device in the standby mode before
     * attempting to write registers.
     *
     * @param dev Device context.
     * @return UPM result
     */
    upm_result_t mma7660_set_mode_active(const mma7660_context dev);

    /**
     * Puts the device in the standby (power saving) mode. Note: when in
     * the standby mode, there is no valid data in the registers. In
     * addition, the only way to write a register is to put the
     * device in the standby mode.
     *
     * @param dev Device context.
     * @return UPM result
     */
    upm_result_t mma7660_set_mode_standby(const mma7660_context dev);

    /**
     * Reads tiltBackFront bits
     *
     * The value returned is one of the MMA7660_TILT_BF_T values
     *
     * @param dev Device context.
     * @param bits Pointer to returned bits corresponding to the
     * BackFront tilt status
     * @return UPM result
     */
    upm_result_t mma7660_tilt_back_front(const mma7660_context dev,
                                         uint8_t *bits);

    /**
     * Reads tiltLandscapePortrait bits
     *
     * The value returned is one of the MMA7660_TILT_LP_T values
     *
     * @param dev Device context.
     * @param bits Pointer to returned bits corresponding to the
     * LandscapePortrait tilt status
     * @return UPM result
     */
    upm_result_t mma7660_tilt_landscape_portrait(const mma7660_context dev,
                                                 uint8_t *bits);

    /**
     * Reads the tiltTap status
     *
     * @param dev Device context.
     * @param tap Pointer to a bool indicating tap detection
     * @return UPM result
     */
    upm_result_t mma7660_tilt_tap(const mma7660_context dev, bool *tap);

    /**
     * Reads the tiltShake status
     *
     * @param dev Device context.
     * @param shake Pointer to a bool indicating shake detection
     * @return UPM result
     */
    upm_result_t mma7660_tilt_shake(const mma7660_context dev, bool *shake);

    /**
     * Installs an interrupt service routine (ISR) to be called when
     * an interrupt occurs
     *
     * @param dev Device context.
     * @param pin GPIO pin to use as the interrupt pin
     * @param fptr Pointer to a function to be called on interrupt
     * @param arg Pointer to an object to be supplied as an
     * argument to the ISR.
     * @return UPM result
     */
    upm_result_t mma7660_install_isr(const mma7660_context dev, int pin,
                                     void (*isr)(void *), void *arg);

    /**
     * Uninstalls the previously installed ISR
     *
     * @param dev Device context.
     */
    void mma7660_uninstall_isr(const mma7660_context dev);

    /**
     * Enables interrupt generation based on passed interrupt bits.
     * The bits are a bitmask of the requested MMA7660_INTR_T values.
     * Note: the device must be in the standby mode to set this register.
     *
     * @param dev Device context.
     * @param ibits Sets the requested interrupt bits
     * @return UPM result
     */
    upm_result_t mma7660_set_interrupt_bits(const mma7660_context dev,
                                            uint8_t ibits);

    /**
     * Sets the sampling rate of the sensor. The value supplied must
     * be one of the MMA7660_AUTOSLEEP_T values.
     *
     * @param dev Device context.
     * @param sr One of the MMA7660_AUTOSLEEP_T values
     * @return UPM result
     */
    upm_result_t mma7660_set_sample_rate(const mma7660_context dev,
                                         MMA7660_AUTOSLEEP_T sr);

#ifdef __cplusplus
}
#endif
