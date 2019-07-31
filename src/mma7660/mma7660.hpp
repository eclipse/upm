/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <vector>

#include "mma7660.h"
#include <interfaces/iAcceleration.hpp>

namespace upm {

    /**
     * @brief MMA7660 3-Axis Digital Accelerometer
     * @defgroup mma7660 libupm-mma7660
     * @ingroup seeed i2c gpio accelerometer
     */
    /**
     * @library mma7660
     * @sensor mma7660
     * @comname I2C 3-axis Digital Accelerometer (1.5g)
     * @altname Grove 3-Axis Digital Accelerometer (1.5g)
     * @type accelerometer
     * @man seeed
     * @con i2c gpio
     * @web http://www.nxp.com/products/sensors/accelerometers/3-axis-accelerometers/1.5g-low-g-digital-accelerometer:MMA7660FC
     *
     * @brief API for the MMA7660 I2C 3-Axis Digital Accelerometer
     *
     * UPM module for the MMA7660 I2C 3-axis digital accelerometer.
     * This device supports a variety of capabilities, including the
     * generation of interrupts for various conditions, tilt and basic
     * gesture detection, and X/Y/Z-axis measurements of g-forces
     * being applied (up to 1.5g)
     *
     * This module was tested with the Grove 3-Axis Digital
     * Accelerometer (1.5g)
     *
     * @image html mma7660.jpg
     * @snippet mma7660.cxx Interesting
     */
    class MMA7660: virtual public iAcceleration {
    public:

        /**
         * MMA7660 constructor
         *
         * @param bus I2C bus to use
         * @param address Address for this sensor; default is 0x55
         */
        MMA7660(int bus, uint8_t address=MMA7660_DEFAULT_I2C_ADDR);

        /**
         * MMA7660 destructor
         */
        virtual ~MMA7660();

        /**
         * Writes a byte value into a register
         *
         * @param reg Register location to write into
         * @param byte Byte to write
         * @return True if successful
         */
        bool writeByte(uint8_t reg, uint8_t byte);

        /**
         * Reads a byte value from a register
         *
         * @param reg Register location to read from
         * @return Value in a specified register
         */
        uint8_t readByte(uint8_t reg);

        /**
         * Reads the current value of conversion
         *
         * @param x Returned x value
         * @param y Returned y value
         * @param z Returned z value
         */
        void getRawValues(int *x, int *y, int *z);

        /**
         * Gets the computed acceleration
         *
         * @param ax Returned computed acceleration of the X-axis
         * @param ay Returned computed acceleration of the Y-axis
         * @param az Returned computed acceleration of the Z-axis
         */
        void getAcceleration(float *ax, float *ay, float *az);

        /**
         * get acceleration values
         * 
         * @return stl vector of size 3 representing the 3 axis
         */
        virtual std::vector<float> getAcceleration();

        /**
         * Reads an axis, verifying its validity. The value passed must
         * be one of REG_XOUT, REG_YOUT, or REG_ZOUT.
         *
         * @param axis Axis to read
         * @return Axis value
         */
        int getVerifiedAxis(MMA7660_REG_T axis);

        /**
         * Reads the tilt register, verifying its validity
         *
         * @return Tilt value
         */
        uint8_t getVerifiedTilt();

        /**
         * Puts the device in the active mode. In this mode, register
         * writes are not allowed. Place the device in the standby mode before
         * attempting to write registers.
         *
         */
        void setModeActive();

        /**
         * Puts the device in the standby (power saving) mode. Note: when in
         * the standby mode, there is no valid data in the registers. In
         * addition, the only way to write a register is to put the
         * device in the standby mode.
         *
         */
        void setModeStandby();

        /**
         * Reads tiltBackFront bits
         *
         * The value returned is one of the MMA7660_TILT_BF_T values
         *
         * @return Bits corresponding to the BackFront tilt status
         */
        uint8_t tiltBackFront();

        /**
         * Reads tiltLandscapePortrait bits
         *
         * The value returned is one of the MMA7660_TILT_LP_T values
         *
         * @return Bits corresponding to the LandscapePortrait tilt status
         */
        uint8_t tiltLandscapePortrait();

        /**
         * Reads the tiltTap status
         *
         * @return True if a tap is detected
         */
        bool tiltTap();

        /**
         * Reads the tiltShake status
         *
         * @return True if a shake is detected
         */
        bool tiltShake();

        /**
         * Uninstalls the previously installed ISR
         *
         */
        void uninstallISR();

        /**
         * Enables interrupt generation based on passed interrupt bits.
         * The bits are a bitmask of the requested MMA7660_INTR_T values.
         * Note: the device must be in the standby mode to set this register.
         *
         * @param ibits Sets the requested interrupt bits
         * @return True if successful
         */
        bool setInterruptBits(uint8_t ibits);

        /**
         * Sets the sampling rate of the sensor. The value supplied must
         * be one of the MMA7660_AUTOSLEEP_T values.
         *
         * @param sr One of the MMA7660_AUTOSLEEP_T values
         * @return True if successful
         */
        bool setSampleRate(MMA7660_AUTOSLEEP_T sr);

        /**
         * Reads the current value of conversion. The returned memory
         * is statically allocated and will be overwritten on each
         * call.
         *
         * @return std::vector containing x, y, z.
         */
        std::vector<int> getRawValues()
        {
            std::vector<int> values(3);
            getRawValues(&values[0], &values[1], &values[2]);
            return values;
        }

        /**
         * Installs an interrupt service routine (ISR) to be called when
         * an interrupt occurs
         *
         * @param pin GPIO pin to use as the interrupt pin
         * @param isr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
        void installISR(int pin, void (*isr)(void *), void *arg);

    protected:
        mma7660_context m_mma7660;

    private:
        /* Disable implicit copy and assignment operators */
        MMA7660(const MMA7660&) = delete;
        MMA7660 &operator=(const MMA7660&) = delete;

    };
}
