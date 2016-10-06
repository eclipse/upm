/*
 * Author: Lay, Kuan Loon <kuan.loon.lay@intel.com>
 *         Jon Trulson <jtrulson@ics.com>
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
 *
 * Thanks to https://github.com/01org/android-iio-sensors-hal for gyroscope
 * calibration and denoise algorithm.
 */
#pragma once

#include <string>
#include <mraa/iio.h>
#include <mraa/i2c.hpp>

#define L3GD20_DEFAULT_I2C_BUS                      0
// if SDO tied to GND
#define L3GD20_DEFAULT_I2C_ADDR                     0x6a
#define L3GD20_DEFAULT_CHIP_ID                      0xd4
// the 'H' variant uses a different chip id
#define L3GD20H_DEFAULT_CHIP_ID                     0xd7

namespace upm
{
/**
 * @brief L3GD20 Tri-axis Digital Gyroscope
 * @defgroup l3gd20 libupm-l3gd20
 * @ingroup stmicro iio i2c tri-axis digital gyroscope
 */

/**
 * @library l3gd20
 * @sensor l3gd20
 * @comname L3GD20 Tri-axis Digital Gyroscope
 * @type gyroscope
 * @man stmicro
 * @con iio i2c
 *
 * @brief L3GD20 Tri-axis Digital Gyroscope API
 *
 * The L3GD20 The L3GD20 is a low-power three-axis angular rate
 * sensor.  This driver supports IIO and I2C modes.  Some methods will
 * only work in one mode or the other.  See the documentation on the
 * methods to determine whether a given method is operation in a given
 * mode.  Both the I2C and IIO mechanisms make use of the calibration and
 * denoise algorithms.
 *
 * For I2C mode, not all capabilities of the device are supported, but
 * a complete register map and low level read/write methods are
 * provided to add any missing functionality.
 *
 * Example using IIO
 * @snippet l3gd20.cxx Interesting
 * Example using I2C
 * @snippet l3gd20-i2c.cxx Interesting
 */

class L3GD20
{
  public:
    typedef struct {
        float bias_x, bias_y, bias_z;
        int count;
        float min_x, min_y, min_z;
        float max_x, max_y, max_z;
    } gyro_cal_t;

    typedef struct {
        float* buff;
        unsigned int idx;
        unsigned int count;
        unsigned int sample_size;
    } filter_median_t;

    // NOTE: Reserved registers must not be written into or permanent
    // device damage can result.  Reading from them may return
    // indeterminate values.  Registers containing reserved bitfields
    // must be written as 0.  Reading reserved bitfields may return
    // indeterminate values.

    /**
     * L3GD20 registers (i2c)
     */
    typedef enum {
      // 0x00-0x0e reserved

      REG_WHO_AM_I                            = 0x0f,

      // 0x10-0x1f reserved

      REG_CTRL_REG1                           = 0x20,
      REG_CTRL_REG2                           = 0x21,
      REG_CTRL_REG3                           = 0x22,
      REG_CTRL_REG4                           = 0x23,
      REG_CTRL_REG5                           = 0x24,

      REG_REFERENCE                           = 0x25,

      REG_OUT_TEMPERATURE                     = 0x26,

      REG_STATUS_REG                          = 0x27,

      // output registers (also for FIFO output)
      REG_OUT_X_L                             = 0x28,
      REG_OUT_X_H                             = 0x29,

      REG_OUT_Y_L                             = 0x2a,
      REG_OUT_Y_H                             = 0x2b,

      REG_OUT_Z_L                             = 0x2c,
      REG_OUT_Z_H                             = 0x2d,

      REG_FIFO_CTRL_REG                       = 0x2e,
      REG_FIFO_SRC_REG                        = 0x2f,

      REG_INT1_CFG                            = 0x30,
      REG_INT1_SRC                            = 0x31,

      REG_INT1_TSH_XH                         = 0x32,
      REG_INT1_TSH_XL                         = 0x33,

      REG_INT1_TSH_YH                         = 0x34,
      REG_INT1_TSH_YL                         = 0x35,

      REG_INT1_TSH_ZH                         = 0x36,
      REG_INT1_TSH_ZL                         = 0x37,
      REG_INT1_DURATION                       = 0x38
    } L3GD20_REGS_T;

    /**
     * CTRL_REG1 bits
     */
    typedef enum {
      CTRL_REG1_YEN                           = 0x01,
      CTRL_REG1_XEN                           = 0x02,
      CTRL_REG1_ZEN                           = 0x04,
      CTRL_REG1_PD                            = 0x08,

      CTRL_REG1_BW0                           = 0x10, // bandwidth
      CTRL_REG1_BW1                           = 0x20,
      _CTRL_REG1_BW_MASK                      = 3,
      _CTRL_REG1_BW_SHIFT                     = 4,

      CTRL_REG1_DR0                           = 0x40, // data rate
      CTRL_REG1_DR1                           = 0x80,
      _CTRL_REG1_DR_MASK                      = 3,
      _CTRL_REG1_DR_SHIFT                     = 6,

      // together the BW and DR modes represent an output data rate
      // (ODR) and a filter cut-off.  So here, we will create a 'fake'
      // bitfield that can be used directly with the ODR_CUTOFF enum
      _CTRL_REG1_ODR_CUTOFF0                  = 0x10,
      _CTRL_REG1_ODR_CUTOFF1                  = 0x20,
      _CTRL_REG1_ODR_CUTOFF2                  = 0x40,
      _CTRL_REG1_ODR_CUTOFF3                  = 0x80,
      _CTRL_REG1_ODR_CUTOFF_MASK              = 15,
      _CTRL_REG1_ODR_CUTOFF_SHIFT             = 4

    } CTRL_REG1_BITS_T;

    /**
     * CTRL_REG1_ODR_CUTOFF values
     */
    typedef enum {
      ODR_CUTOFF_95_12_5                      = 0, // ODR 95Hz, CO 12.5
      ODR_CUTOFF_95_25                        = 1, // ODR 95Hz, CO 25
      // 2 and 3 same as 1

      ODR_CUTOFF_190_12_5                     = 4,
      ODR_CUTOFF_190_25                       = 5,
      ODR_CUTOFF_190_50                       = 6,
      ODR_CUTOFF_190_70                       = 7,

      ODR_CUTOFF_380_20                       = 8,
      ODR_CUTOFF_380_25                       = 9,
      ODR_CUTOFF_380_50                       = 10,
      ODR_CUTOFF_380_100                      = 11,

      ODR_CUTOFF_760_30                       = 12,
      ODR_CUTOFF_760_35                       = 13,
      ODR_CUTOFF_760_50                       = 14,
      ODR_CUTOFF_760_100                      = 15
    } ODR_CUTOFF_T;

    /**
     * CTRL_REG1 power modes. Power is controlled via the PD, Zen,
     * Yen, and Xen bitfields.
     */
    typedef enum {
      POWER_DOWN,
      POWER_SLEEP,
      POWER_NORMAL
    } POWER_MODES_T;

    /**
     * CTRL_REG2 bits
     */
    typedef enum {
      _CTRL_REG2_RESERVED_BITS                = 0x40 | 0x80,

      CTRL_REG2_HPCF0                         = 0x01, // highpass filter cutoff
      CTRL_REG2_HPCF1                         = 0x02,
      CTRL_REG2_HPCF2                         = 0x04,
      CTRL_REG2_HPCF3                         = 0x08,
      _CTRL_REG2_HPCF_MASK                    = 15,
      _CTRL_REG2_HPCF_SHIFT                   = 0,

      CTRL_REG2_HPM0                          = 0x10, // highpass filter mode
      CTRL_REG2_HPM1                          = 0x20,
      _CTRL_REG2_HPM_MASK                     = 3,
      _CTRL_REG2_HPM_SHIFT                    = 4

      // 0x40-0x80 reserved
    } CTRL_REG2_BITS_T;


    /**
     * CTRL_REG2_HPCF values (see table 26 in the datasheet)
     */
    typedef enum {
      HPCF_7_2                                = 0, // 7.2Hz CO (w/ ODR@95Hz)
      HPCF_3_5                                = 1,
      HPCF_1_8                                = 2,
      HPCF_0_9                                = 3,
      HPCF_0_45                               = 4,
      HPCF_0_18                               = 5,
      HPCF_0_09                               = 6,
      HPCF_0_045                              = 7,
      HPCF_0_018                              = 8,
      HPCF_0_009                              = 9
    } HPCF_T;

    /**
     * CTRL_REG2_HPM values
     */
    typedef enum {
      HPM_NORMAL_RESET_FILTER                 = 0,
      HPM_REFERENCE_SIGNAL                    = 1,
      HPM_NORMAL                              = 2,
      HPM_AUTORESET_ON_INT                    = 3
    } HPM_T;

    /**
     * CTRL_REG3 bits
     */
    typedef enum {
      CTRL_REG3_I2_EMPTY                      = 0x01,
      CTRL_REG3_I2_ORUN                       = 0x02,
      CTRL_REG3_I2_WTM                        = 0x04,
      CTRL_REG3_I2_DRDY                       = 0x08,
      CTRL_REG3_PP_OD                         = 0x10,
      CTRL_REG3_H_LACTIVE                     = 0x20,
      CTRL_REG3_I1_BOOT                       = 0x40,
      CTRL_REG3_I1_INT1                       = 0x80
    } CTRL_REG3_BITS_T;

    /**
     * CTRL_REG4 bits
     */
    typedef enum {
      _CTRL_REG4_RESERVED_BITS                = 0x02 | 0x04 | 0x08,

      CTRL_REG4_SIM                           = 0x01, // SPI 3 or 4 wire
      // 0x02-0x08 reserved

      CTRL_REG4_FS0                           = 0x10, // full scale select
      CTRL_REG4_FS1                           = 0x20,
      _CTRL_REG4_FS_MASK                      = 3,
      _CTRL_REG4_FS_SHIFT                     = 4,

      CTRL_REG4_BLE                           = 0x40, // endian selection
      CTRL_REG4_BDU                           = 0x80  // block updating
    } CTRL_REG4_BITS_T;

    /**
     * CTRL_REG4_FS values
     */
    typedef enum {
      FS_250                                  = 0, // 250 deg/s
      FS_500                                  = 1,
      FS_2000                                 = 2
    } FS_T;

    /**
     * CTRL_REG5 bits
     */
    typedef enum {
      _CTRL_REG5_RESERVED_BITS                = 0x20,

      CTRL_REG5_OUT_SEL0                      = 0x01,
      CTRL_REG5_OUT_SEL1                      = 0x02,
      _CTRL_REG5_OUT_SEL_MASK                 = 3,
      _CTRL_REG5_OUT_SEL_SHIFT                = 0,

      CTRL_REG5_INT1_SEL0                     = 0x04,
      CTRL_REG5_INT1_SEL1                     = 0x08,
      _CTRL_REG5_INT1_SEL_MASK                = 3,
      _CTRL_REG5_INT1_SEL_SHIFT               = 2,

      CTRL_REG5_HPEN                          = 0x10,

      // 0x20 reserved
      CTRL_REG5_FIFO_EN                       = 0x40,
      CTRL_REG5_BOOT                          = 0x80
    } CTRL_REG5_BITS_T;

    /**
     * STATUS_REG bits
     */
    typedef enum {
      STATUS_REG_XDA                          = 0x01, // axis data avail
      STATUS_REG_YDA                          = 0x02,
      STATUS_REG_ZDA                          = 0x04,
      STATUS_REG_ZYXDA                        = 0x08,

      STATUS_REG_XOR                          = 0x10, // axis data overrun
      STATUS_REG_YOR                          = 0x20,
      STATUS_REG_ZOR                          = 0x40,
      STATUS_REG_ZYXOR                        = 0x80
    } STATUS_REG_BITS_T;

    /**
     * FIFO_CTRL_REG bits
     */
    typedef enum {
      FIFO_CTRL_REG_WTM0                      = 0x01, // FIFO watermark
      FIFO_CTRL_REG_WTM1                      = 0x02,
      FIFO_CTRL_REG_WTM2                      = 0x04,
      FIFO_CTRL_REG_WTM3                      = 0x08,
      FIFO_CTRL_REG_WTM4                      = 0x10,
      _FIFO_CTRL_REG_WTM_MASK                 = 31,
      _FIFO_CTRL_REG_WTM_SHIFT                = 0,

      FIFO_CTRL_REG_FM0                       = 0x20, // FIFO mode
      FIFO_CTRL_REG_FM1                       = 0x40,
      FIFO_CTRL_REG_FM2                       = 0x80,
      _FIFO_CTRL_REG_FM_MASK                  = 7,
      _FIFO_CTRL_REG_FM_SHIFT                 = 5
    } FIFO_CTRL_REG_BITS_T;

    /**
     * FIFO_CTRL_REG_FM (FIFO mode) values
     */
    typedef enum {
      FIFO_MODE_BYPASS                        = 0,
      FIFO_MODE_FIFO                          = 1,
      FIFO_MODE_STREAM                        = 2,
      FIFO_MODE_STREAM_TO_FIFO                = 3,
      FIFO_MODE_BYPASS_TO_STREAM              = 4
    } FIFO_MODE_T;


    /**
     * FIFO_SRC_REG bits
     */
    typedef enum {
      FIFO_SRC_REG_FSS0                       = 0x01, // FIFO stored data level
      FIFO_SRC_REG_FSS1                       = 0x02,
      FIFO_SRC_REG_FSS2                       = 0x04,
      FIFO_SRC_REG_FSS3                       = 0x08,
      FIFO_SRC_REG_FSS4                       = 0x10,
      _FIFO_SRC_REG_FSS_MASK                  = 31,
      _FIFO_SRC_REG_FSS_SHIFT                 = 0,

      FIFO_SRC_REG_EMPTY                      = 0x20,
      FIFO_SRC_REG_OVRN                       = 0x40,
      FIFO_SRC_REG_WTM                        = 0x80
    } FIFO_SRC_BITS_T;

    /**
     * INT1_CFG bits
     */
    typedef enum {
      INT1_CFG_XLIE                           = 0x01, // low intr en
      INT1_CFG_XHIE                           = 0x02, // high intr en

      INT1_CFG_YLIE                           = 0x04,
      INT1_CFG_YHIE                           = 0x08,

      INT1_CFG_ZLIE                           = 0x10,
      INT1_CFG_ZHIE                           = 0x20,

      INT1_CFG_LIR                            = 0x40,
      INT1_CFG_AND_OR                         = 0x80
    } INT1_CFG_BITS_T;

    /**
     * INT1_SRC bits
     */
    typedef enum {
      _INT1_SRC_RESERVED_BITS                 = 0x80,

      INT1_SRC_XL                             = 0x01, // X low intr
      INT1_SRC_XH                             = 0x02, // X high intr

      INT1_SRC_YL                             = 0x04,
      INT1_SRC_YH                             = 0x08,

      INT1_SRC_ZL                             = 0x10,
      INT1_SRC_ZH                             = 0x20,

      INT1_SRC_IA                             = 0x40 // intr active

      // 0x80 reserved
    } INT1_SRC_BITS_T;

    /**
     * INT1_DURATION bits
     */
    typedef enum {
      INT1_DURATION_D0                        = 0x01,
      INT1_DURATION_D1                        = 0x02,
      INT1_DURATION_D2                        = 0x04,
      INT1_DURATION_D3                        = 0x08,
      INT1_DURATION_D4                        = 0x10,
      INT1_DURATION_D5                        = 0x20,
      INT1_DURATION_D6                        = 0x40,

      INT1_DURATION_WAIT                      = 0x80
    } INT1_DURATION_BITS_T;


    /**
     * L3GD20 Tri-axis Digital Gyroscope Contructor for IIO operation
     *
     * @param iio device number
     */
    L3GD20(int device);

    /**
     * L3GD20 Tri-axis Digital Gyroscope Contructor for I2C operation
     *
     * @param bus i2c bus
     * @param addr I2C address
     */
    L3GD20(int bus, int addr);

    /**
     * L3GD20 destructor
     */
    ~L3GD20();

    /**
     * Return the chip ID.  I2C only.
     *
     * @return The chip ID (L3GD20_DEFAULT_CHIP_ID).
     */
    uint8_t getChipID();

    /**
     * Return gyroscope data in radians per second.  update() must
     * have been called prior to calling this method.  I2C only.
     *
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it.
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it.
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it.
     */
    void getGyroscope(float *x, float *y, float *z);

    /**
     * Set the power mode of the device.  I2C only.
     *
     * @param power One of the POWER_MODES_T values.
     */
    void setPowerMode(POWER_MODES_T mode);

    /**
     * Set the gyroscope detection scaling range.  This device
     * supports 250, 500 and 2000 degree/s ranges.  I2C only.
     *
     * @param range One of the FS_T values.
     */
    void setRange(FS_T range);

    /**
     * Update the internal stored values from sensor data.  This
     * method must be called before querying any data
     * (getTemperature() and getGyroscope()).  I2C only.
     */
    void update();

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature.  update() must have been called prior to
     * calling this method.  I2C only.
     *
     * @param fahrenheit true to return data in Fahrenheit, false for
     * Celicus.  Celsius is the default.
     * @return The temperature in degrees Celsius or Fahrenheit.
     */
   float getTemperature(bool fahrenheit=false);

    /**
     * Set the output data rate and cut off frequency of the device.
     * I2C only.
     *
     * @param odr One of the ODR_CUTOFF_T values.
     */
    void setODR(ODR_CUTOFF_T odr);

    /**
     * Enable or disable Block Data Update.  When enabled, this
     * ensures that LSB's or MSB's of a given axis are not being
     * updated while the other is being read.  This is enabled by
     * default.  I2C only.
     *
     * @param enable true to enable, false to disable
     */
    void enableBDU(bool enable);

    /**
     * Return the bitfields of the Status register.  This register
     * provides information on the status of data gathering.  I2C
     * only.
     *
     * @return The contents of the REG_STATUS_REG register.
     */
    uint8_t getStatusBits();

    /**
     * Installs an interrupt service routine (ISR) to be called when
     * an interrupt occurs.  IIO only.
     *
     * @param interrupt channel
     * @param fptr Pointer to a function to be called on interrupt
     * @param arg Pointer to an object to be supplied as an
     * argument to the ISR.
     */
    void installISR(void (*isr)(char*), void* arg);

    /**
     * Extract the channel value based on channel type. IIO only.
     *
     * @param input Channel data
     * @param chan MRAA iio-layer channel info
     */
    int64_t getChannelValue(unsigned char* input, mraa_iio_channel* chan);

    /**
     * Enable trigger buffer.  IIO only.
     *
     * @param trigger buffer length in integer
     */
    bool enableBuffer(int length);

    /**
     * Disable trigger buffer.   IIO only.
     */
    bool disableBuffer();

    /**
     * Set scale.  IIO only.  For I2C operation, use setRange() with
     * the appropriate FS_T value.
     *
     * @param scale in float
     * Available scales are 0.000153(250dps), 0.000305(500dps), and
     * 0.001222(2000dps) Default scale is 0.000153
     */
    bool setScale(const float scale);

    /**
     * Set sampling frequency.  IIO only.  For I2C operation, use the
     * setODR() method with the appropriate ODR_CUTOFF_T value.
     *
     * @param sampling frequency in float
     * Available sampling frequency are 95, 190, 380, and 760
     * Default sampling frequency is 95
     */
    bool setSamplingFrequency(const float sampling_frequency);

    /**
     * Enable 3 axis scan element.  IIO only.
     */
    bool enable3AxisChannel();

    /**
     * Process enabled channel buffer and return x, y, z axis. IIO only.
     * @param data Enabled channel data, 6 bytes, each axis 2 bytes
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    bool extract3Axis(char* data, float* x, float* y, float* z);

    /**
     * Reset calibration data and start collect calibration data again
     */
    void initCalibrate();

    /**
     * Get calibrated status, return true if calibrate successfully
     */
    bool getCalibratedStatus();

    /**
     * Get calibrated data
     */
    void getCalibratedData(float* bias_x, float* bias_y, float* bias_z);

    /**
     * Load calibrated data
     */
    void loadCalibratedData(float bias_x, float bias_y, float bias_z);

    /**
     * Read a register. I2C mode only.
     *
     * @param reg The register to read.
     * @return The value of the register.
     */
    uint8_t readReg(uint8_t reg);

    /**
     * Read contiguous registers into a buffer. I2C mode only.
     *
     * @param buffer The buffer to store the results.
     * @param len The number of registers to read.
     * @return The number of bytes read.
     */
    int readRegs(uint8_t reg, uint8_t *buffer, int len);

    /**
     * Write to a register.  I2C mode only.
     *
     * @param reg The register to write to.
     * @param val The value to write.
     */
    void writeReg(uint8_t reg, uint8_t val);

    /**
     * Calibrate gyro
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    bool gyroCollect(float x, float y, float z);

    /**
     * Denoise gyro
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    void gyroDenoiseMedian(float* x, float* y, float* z);

    /**
     * median algorithm
     * @param queue
     * @param size
     */
    float median(float* queue, unsigned int size);

    /**
     * partition algorithm
     * @param list
     * @param left
     * @param right
     * @param pivot_index
     */
    unsigned int
    partition(float* list, unsigned int left, unsigned int right, unsigned int pivot_index);

    /**
     * Clamp Gyro Readings to Zero
     * @param x X-Axis
     * @param y Y-Axis
     * @param z Z-Axis
     */
    void clampGyroReadingsToZero(float* x, float* y, float* z);

  protected:
    mraa::I2c *m_i2c;
    float m_gyrScale;
    float m_gyrX;
    float m_gyrY;
    float m_gyrZ;
    float m_temperature;

  private:
    mraa_iio_context m_iio;

    int m_iio_device_num;
    bool m_mount_matrix_exist; // is mount matrix exist
    float m_mount_matrix[9];   // mount matrix
    float m_scale;             // gyroscope data scale
    int m_event_count;         // sample data arrive
    bool m_calibrated;         // calibrate state
    gyro_cal_t m_cal_data;     // calibrate data
    filter_median_t m_filter;  // filter data
};
}
