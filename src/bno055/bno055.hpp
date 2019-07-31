/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <vector>
#include "bno055.h"
#include "mraa/initio.hpp"


#include <interfaces/iAcceleration.hpp>
#include <interfaces/iGyroscope.hpp>
#include <interfaces/iMagnetometer.hpp>

namespace upm {

    /**
     * @brief BNO055 9DOF Fusion Hub
     * @defgroup bno055 libupm-bno055
     * @ingroup i2c gpio accelerometer compass bosch adafruit
     */

    /**
     * @library bno055
     * @sensor bno055
     * @comname Intelligent 9-axis Absolute Orientation Sensor
     * @type accelerometer compass
     * @man bosch adafruit
     * @con i2c gpio
     * @web https://www.adafruit.com/products/2472
     *
     * @brief API for the BNO055 Absolute Orientation 9DOF Fusion Hub
     *
     * The BNO055 is a System in Package (SiP), integrating a triaxial
     * 14-bit accelerometer, a triaxial 16-bit gyroscope with a range of
     * +/- 2000 degrees per second, a triaxial geomagnetic sensor and a
     * 32-bit cortex M0+ microcontroller running Bosch Sensortec sensor
     * fusion software, in a single package.
     *
     * This sensor handles the hard problem of combining various sensor
     * information into a reliable measurement of sensor orientation
     * (refered to as 'sensor fusion').  The onboard MCU runs this
     * software and can provide fusion output in the form of Euler
     * Angles, Quaternions, Linear Acceleration, and Gravity Vectors in
     * 3 axes.
     *
     * The focus on this driver has been on supporting the fusion
     * components.  Less support is available for use of this device as
     * a generic accelerometer, gyroscope and magnetometer, however
     * enough infrastructure is available to add any missing
     * functionality.
     *
     * This device requires calibration in order to operate accurately.
     * Methods are provided to retrieve calibration data (once
     * calibrated) to be stored somewhere else, like in a file.  A
     * method is provided to load this data as well.  Calibration data
     * is lost on a power cycle.  See one of the examples for a
     * description of how to calibrate the device, but in essence:
     *
     * There is a calibration status register available
     * (getCalibrationStatus()) that returns the calibration status of
     * the accelerometer (ACC), magnetometer (MAG), gyroscope (GYR), and
     * overall system (SYS).  Each of these values range from 0
     * (uncalibrated) to 3 (fully calibrated).  Calibration involves
     * certain motions to get all 4 values at 3.  The motions are as
     * follows (though see the datasheet for more information):
     *
     * GYR: Simply let the sensor sit flat for a few seconds.
     *
     * ACC: Move the sensor in various positions.  Start flat, then
     * rotate slowly by 45 degrees, hold for a few seconds, then
     * continue rotating another 45 degrees and hold, etc.  6 or more
     * movements of this type may be required.  You can move through any
     * axis you desire, but make sure that the device is lying at least
     * once perpendicular to the x, y, and z axis.
     *
     * MAG: Move slowly in a figure 8 pattern in the air, until the
     * calibration values reaches 3.
     *
     * SYS: This will usually reach 3 when the other items have also
     * reached 3.  If not, continue slowly moving the device though
     * various axes until it does.
     *
     * @snippet bno055.cxx Interesting
     */

    class BNO055: virtual public iAcceleration {

    public:
        /**
         * BNO055 constructor.
         *
         * By default, the constructor sets the acceleration units to
         * m/s^2, gyro and Euler units to degrees, and temperature to
         * celsius.  It then enters the NDOF fusion mode.
         *
         * In addition, the internal clock is used so that compatibility
         * with other implementations is assured.  If you are using a
         * device with an external clock, call setClockExternal(true) to
         * enable it.
         *
         * @param bus I2C bus to use.
         * @param address The address for this device.
         * @throws std::runtime_error on initialization failure.
         */
        BNO055(int bus=BNO055_DEFAULT_I2C_BUS,
               uint8_t addr=BNO055_DEFAULT_ADDR);

        /**
         * Instantiates BNO055 9DOF Fusion Hub based on a given string.
         *
         * @param initStr string containing specific information for BNO055 initialization.
         */
        BNO055(std::string initStr);

        /**
         * BNO055 Destructor.
         */
        virtual ~BNO055();

        /**
         * Update the internal stored values from sensor data.
         *
         * @throws std::runtime_error on failure.
         */
        void update();

        /**
         * Return the chip ID.
         *
         * @return The chip ID (BNO055_CHIPID).
         * @throws std::runtime_error on failure.
         */
        uint8_t getChipID();

        /**
         * Return the accelerometer chip ID.
         *
         * @return The chip ID.
         * @throws std::runtime_error on failure.
         */
        uint8_t getACCID();

        /**
         * Return the magnetometer chip ID.
         *
         * @return The chip ID.
         * @throws std::runtime_error on failure.
         */
        uint8_t getMAGID();

        /**
         * Return the gyroscope chip ID.
         *
         * @return The chip ID.
         * @throws std::runtime_error on failure.
         */
        uint8_t getGYRID();

        /**
         * Return the fusion firmware revison.
         *
         * @return The firmware revison.
         * @throws std::runtime_error on failure.
         */
        uint16_t getSWRevID();

        /**
         * Return the bootloader ID.
         *
         * @return The bootloader ID.
         * @throws std::runtime_error on failure.
         */
        uint8_t getBootLoaderID();

        /**
         * Enable or disables the use of the external clock.  The Adafruit
         * device does contain an external clock which might be more
         * stable.  By default, the internal clock is used.
         *
         * @param extClock true to use external clock, false otherwise.
         * @throws std::runtime_error on failure.
         */
        void setClockExternal(bool extClock);

        /**
         * Select the temperature source.  This can be the accelerometer
         * or the gyroscope.  By default, the accelerometer temperature is
         * used as the source.
         *
         * @param src One of the BNO055_TEMP_SOURCES_T values.
         * @throws std::runtime_error on failure.
         */
        void setTemperatureSource(BNO055_TEMP_SOURCES_T src);

        /**
         * Set the operating mode for the device.  This places the device
         * into a config mode, one of 7 non-fusion modes, or one of 5
         * fusion modes.  All stored sensor data is cleared when switching
         * modes.  The device must be in config mode for most
         * configuration operations.  See the datasheet for details.
         *
         * @param mode One of the BNO055_OPERATION_MODES_T values.
         * @throws std::runtime_error on failure.
         */
        void setOperationMode(BNO055_OPERATION_MODES_T mode);

        /**
         * Reboot the sensor.  This is equivalent to a power on reset.
         * All calibration data will be lost, and the device must be
         * recalibrated.
         * @throws std::runtime_error on failure.
         */
        void resetSystem();

        /**
         * Read the calibration status registers and return them.  The
         * values range from 0 (uncalibrated) to 3 (fully calibrated).
         *
         * @param mag The calibration status of the magnetometer.
         * @param acc The calibration status of the accelerometer.
         * @param mag The calibration status of the gyroscope.
         * @param mag The calibration status of the overall system.
         * @throws std::runtime_error on failure.
         */
        void getCalibrationStatus(int *mag, int *acc, int *gyr, int *sys);

        /**
         * Read the calibration status registers and return them as an
         * integer vector.  The values range from 0 (uncalibrated) to 3
         * (fully calibrated).
         *
         * @return An integer vector containing the values in the
         * order: mag, acc, gyr, and sys.
         * @throws std::runtime_error on failure.
         */
        std::vector<int> getCalibrationStatus();

        /**
         * Read the calibration status registers and return true or false,
         * indicating whether all of the calibration parameters are fully
         * calibrated.
         *
         * @return True if all 4 calibration parameters are fully
         * calibrated, else false.
         */
        bool isFullyCalibrated();

        /**
         * Read the calibration data and return it as a string.  This
         * data can then be saved for later reuse by
         * writeCalibrationData() to restore calibration data after a
         * reset.  The sensor must be fully calibrated before
         * calibration data can be read.
         *
         * @return A vector of uint8_t's representing the calibration
         * data.  This vector will always be exactly
         * BNO055_CALIBRATION_DATA_SIZE in size.
         * @throws std::runtime_error if an error occurs.
         */
        std::vector<uint8_t> readCalibrationData();

        /**
         * Write previously saved calibration data to the calibration
         * registers.
         *
         * @param calibrationData A vector of uint8_t (bytes) representing
         * calibration data as returned by readCalibrationData().
         * It's length must always be exactly BNO055_CALIBRATION_DATA_SIZE.
         * @throws std::length_error if the vector size is not equal to
         * BNO055_CALIBRATION_DATA_SIZE.
         */
        void writeCalibrationData(std::vector<uint8_t> calibrationData);

        /**
         * Return the current measured temperature.  Note, this is not
         * ambient temperature - this is the temperature of the selected
         * source on the chip.  update() must have been called prior to
         * calling this method.
         *
         * @param fahrenheit true to return data in Fahrenheit, false for
         * Celicus.  Celsius is the default.
         * @return The temperature in degrees Celsius or Fahrenheit.
         */
        float getTemperature(bool fahrenheit=false);

        /**
         * Return current orientation fusion data in the form of Euler
         * Angles.  By default, the returned values are in degrees.
         * update() must have been called prior to calling this method.
         *
         * @param heading Pointer to a floating point value that will have
         * the current heading angle placed into it.
         * @param roll Pointer to a floating point value that will have
         * the current roll angle placed into it.
         * @param pitch Pointer to a floating point value that will have
         * the current pitch angle placed into it.
         */
        void getEulerAngles(float *heading, float *roll, float *pitch);

        /**
         * Return current orientation fusion data in the form of Euler
         * Angles as a floating point vector.  By default, the returned
         * values are in degrees.  update() must have been called prior to
         * calling this method.
         *
         * @return A floating point vector containing heading, roll, and
         * pitch, in that order.
         */
        std::vector<float> getEulerAngles();

        /**
         * Return current orientation fusion data in the form of
         * Quaternions.  update() must have been called prior to calling
         * this method.
         *
         * @param w Pointer to a floating point value that will have
         * the current w component placed into it.
         * @param x Pointer to a floating point value that will have
         * the current x component placed into it.
         * @param y Pointer to a floating point value that will have
         * the current y component placed into it.
         * @param z Pointer to a floating point value that will have
         * the current z component placed into it.
         */
        void getQuaternions(float *w, float *x, float *y, float *z);

        /**
         * Return current orientation fusion data in the form of
         * Quaternions, as a floating point vector.  update() must have
         * been called prior to calling this method.
         *
         * @return A floating point vector containing w, x, y, and z in
         * that order.
         */
        std::vector<float> getQuaternions();

        /**
         * Return current orientation fusion data in the form of Linear
         * Acceleration.  By default the returned values are in meters
         * per-second squared (m/s^2).  update() must have been called
         * prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have
         * the current x component placed into it.
         * @param y Pointer to a floating point value that will have
         * the current y component placed into it.
         * @param z Pointer to a floating point value that will have
         * the current z component placed into it.
         */
        void getLinearAcceleration(float *x, float *y, float *z);

        /**
         * Return current orientation fusion data in the form of Linear
         * Acceleration, as a floating point vector.  update() must have
         * been called prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getLinearAcceleration();

        /**
         * Return current orientation fusion data in the form of a Gravity
         * Vector per-axis.  By default the returned values are in meters
         * per-second squared (m/s^2).  update() must have been called
         * prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have
         * the current x component placed into it.
         * @param y Pointer to a floating point value that will have
         * the current y component placed into it.
         * @param z Pointer to a floating point value that will have
         * the current z component placed into it.
         */
        void getGravityVectors(float *x, float *y, float *z);

        /**
         * Return current orientation fusion data in the form of a Gravity
         * Vector per-axis as a floating point vector.  update() must have
         * been called prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getGravityVectors();

        /**
         * Return uncompensated accelerometer data (non-fusion).  In
         * fusion modes, this data will be of little value.  By default
         * the returned values are in meters per-second squared (m/s^2).
         * update() must have been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have
         * the current x component placed into it.
         * @param y Pointer to a floating point value that will have
         * the current y component placed into it.
         * @param z Pointer to a floating point value that will have
         * the current z component placed into it.
         */
        void getAccelerometer(float *x, float *y, float *z);

        /**
         * Return current uncompensated accelerometer (non-fusion) data in
         * the form of a floating point vector.  By default the returned
         * values are in meters per-second squared (m/s^2).  update() must
         * have been called prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getAccelerometer();

        /**
         * get acceleration values
         * 
         * @return stl vector of size 3 representing the 3 axis
         */
        virtual std::vector<float> getAcceleration();

        /**
         * Return uncompensated magnetometer data (non-fusion).  In fusion
         * modes, this data will be of little value.  The returned values
         * are in micro-teslas (uT).  update() must have been called prior
         * to calling this method.
         *
         * @param x Pointer to a floating point value that will have
         * the current x component placed into it.
         * @param y Pointer to a floating point value that will have
         * the current y component placed into it.
         * @param z Pointer to a floating point value that will have
         * the current z component placed into it.
         */
        void getMagnetometer(float *x, float *y, float *z);

        /**
         * Return current uncompensated magnetometer (non-fusion) data in
         * the form of a floating point vector.  The returned values are in
         * micro-teslas (uT).  update() must have been called prior to
         * calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getMagnetometer();

        /**
         * Return uncompensated gyroscope data (non-fusion).  In fusion
         * modes, this data will be of little value.  By default the
         * returned values are in meters per-second squared (m/s^2).
         * update() must have been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have
         * the current x component placed into it.
         * @param y Pointer to a floating point value that will have
         * the current y component placed into it.
         * @param z Pointer to a floating point value that will have
         * the current z component placed into it.
         */
        void getGyroscope(float *x, float *y, float *z);

        /**
         * Return current uncompensated gyroscope (non-fusion) data in the
         * form of a floating point vector.  By default the returned values
         * are in meters per-second squared (m/s^2).  update() must have
         * been called prior to calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getGyroscope();

        /**
         * Set the bandwidth, range, and power modes of the accelerometer.
         * In fusion modes, these values will be ignored.
         *
         * @param range One of the BNO055_ACC_RANGE_T values.
         * @param bw One of the BNO055_ACC_BW_T values.
         * @param pwr One of the BNO055_ACC_PWR_MODE_T values.
         * @throws std::runtime_error on failure.
         */
        void setAccelerationConfig(BNO055_ACC_RANGE_T range,
                                   BNO055_ACC_BW_T bw,
                                   BNO055_ACC_PWR_MODE_T pwr);

        /**
         * Set the output data rate, operating mode and power mode of the
         * magnetometer.  In fusion modes, these values will be ignored.
         *
         * @param odr One of the BNO055_MAG_ODR_T values.
         * @param opr One of the BNO055_MAG_OPR_T values.
         * @param pwr One of the BNO055_MAG_POWER_T values.
         * @throws std::runtime_error on failure.
         */
        void setMagnetometerConfig(BNO055_MAG_ODR_T odr,
                                   BNO055_MAG_OPR_T opr,
                                   BNO055_MAG_POWER_T pwr);

        /**
         * Set the range, bandwidth and power modes of the gyroscope.  In
         * fusion modes, these values will be ignored.
         *
         * @param range One of the BNO055_GYR_RANGE_T values.
         * @param bw One of the BNO055_GYR_BW_T values.
         * @param pwr One of the BNO055_GYR_POWER_MODE_T values.
         * @throws std::runtime_error on failure.
         */
        void setGyroscopeConfig(BNO055_GYR_RANGE_T range,
                                BNO055_GYR_BW_T bw,
                                BNO055_GYR_POWER_MODE_T pwr);

        /**
         * Set the unit of measurement for the accelerometer related
         * sensor values.  The choices are mg (milligravities) or meters
         * per-second squared (m/s^2).  The default is m/s^2.
         *
         * @param mg true for mg, false for m/s^2.
         * @throws std::runtime_error on failure.
         */
        void setAccelerometerUnits(bool mg=false);

        /**
         * Set the unit of measurement for the gyroscope related sensor
         * values.  The choices are degrees and radians.  The default is
         * degrees.
         *
         * @param radians true for radians, false for degrees.
         * @throws std::runtime_error on failure.
         */
        void setGyroscopeUnits(bool radians=false);

        /**
         * Set the unit of measurement for the Euler Angle related sensor
         * values.  The choices are degrees and radians.  The default is
         * degrees.
         *
         * @param radians true for radians, false for degrees.
         * @throws std::runtime_error on failure.
         */
        void setEulerUnits(bool radians=false);

        /**
         * Reset all interrupt status bits and interrupt output.
         *
         * @throws std::runtime_error on failure.
         */
        void resetInterruptStatus();

        /**
         * Return the interrupt status register.  This is a bitmask of the
         * BNO055_INT_STA_BITS_T bits.
         *
         * @return a bitmask of BNO055_INT_STA_BITS_T bits.
         * @throws std::runtime_error on failure.
         */
        uint8_t getInterruptStatus();

        /**
         * Return the interrupt enables register.  This is a bitmask of the
         * BNO055_INT_STA_BITS_T bits.
         *
         * @return a bitmask of BNO055_INT_STA_BITS_T bits currently set in the
         * enable register.
         * @throws std::runtime_error on failure.
         */
        uint8_t getInterruptEnable();

        /**
         * Set the interrupt enable register. This is composed of a
         * bitmask of the BNO055_INT_STA_BITS_T bits.
         *
         * @param enables a bitmask of BNO055_INT_STA_BITS_T bits to enable
         * @throws std::runtime_error on failure.
         */
        void setInterruptEnable(uint8_t enables);

        /**
         * Return the interrupt mask register.  This is a bitmask of
         * the BNO055_INT_STA_BITS_T bits.  The interrupt mask is used
         * to mask off enabled interrupts from generating a hardware
         * interrupt.  The interrupt status register can still be used
         * to detect masked interrupts if they are enabled.
         *
         * @return a bitmask of BNO055_INT_STA_BITS_T bits currently set in the
         * interrupt mask register.
         * @throws std::runtime_error on failure.
         */
        uint8_t getInterruptMask();

        /**
         * Set the interrupt mask register.  This is a bitmask of the
         * BNO055_INT_STA_BITS_T bits.  The interrupt mask is used to
         * mask off enabled interrupts from generating a hardware
         * interrupt.  The interrupt status register can still be used
         * to detect masked interrupts if they are enabled.
         *
         * @param mask A bitmask of BNO055_INT_STA_BITS_T bits to set in
         * the interrupt mask register.
         * @throws std::runtime_error on failure.
         */
        void setInterruptMask(uint8_t mask);

        /**
         * Return the value of the system status register.  This method
         * can be used to determine the overall status of the device.
         *
         * @return One of the BNO055_SYS_STATUS_T values.
         * @throws std::runtime_error on failure.
         */
        BNO055_SYS_STATUS_T getSystemStatus();

        /**
         * Return the value of the system error register.  This mathod can
         * be used to determine a variety of system related error
         * conditions.
         *
         * @return One of the BNO055_SYS_ERR_T values.
         * @throws std::runtime_error on failure.
         */
        BNO055_SYS_ERR_T getSystemError();


        /**
         * install an interrupt handler.
         *
         * @param gpio gpio pin to use as interrupt pin
         * @param level the interrupt trigger level (one of mraa_gpio_edge_t
         * values).  Make sure that you have configured the interrupt pin
         * properly for whatever level you choose.
         * @param isr the interrupt handler, accepting a void * argument
         * @param arg the argument to pass the the interrupt handler
         * @throws std::runtime_error on failure.
         */
        void installISR(int gpio, mraa_gpio_edge_t level,
                        void (*isr)(void *), void *arg);

        /**
         * uninstall a previously installed interrupt handler
         *
         */
        void uninstallISR();

    protected:
        bno055_context m_bno055;
        mraa::MraaIo mraaIo;

        /**
         * Set the current internal device register page.  This is a low
         * level function and should not be used unless you know what you
         * are doing.
         *
         * @param dev The device context.
         * @param page The page number to set.  This can only be 0 or 1.
         * @param force If true, force the device page state to match
         * indicated internal page state regardless of current state.
         * @throws std::runtime_error on failure.
         */
        void setPage(uint8_t page, bool force=false);

        /**
         * Read a register.
         *
         * @param reg The register to read
         * @return The value of the register
         * @throws std::runtime_error on failure.
         */
        uint8_t readReg(uint8_t reg);

        /**
         * Read contiguous registers into a buffer.
         *
         * @param buffer The buffer to store the results
         * @param len The number of registers to read
         * @throws std::runtime_error on failure.
         */
        void readRegs(uint8_t reg, uint8_t *buffer, int len);

        /**
         * Write to a register
         *
         * @param reg The register to write to
         * @param val The value to write
         * @throws std::runtime_error on failure.
         */
        void writeReg(uint8_t reg, uint8_t val);

        /**
         * Write data to contiguous registers
         *
         * @param reg The starting register to write to
         * @param buffer The buffer containing the data to write
         * @param len The number of bytes to write
         * @throws std::runtime_error on failure.
         */
        void writeRegs(uint8_t reg, uint8_t *buffer, int len);

    private:
        /* Disable implicit copy and assignment operators */
        BNO055(const BNO055&) = delete;
        BNO055 &operator=(const BNO055&) = delete;

    };
}
