/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include <mma7660.h>

namespace upm {

    /**
     * @brief MMA7660 I2C 3-Axis Digital Accelerometer library
     * @defgroup mma7660 libupm-mma7660
     * @ingroup seeed i2c gpio accelerometer
     */
    /**
     * @library mma7660
     * @sensor mma7660
     * @comname MMA7660 3-Axis Digital Accelerometer
     * @altname Grove 3-Axis Digital Accelerometer (1.5g)
     * @type accelerometer
     * @man seeed
     * @con i2c gpio
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
    class MMA7660 {
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

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        /**
         * Reads the current acceleration values.  The returned memory
         * is statically allocated and will be overwritten on each
         * call.
         *
         * @return Array containing x, y, z.
         */
        float *getAcceleration()
        {
            static float values[3];
            getAcceleration(&values[0], &values[1], &values[2]);
            return values;
        }

        /**
         * Reads the current value of conversion. The returned memory
         * is statically allocated and will be overwritten on each
         * call.
         *
         * @return Array containing x, y, z.
         */
        int *getRawValues()
        {
            static int values[3];
            getRawValues(&values[0], &values[1], &values[2]);
            return values;
        }

        void installISR(int pin, jobject runnable)
        {
            installISR(pin, mraa_java_isr_callback, runnable);
        }
#else
        /**
         * Installs an interrupt service routine (ISR) to be called when
         * an interrupt occurs
         *
         * @param pin GPIO pin to use as the interrupt pin
         * @param fptr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
        void installISR(int pin, void (*isr)(void *), void *arg);
#endif // defined(SWIGJAVA) || defined(JAVACALLBACK)

    protected:
        mma7660_context m_mma7660;

    private:
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        void installISR(int pin, void (*isr)(void *), void *arg);
#endif

    };
}
