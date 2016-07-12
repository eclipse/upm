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

#include <string>
#include <mraa/i2c.hpp>
#include <mraa/spi.hpp>
#include <mraa/gpio.hpp>

#define BMG160_I2C_BUS 0
#define BMG160_SPI_BUS 0
#define BMG160_DEFAULT_ADDR 0x68


namespace upm {

  /**
   * @library bmx050
   * @sensor bmg160
   * @comname BMG160 16 bit Trixial Gyroscope
   * @type gyro
   * @man bosch
   * @con i2c spi gpio
   *
   * @brief API for the BMG160 16 bit Trixial Gyroscope
   *
   * The BMG160 is a 3-axis angular rate sensor that is made of a
   * surface micro machined sensing element and an evaluation ASIC.
   * Both parts are packed into one single LGA 3.0mm x 3.0mm x 0.95mm
   * housing.  The BMG160 is designed to meet requirements for
   * consumer applications such as image stabilization (DSC and
   * camera-phone), gaming and pointing devices. It is capable to
   * measure angular rates in three perpendicular room dimensions, the
   * x-, y- and z-axis, and to provide the corresponding output
   * signals. The BMG160 is fitted with digital bi-directional SPI and
   * I2C interfaces for optimum system integration.
   *
   * Not all functionality of this chip has been implemented in this
   * driver, however all the pieces are present to add any desired
   * functionality.  This driver supports both I2C (default) and SPI
   * operation.
   *
   * This device requires 3.3v operation.
   *
   * @snippet bmg160.cxx Interesting
   */

  class BMG160 {
  public:
    // special reset byte
    static const uint8_t BMG160_RESET_BYTE = 0xb6;

    // NOTE: Reserved registers must not be written into.  Reading
    // from them may return indeterminate values.  Registers
    // containing reserved bitfields must be written as 0.  Reading
    // reserved bitfields may return indeterminate values.

    /**
     * BMG160 registers
     */
    typedef enum : uint8_t {
      REG_CHIP_ID                      = 0x00,

      // 0x01 reserved

      REG_RATE_X_LSB                   = 0x02,
      REG_RATE_X_MSB                   = 0x03,
      REG_RATE_Y_LSB                   = 0x04,
      REG_RATE_Y_MSB                   = 0x05,
      REG_RATE_Z_LSB                   = 0x06,
      REG_RATE_Z_MSB                   = 0x07,

      REG_TEMP                         = 0x08,

      REG_INT_STATUS_0                 = 0x09,
      REG_INT_STATUS_1                 = 0x0a,
      REG_INT_STATUS_2                 = 0x0b,
      REG_INT_STATUS_3                 = 0x0c,

      // 0x0d reserved

      REG_FIFO_STATUS                  = 0x0e,

      REG_GYR_RANGE                    = 0x0f,
      REG_GYR_BW                       = 0x10,
      REG_LPM1                         = 0x11,
      REG_LPM2                         = 0x12,

      REG_RATE_HBW                     = 0x13,

      REG_SOFTRESET                    = 0x14,

      REG_INT_EN_0                     = 0x15,
      REG_INT_EN_1                     = 0x16,

      REG_INT_MAP_0                    = 0x17,
      REG_INT_MAP_1                    = 0x18,
      REG_INT_MAP_2                    = 0x19,

      REG_INT_1A                       = 0x1a,
      REG_INT_1B                       = 0x1b,
      REG_INT_1C                       = 0x1c,

      // 0x1d reserved

      REG_INT_1E                       = 0x1e,

      // 0x1f-0x20 reserved

      REG_INT_RST_LATCH                = 0x21,

      REG_HIGH_TH_X                    = 0x22,
      REG_HIGH_DUR_X                   = 0x23,
      REG_HIGH_TH_Y                    = 0x24,
      REG_HIGH_DUR_Y                   = 0x25,
      REG_HIGH_TH_Z                    = 0x26,
      REG_HIGH_DUR_Z                   = 0x27,

      // 0x28-0x30 reserved

      REG_SOC                          = 0x31,
      REG_A_FOC                        = 0x32,

      REG_TRIM_NVM_CTRL                = 0x33,

      REG_SPI3_WDT                     = 0x34,

      // 0x35 reserved

      REG_OFC1                         = 0x36,
      REG_OFC2                         = 0x37,
      REG_OFC3                         = 0x38,
      REG_OFC4                         = 0x39,

      REG_TRIM_GP0                     = 0x3a,
      REG_TRIM_GP1                     = 0x3b,

      REG_BIST                         = 0x3c,

      REG_FIFO_CONFIG_0                = 0x3d,
      REG_FIFO_CONFIG_1                = 0x3e,

      REG_FIFO_DATA                    = 0x3f

    } BMG160_REGS_T;

    /**
     * REG_INT_STATUS_0 bits
     */
    typedef enum {
      _INT_STATUS_0_RESERVED_BITS      = 0xf0 | 0x08 | 0x01,

      INT_STATUS_0_HIGH_INT            = 0x02,
      INT_STATUS_0_ANY_INT             = 0x04
    } INT_STATUS_0_BITS_T;

    /**
     * REG_INT_STATUS_1 bits
     */
    typedef enum {
      _INT_STATUS_1_RESERVED_BITS      = 0x0f,

      INT_STATUS_1_FIFO_INT            = 0x10,
      INT_STATUS_1_FAST_OFFSET_INT     = 0x20,
      INT_STATUS_1_AUTO_OFFSET_INT     = 0x40,
      INT_STATUS_1_DATA_INT            = 0x80
    } INT_STATUS_1_BITS_T;

    /**
     * REG_INT_STATUS_2 bits
     */
    typedef enum {
      _INT_STATUS_2_RESERVED_BITS      = 0xf0,

      INT_STATUS_2_ANY_FIRST_X         = 0x01,
      INT_STATUS_2_ANY_FIRST_Y         = 0x02,
      INT_STATUS_2_ANY_FIRST_Z         = 0x04,
      INT_STATUS_2_ANY_SIGN            = 0x08
    } INT_STATUS_2_BITS_T;

    /**
     * REG_INT_STATUS_3 bits
     */
    typedef enum {
      _INT_STATUS_3_RESERVED_BITS      = 0xf0,

      INT_STATUS_3_HIGH_FIRST_X        = 0x01,
      INT_STATUS_3_HIGH_FIRST_Y        = 0x02,
      INT_STATUS_3_HIGH_FIRST_Z        = 0x04,
      INT_STATUS_3_HIGH_SIGN           = 0x08
    } INT_STATUS_3_BITS_T;

    /**
     * REG_FIFO_STATUS bits
     */
    typedef enum {
      FIFO_STATUS_FRAME_COUNTER0       = 0x01,
      FIFO_STATUS_FRAME_COUNTER1       = 0x02,
      FIFO_STATUS_FRAME_COUNTER2       = 0x04,
      FIFO_STATUS_FRAME_COUNTER3       = 0x08,
      FIFO_STATUS_FRAME_COUNTER4       = 0x10,
      FIFO_STATUS_FRAME_COUNTER5       = 0x20,
      FIFO_STATUS_FRAME_COUNTER6       = 0x40,
      _FIFO_STATUS_FRAME_COUNTER_MASK  = 127,
      _FIFO_STATUS_FRAME_COUNTER_SHIFT = 0,

      FIFO_STATUS_FIFO_OVERRUN         = 0x80
    } FIFO_STATUS_BITS_T;

    /**
     * REG_GYR_RANGE bits
     */
    typedef enum {
      _GYR_RANGE_RESERVED_BITS         = 0x20 | 0x10 | 0x08,

      GYR_RANGE0                       = 0x01,
      GYR_RANGE1                       = 0x02,
      GYR_RANGE2                       = 0x04,
      _GYR_RANGE_MASK                  = 7,
      _GYR_RANGE_SHIFT                 = 0,

      GYR_RANGE_FIXED0                 = 0x40, // bits need hardcoding to 0b10
      GYR_RANGE_FIXED1                 = 0x80, // for some odd reason...
      _GYR_RANGE_FIXED_MASK            = 3,
      _GYR_RANGE_FIXED_SHIFT           = 6,
      _GYR_RANGE_FIXED_VALUE           = 2    // 0b10
    } GYR_RANGE_BITS_T;

    /**
     * GYR_RANGE (gyroscope deg/s range) values
     */
    typedef enum {
      RANGE_2000                       = 0, // degrees/sec
      RANGE_1000                       = 1,
      RANGE_500                        = 2,
      RANGE_250                        = 3,
      RANGE_125                        = 4
    } RANGE_T;

    /**
     * REG_GYR_BW bits
     */
    typedef enum {
      _GYR_BW_RESERVED_BITS            = 0xf0,

      GYR_BW0                          = 0x01,
      GYR_BW1                          = 0x02,
      GYR_BW2                          = 0x04,
      GYR_BW3                          = 0x08,
      _GYR_BW_MASK                     = 15,
      _GYR_BW_SHIFT                    = 0
    } GYR_BW_BITS_T;

    /**
     * GYR_BW (gyroscope filter bandwidth) values
     */
    typedef enum {
      BW_2000_UNFILTERED               = 0, // ODR/Filter BW
      BW_2000_230                      = 1, // ODR 2000Hz, Filter BW 230Hz
      BW_1000_116                      = 2,
      BW_400_47                        = 3,
      BW_200_23                        = 4,
      BW_100_12                        = 5,
      BW_200_64                        = 6,
      BW_100_32                        = 7
    } BW_T;

    /**
     * REG_LPM1 bits
     */
    typedef enum {
      // 0x01 reserved
      _LPM1_RESERVED_MASK              = 0x40 | 0x10 | 0x01,

      LPM1_SLEEP_DUR0                  = 0x02, // sleep dur in low power mode
      LPM1_SLEEP_DUR1                  = 0x04,
      LPM1_SLEEP_DUR2                  = 0x08,
      _LPM1_SLEEP_MASK                 = 7,
      _LPM1_SLEEP_SHIFT                = 1,

      // These are separate bits, deep_suspend and suspend (and if all
      // 0, normal).  Since only specific combinations are allowed, we
      // will treat this as a 3 bit bitfield called POWER_MODE.
      LPM1_POWER_MODE0                 = 0x20, // deep_suspend
      LPM1_POWER_MODE1                 = 0x40, // must always be 0!
      LPM1_POWER_MODE2                 = 0x80, // suspend
      _LPM1_POWER_MODE_MASK            = 7,
      _LPM1_POWER_MODE_SHIFT           = 5
    } LPM1_BITS_T;

    /**
     * SLEEP_DUR values
     */
    typedef enum {
      SLEEP_DUR_2                      = 0, // 2ms
      SLEEP_DUR_4                      = 1,
      SLEEP_DUR_5                      = 2,
      SLEEP_DUR_8                      = 3,
      SLEEP_DUR_10                     = 4,
      SLEEP_DUR_15                     = 5,
      SLEEP_DUR_18                     = 6,
      SLEEP_DUR_20                     = 7
    } SLEEP_DUR_T;

    /**
     * POWER_MODE values
     */
    typedef enum {
      POWER_MODE_NORMAL                = 0,
      POWER_MODE_DEEP_SUSPEND          = 1,
      POWER_MODE_SUSPEND               = 4
    } POWER_MODE_T;

    /**
     * REG_LPM2 bits
     */
    typedef enum {
      _LPM2_RESERVED_BITS              = 0x08,

      LPM2_AUTOSLEEP_DUR0              = 0x01,
      LPM2_AUTOSLEEP_DUR1              = 0x02,
      LPM2_AUTOSLEEP_DUR2              = 0x04,
      _LPM2_AUTOSLEEP_DUR_MASK         = 7,
      _LPM2_AUTOSLEEP_DUR_SHIFT        = 0,

      LPM2_EXT_TRIG_SEL0               = 0x10,
      LPM2_EXT_TRIG_SEL1               = 0x20,
      _LPM2_EXT_TRIG_SEL_MASK          = 3,
      _LPM2_EXT_TRIG_SEL_SHIFT         = 4,

      LPM2_POWER_SAVE_MODE             = 0x40,
      LPM2_FAST_POWERUP                = 0x80
    } LPM2_BITS_T;


    /**
     * LPM2_AUTOSLEEP_DUR values
     */
    typedef enum {
      AUTOSLEEP_DUR_NONE               = 0,
      AUTOSLEEP_DUR_4MS                = 1,
      AUTOSLEEP_DUR_5MS                = 2,
      AUTOSLEEP_DUR_8MS                = 3,
      AUTOSLEEP_DUR_10MS               = 4,
      AUTOSLEEP_DUR_15MS               = 5,
      AUTOSLEEP_DUR_20MS               = 6,
      AUTOSLEEP_DUR_40MS               = 7
    } AUTOSLEEP_DUR_T;

    /**
     * LPM2_EXT_TRIG_SEL values
     */
    typedef enum {
      EXT_TRIG_SEL_NONE                = 0,
      EXT_TRIG_SEL_INT1                = 1,
      EXT_TRIG_SEL_INT2                = 2,
      EXT_TRIG_SEL_SDO                 = 3 // if SPI3 mode (unsupported)
    } EXT_TRIG_SEL_T;

    /**
     * REG_RATE_HBW bits
     */
    typedef enum {
      _RATE_HBW_RESERVED_BITS          = 0x0f | 0x10 | 0x20,

      RATE_HBW_SHADOW_DIS              = 0x40,
      RATE_HBW_DATA_HIGH_BW            = 0x80
    } RATE_HBW_BITS_T;

    /**
     * REG_INT_EN_0 bits
     */
    typedef enum {
      _INT_EN_0_RESERVED_BITS          = 0x20 | 0x10 | 0x08 | 0x02 | 0x01,

      INT_EN_0_AUTO_OFFSET_EN          = 0x04,

      INT_EN_0_FIFO_EN                 = 0x40,
      INT_EN_0_DATA_EN                 = 0x80
    } INT_EN_0_BITS_T;

    /**
     * REG_INT_EN_1 bits
     */
    typedef enum {
      _INT_EN_1_INT1_RESERVED_BITS     = 0xf0,

      INT_EN_1_INT1_LVL                = 0x01, // level or edge
      INT_EN_1_INT1_OD                 = 0x02, // push-pull or open drain
      INT_EN_1_INT2_LVL                = 0x04,
      INT_EN_1_INT2_OD                 = 0x08
    } INT_EN_1_BITS_T;

    /**
     * REG_INT_MAP_0 bits
     */
    typedef enum {
      _INT_MAP_0_RESERVED_BITS         = 0xf0 | 0x04 | 0x01,

      INT_MAP_0_INT1_ANY               = 0x02,
      INT_MAP_0_INT1_HIGH              = 0x08
    } INT_MAP_0_BITS_T;

    /**
     * REG_INT_MAP_1 bits
     */
    typedef enum {
      INT_MAP_1_INT1_DATA              = 0x01,
      INT_MAP_1_INT1_FAST_OFFSET       = 0x02,
      INT_MAP_1_INT1_FIFO              = 0x04,
      INT_MAP_1_INT1_AUTO_OFFSET       = 0x08,
      INT_MAP_1_INT2_AUTO_OFFSET       = 0x10,
      INT_MAP_1_INT2_FIFO              = 0x20,
      INT_MAP_1_INT2_FAST_OFFSET       = 0x40,
      INT_MAP_1_INT2_DATA              = 0x80
    } INT_MAP_1_BITS_T;

    /**
     * REG_INT_1A bits
     */
    typedef enum {
      _INT_1A_RESERVED_BITS           = 0xd5,

      INT_1A_ANY_UNFILT_DATA          = 0x02,
      INT_1A_HIGH_UNFILT_DATA         = 0x08,
      INT_1A_SLOW_OFFSET_UNFILT       = 0x20
    } INT_1A_BITS_T;

    /**
     * REG_INT_1B bits
     */
    typedef enum {
      INT_1B_ANY_TH0                  = 0x01,
      INT_1B_ANY_TH1                  = 0x02,
      INT_1B_ANY_TH2                  = 0x04,
      INT_1B_ANY_TH3                  = 0x08,
      INT_1B_ANY_TH4                  = 0x10,
      INT_1B_ANY_TH5                  = 0x20,
      INT_1B_ANY_TH6                  = 0x40,
      _INT_1B_ANY_TH_MASK             = 127,
      _INT_1B_ANY_TH_SHIFT            = 0,

      INT_1B_FAST_OFFSET_UNFILT       = 0x80
    } INT_1B_BITS_T;

    /**
     * REG_INT_1C bits
     */
    typedef enum {
      _INT_1C_RESERVED_BITS           = 0x08,

      INT_1C_ANY_EN_X                 = 0x01,
      INT_1C_ANY_EN_Y                 = 0x02,
      INT_1C_ANY_EN_Z                 = 0x04,

      INT_1C_ANY_DUR_SAMPLE0          = 0x10,
      INT_1C_ANY_DUR_SAMPLE1          = 0x20,
      INT_1C_ANY_DUR_SAMPLE_MASK      = 3,
      INT_1C_ANY_DUR_SAMPLE_SHIFT     = 4,

      INT_1C_AWAKE_DUR0               = 0x40,
      INT_1C_AWAKE_DUR1               = 0x80,
      INT_1C_AWAKE_DUR_MASK           = 3,
      INT_1C_AWAKE_DUR_SHIFT          = 6
    } INT_1C_BITS_T;

    /**
     * INT_1C_ANY_DUR_SAMPLE values
     */
    typedef enum {
      ANY_DUR_SAMPLE_4                 = 0, // samples
      ANY_DUR_SAMPLE_8                 = 1,
      ANY_DUR_SAMPLE_12                = 2,
      ANY_DUR_SAMPLE_16                = 3
    } ANY_DUR_SAMPLE_T;

    /**
     * INT_1C_AWAKE_DUR values
     */
    typedef enum {
      AWAKE_DUR_SAMPLE_8               = 0, // samples
      AWAKE_DUR_SAMPLE_16              = 1,
      AWAKE_DUR_SAMPLE_32              = 2,
      AWAKE_DUR_SAMPLE_64              = 3
    } AWAKE_DUR_SAMPLE_T;

    /**
     * REG_INT_1E bits
     */
    typedef enum {
      _INT_1E_RESERVED_BITS           = 0x7f,

      INT_1E_FIFO_WM_EN               = 0x80
    } INT_1E_BITS_T;

    /**
     * REG_INT_RST_LATCH bits
     */
    typedef enum {
      _INT_RST_LATCH_RESERVED_BITS     = 0x20,

      INT_RST_LATCH0                   = 0x01,
      INT_RST_LATCH1                   = 0x02,
      INT_RST_LATCH2                   = 0x04,
      INT_RST_LATCH3                   = 0x08,
      _INT_RST_LATCH_MASK              = 15,
      _INT_RST_LATCH_SHIFT             = 0,

      INT_RST_LATCH_STATUS_BIT         = 0x10,

      INT_RST_LATCH_OFFSET_RESET       = 0x40,
      INT_RST_LATCH_RESET_INT          = 0x80
    } INT_RST_LATCH_BITS_T;

    /**
     * RST_LATCH values
     */
    typedef enum {
      RST_LATCH_NON_LATCHED            = 0,
      RST_LATCH_TEMPORARY_250MS        = 1,
      RST_LATCH_TEMPORARY_500MS        = 2,
      RST_LATCH_TEMPORARY_1S           = 3,
      RST_LATCH_TEMPORARY_2S           = 4,
      RST_LATCH_TEMPORARY_4S           = 5,
      RST_LATCH_TEMPORARY_8S           = 6,
      RST_LATCH_LATCHED                = 7,

      // 8 == non latched

      RST_LATCH_TEMPORARY_250US        = 9,
      RST_LATCH_TEMPORARY_500US        = 10,
      RST_LATCH_TEMPORARY_1MS          = 11,
      RST_LATCH_TEMPORARY_12_5MS       = 12,
      RST_LATCH_TEMPORARY_25MS         = 13,
      RST_LATCH_TEMPORARY_50MS         = 14

      // 15 == latched
    } RST_LATCH_T;

    /**
     * REG_HIGH_TH_* (X, Y, and Z) register bits
     */
    typedef enum {
      HIGH_TH_EN                       = 0x01,

      HIGH_TH_TH0                      = 0x02,
      HIGH_TH_TH1                      = 0x04,
      HIGH_TH_TH2                      = 0x08,
      HIGH_TH_TH3                      = 0x10,
      HIGH_TH_TH4                      = 0x20,
      _HIGH_TH_TH_MASK                 = 31,
      _HIGH_TH_TH_SHIFT                = 1,

      HIGH_TH_HY0                      = 0x40,
      HIGH_TH_HY1                      = 0x80,
      _HIGH_TH_HY_MASK                 = 3,
      _HIGH_TH_HY_SHIFT                = 6
    } HIGH_TH_BITS_T;

    /**
     * REG_SOC bits
     */
    typedef enum {
      SOC_SLOW_OFFSET_EN_X             = 0x01,
      SOC_SLOW_OFFSET_EN_Y             = 0x02,
      SOC_SLOW_OFFSET_EN_Z             = 0x04,

      SOC_SLOW_OFFSET_DUR0             = 0x08,
      SOC_SLOW_OFFSET_DUR1             = 0x10,
      SOC_SLOW_OFFSET_DUR2             = 0x20,
      _SOC_SLOW_OFFSET_DUR_MASK        = 7,
      _SOC_SLOW_OFFSET_DUR_SHIFT       = 3,

      SOC_SLOW_OFFSET_TH0              = 0x40,
      SOC_SLOW_OFFSET_TH1              = 0x80,
      _SOC_SLOW_OFFSET_TH_MASK         = 3,
      _SOC_SLOW_OFFSET_TH_SHIFT        = 6
    } SOC_BITS_T;

    /**
     * SOC_SLOW_OFFSET_DUR values
     */
    typedef enum {
      SLOW_OFFSET_DUR_40MS             = 0, // 40ms
      SLOW_OFFSET_DUR_80MS             = 1,
      SLOW_OFFSET_DUR_160MS            = 2,
      SLOW_OFFSET_DUR_320MS            = 3,
      SLOW_OFFSET_DUR_640MS            = 4,
      SLOW_OFFSET_DUR_1280MS           = 5
    } SLOW_OFFSET_DUR_T;

    /**
     * SOC_SLOW_OFFSET_TH values
     */
    typedef enum {
      SLOW_OFFSET_TH_0_1               = 0, // 0.1 degree/s
      SLOW_OFFSET_TH_0_2               = 1,
      SLOW_OFFSET_TH_0_5               = 2,
      SLOW_OFFSET_TH_1                 = 3
    } SLOW_OFFSET_TH_T;

    /**
     * REG_A_FOC bits
     */
    typedef enum {
      A_FOC_FAST_OFFSET_EN_X           = 0x01,
      A_FOC_FAST_OFFSET_EN_Y           = 0x02,
      A_FOC_FAST_OFFSET_EN_Z           = 0x04,

      A_FOC_FAST_OFFSET_EN             = 0x08,

      A_FOC_FAST_OFFSET_WORDLENGTH0    = 0x10,
      A_FOC_FAST_OFFSET_WORDLENGTH1    = 0x20,
      _A_FOC_FAST_OFFSET_WORDLENGTH_MASK = 3,
      _A_FOC_FAST_OFFSET_WORDLENGTH_SHIFT = 4,

      A_FOC_AUTO_OFFSET_WORDLENGTH0    = 0x40,
      A_FOC_AUTO_OFFSET_WORDLENGTH1    = 0x80,
      _A_FOC_AUTO_OFFSET_WORDLENGTH_MASK = 3,
      _A_FOC_AUTO_OFFSET_WORDLENGTH_SHIFT = 6
    } A_FOC_BITS_T;

    /**
     * FAST_OFFSET_WORDLENGTH values
     */
    typedef enum {
      FAST_OFFSET_WORDLENGTH_32        = 0, // samples
      FAST_OFFSET_WORDLENGTH_64        = 1,
      FAST_OFFSET_WORDLENGTH_128       = 2,
      FAST_OFFSET_WORDLENGTH_256       = 3
    } FAST_OFFSET_WORDLENGTH_T;

    /**
     * AUTO_OFFSET_WORDLENGTH values
     */
    typedef enum {
      AUTO_OFFSET_WORDLENGTH_32        = 0, // samples
      AUTO_OFFSET_WORDLENGTH_64        = 1,
      AUTO_OFFSET_WORDLENGTH_128       = 2,
      AUTO_OFFSET_WORDLENGTH_256       = 3
    } AUTO_OFFSET_WORDLENGTH_T;

    /**
     * REG_TRIM_NVM_CTRL bits
     */
    typedef enum {
      TRIM_NVM_CTRL_NVM_PROG_MODE     = 0x01,
      TRIM_NVM_CTRL_NVM_PROG_TRIG     = 0x02,
      TRIM_NVM_CTRL_NVM_PROG_RDY      = 0x04,
      TRIM_NVM_CTRL_NVM_PROG_LOAD     = 0x08,

      TRIM_NVM_CTRL_NVM_REMAIN0       = 0x10,
      TRIM_NVM_CTRL_NVM_REMAIN1       = 0x20,
      TRIM_NVM_CTRL_NVM_REMAIN2       = 0x40,
      TRIM_NVM_CTRL_NVM_REMAIN3       = 0x80,
      _TRIM_NVM_CTRL_NVM_REMAIN_MASK  = 15,
      _TRIM_NVM_CTRL_NVM_REMAIN_SHIFT = 4
    } TRIM_NVM_CTRL_BITS_T;

    /**
     * REG_SPI3_WDT bits
     */
    typedef enum {
      _SPI3_WDT_RESERVED_BITS         = 0xf0 | 0x08,

      SPI3_WDT_SPI3                   = 0x01, // 3-wire SPI - NOT SUPPORTED

      SPI3_WDT_I2C_WDT_SEL            = 0x02,
      SPI3_WDT_I2C_WDT_EN             = 0x04

      // 0x08-0x80 reserved
    } SPI3_WDT_BITS_T;

    /**
     * REG_OFC1 bits, the missing x, y, and z llsb bits are in GP0
     */
    typedef enum {
      OFC1_OFFSET_Z0                  = 0x01, // Z lsb (3:1)
      OFC1_OFFSET_Z1                  = 0x02,
      OFC1_OFFSET_Z2                  = 0x04,
      _OFC1_OFFSET_Z_MASK             = 7,
      _OFC1_OFFSET_Z_SHIFT            = 0,

      OFC1_OFFSET_Y0                  = 0x08, // Y lsb (3:1)
      OFC1_OFFSET_Y1                  = 0x10,
      OFC1_OFFSET_Y2                  = 0x20,
      _OFC1_OFFSET_Y_MASK             = 7,
      _OFC1_OFFSET_Y_SHIFT            = 3,

      OFC1_OFFSET_X0                  = 0x08, // bits 3:2 of X lsb. geez
      OFC1_OFFSET_X1                  = 0x10,
      _OFC1_OFFSET_X_MASK             = 3,
      _OFC1_OFFSET_X_SHIFT            = 6
    } OFC1_OFFSET_BITS_T;

    /**
     * REG_GP0 bits
     */
    typedef enum {
      GP0_OFFSET_Z                    = 0x01, // Z llsb (bit 0)
      GP0_OFFSET_Y                    = 0x02, // Y llsb (bit 0)

      GP0_OFFSET_X0                   = 0x04, // X llsbs (bits 1:0)
      GP0_OFFSET_X1                   = 0x08,
      _GP0_OFFSET_X_MASK              = 3,
      _GP0_OFFSET_X_SHIFT             = 2,

      GP0_GP00                        = 0x10,
      GP0_GP01                        = 0x20,
      GP0_GP02                        = 0x40,
      GP0_GP03                        = 0x80,
      _GP0_GP0_MASK                   = 15,
      _GP0_GP0_SHIFT                  = 4
    } GP0_BITS_T;

    /**
     * REG_BIST bits
     */
    typedef enum {
      _BIST_RESERVED_BITS             = 0x80 | 0x40 | 0x20 | 0x08,

      BIST_TRIG_BIST                  = 0x01,
      BIST_BIST_RDY                   = 0x02,
      BIST_BIST_FAIL                  = 0x04,

      BIST_RATE_OK                    = 0x10
    } BIST_BITS_T;

    /**
     * REG_FIFO_CONFIG_0 bits
     */
    typedef enum {
      FIFO_CONFIG_0_WATER_MARK0       = 0x01,
      FIFO_CONFIG_0_WATER_MARK1       = 0x02,
      FIFO_CONFIG_0_WATER_MARK2       = 0x04,
      FIFO_CONFIG_0_WATER_MARK3       = 0x08,
      FIFO_CONFIG_0_WATER_MARK4       = 0x10,
      FIFO_CONFIG_0_WATER_MARK5       = 0x20,
      FIFO_CONFIG_0_WATER_MARK6       = 0x40,
      _FIFO_CONFIG_0_WATER_MARK_MASK  = 127,
      _FIFO_CONFIG_0_WATER_MARK_SHIFT = 0,

      FIFO_CONFIG_0_TAG               = 0x80
    } FIFO_CONFIG_0_BITS_T;

    /**
     * REG_FIFO_CONFIG_1 bits
     */
    typedef enum {
      _FIFO_CONFIG_1_RESERVED_BITS     = 0x20 | 0x10 |0x08 | 0x04,

      FIFO_CONFIG_1_FIFO_DATA_SEL0     = 0x01,
      FIFO_CONFIG_1_FIFO_DATA_SEL1     = 0x02,
      _FIFO_CONFIG_1_FIFO_DATA_SEL     = 3,
      _FIFO_CONFIG_1_FIFO_DATA_SHIFT   = 0,

      FIFO_CONFIG_1_FIFO_MODE0         = 0x40,
      FIFO_CONFIG_1_FIFO_MODE1         = 0x80,
      _FIFO_CONFIG_1_FIFO_MODE_MASK    = 3,
      _FIFO_CONFIG_1_FIFO_MODE_SHIFT   = 6
    } FIFO_CONFIG_1_BITS_T;

    /**
     * FIFO_DATA_SEL values
     */
    typedef enum {
      FIFO_DATA_SEL_XYZ               = 0,
      FIFO_DATA_SEL_X                 = 1,
      FIFO_DATA_SEL_Y                 = 2,
      FIFO_DATA_SEL_Z                 = 3
    } FIFO_DATA_SEL_T;

    /**
     * FIFO_MODE values
     */
    typedef enum {
      FIFO_MODE_BYPASS                = 0,
      FIFO_MODE_FIFO                  = 1,
      FIFO_MODE_STREAM                = 2
    } FIFO_MODE_T;

    // interrupt selection for installISR() and uninstallISR()
    typedef enum {
      INTERRUPT_INT1,
      INTERRUPT_INT2
    } INTERRUPT_PINS_T;


    /**
     * BMG160 constructor.
     *
     * This device can support both I2C and SPI. For SPI, set the addr
     * to -1, and specify a positive integer representing the Chip
     * Select (CS) pin for the cs argument.  If you are using a
     * hardware CS pin (like edison with arduino breakout), then you
     * can connect the proper pin to the hardware CS pin on your MCU
     * and supply -1 for cs.  The default operating mode is I2C.
     *
     * @param bus I2C or SPI bus to use.
     * @param addr The address for this device.  -1 for SPI.
     * @param cs The gpio pin to use for the SPI Chip Select.  -1 for
     * I2C or for SPI with a hardware controlled pin.
     * @param theChipID The chip ID to use for validation
     */
    BMG160(int bus=BMG160_I2C_BUS, int addr=BMG160_DEFAULT_ADDR,
           int cs=-1);

    /**
     * BMG160 Destructor.
     */
    ~BMG160();

    /**
     * Update the internal stored values from sensor data.
     */
    void update();

    /**
     * Return the chip ID.
     *
     * @return The chip ID (BMG160_CHIPID).
     */
    uint8_t getChipID();

    /**
     * Return gyroscope data in degrees per second.  update() must
     * have been called prior to calling this method.
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
     * Return gyroscope data in degrees per second in the form of a
     * floating point array.  The pointer returned by this function is
     * statically allocated and will be rewritten on each call.
     * update() must have been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getGyroscope();

    /**
     * Return the current measured temperature.  Note, this is not
     * ambient temperature.  update() must have been called prior to
     * calling this method.
     *
     * @param fahrenheit true to return data in Fahrenheit, false for
     * Celicus.  Celsius is the default.
     * @return The temperature in degrees Celsius or Fahrenheit.
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * Initialize the device and start operation.  This function is
     * called from the constructor so will not typically need to be
     * called by a user unless the device is reset.
     *
     * @param pwr One of the POWER_MODE_T values.  The default is
     * POWER_MODE_NORMAL.
     * @param range One of the RANGE_T values.  The default is
     * RANGE_250.
     * @param bw One of the filtering BW_T values.  The default is
     * BW_400_47.
     */
    void init(POWER_MODE_T pwr=POWER_MODE_NORMAL,
              RANGE_T range=RANGE_250, BW_T bw=BW_400_47);

    /**
     * Reset the device as if during a power on reset.  All configured
     * values are lost when this happens.  You should call init()
     * afterwards, or at least perform the same initialization init()
     * does before continuing.
     */
    void reset();

    /**
     * Set the gyroscope detection scaling range.  This device
     * supports 125, 250, 500, 100, and 2000 degree/s ranges.
     *
     * @param range One of the RANGE_T values.
     */
    void setRange(RANGE_T range);

    /**
     * Set the output filtering bandwidth of the device.
     *
     * @param bw One of the BW_T values.
     */
    void setBandwidth(BW_T bw);

    /**
     * Set the power mode of the device.  Care must be taken when
     * setting a low power or suspend mode.  See the datasheet for
     * details.  I ncertain power modes, register write must be
     * drastically slowed down. which we cannot support.
     *
     * @param power One of the POWER_MODE_T values.
     */
    void setPowerMode(POWER_MODE_T power);

    /**
     * Enable update() to read from the FIFO rather than the gyroscope
     * axis registers directly.  init() enables this mode by default.
     * An advantage to this mode that all axis data is sampled from
     * the same timeslice.  When reading directly from the gyroscope
     * output registers, it's possible for one axis to be updated
     * while another is being read, causing a temporal
     * inconsistancy..
     *
     * Using the FIFO removes this problem.
     *
     * @param useFIFO true to enable update() to read from the FIFO.
     * When false, update will read from the gyroscope output
     * registers directly.
     */
    void enableFIFO(bool useFIFO);

    /**
     * Set the FIFO watermark.  When the watermark is reached an
     * interrupt (if enabled) will be generated.
     *
     * @param wm The FIFO watermark to use.  The maximum value is 63.
     */
    void fifoSetWatermark(int wm);

    /**
     * Set the FIFO configuration.  init() uses the FIFO_MODE_BYPASS
     * mode with axes set to FIFO_DATA_SEL_XYZ by default.
     *
     * @param mode One of the FIFO_MODE_T values.
     * @param axes One of the FIFO_DATA_SEL_T values.
     */
    void fifoConfig(FIFO_MODE_T mode, FIFO_DATA_SEL_T axes);

    /**
     * Return the Interrupt Enables 0 register.  These resgisters
     * allow you to enable various interrupt conditions.  See the
     * datasheet for details.
     *
     * @return A bitmask of INT_EN_0_BITS_T bits.
     */
    uint8_t getInterruptEnable0();

    /**
     * Set the Interrupt Enables 0 register.  See the datasheet for
     * details.
     *
     * @param bits A bitmask of INT_EN_0_BITS_T bits.
     */
    void setInterruptEnable0(uint8_t bits);

    /**
     * Return the Interrupt Map 0 register.  These registers allow you
     * to map specific interrupts to the interrupt 1 or interrupt 2
     * pin.  See the datasheet for details.
     *
     * @return A bitmask of INT_MAP_0_BITS_T bits.
     */
    uint8_t getInterruptMap0();

    /**
     * Set the Interrupt Map 0 register.  These registers allow you
     * to map specific interrupts to the interrupt 1 or interrupt 2
     * pin.  See the datasheet for details.
     *
     * @param A bitmask of INT_MAP_0_BITS_T bits.
     */
    void setInterruptMap0(uint8_t bits);

    /**
     * Return the Interrupt Map 1 register.  See the datasheet for
     * details.
     *
     * @return A bitmask of INT_MAP_1_BITS_T bits.
     */
    uint8_t getInterruptMap1();

    /**
     * Set the Interrupt Map 1 register.  See the datasheet for
     * details.
     *
     * @param A bitmask of INT_MAP_1_BITS_T bits.
     */
    void setInterruptMap1(uint8_t bits);

    /**
     * Return the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @return A bitmask of INT_1A_BITS_T bits.
     */
    uint8_t getInterruptSrc();

    /**
     * Set the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @param bits A bitmask of INT_1A_BITS_T bits.
     */
    void setInterruptSrc(uint8_t bits);

    /**
     * Return the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @return A bitmask of INT_EN_1_BITS_T bits.
     */
    uint8_t getInterruptOutputControl();

    /**
     * Set the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @param bits A bitmask of INT_EN_1_BITS_T bits.
     */
    void setInterruptOutputControl(uint8_t bits);

    /**
     * Clear all latched interrupts.  See the datasheet for details.
     */
    void clearInterruptLatches();

    /**
     * Return the current interrupt latching behavior.  See the
     * datasheet for details.
     *
     * @return One of the RST_LATCH_T values.
     */
    RST_LATCH_T getInterruptLatchBehavior();

    /**
     * Set the current interrupt latching behavior.  See the datasheet
     * for details.
     *
     * @param latch One of the RST_LATCH_T values.
     */
    void setInterruptLatchBehavior(RST_LATCH_T latch);

    /**
     * Return the interrupt status 0 register.  These registers
     * indicate which interrupts have been triggered.  See the
     * datasheet for details.
     *
     * @return a bitmask of INT_STATUS_0_BITS_T bits.
     */
    uint8_t getInterruptStatus0();

    /**
     * Return the interrupt status 1 register.  See the datasheet for
     * details.
     *
     * @return a bitmask of INT_STATUS_1_BITS_T bits.
     */
    uint8_t getInterruptStatus1();

    /**
     * Return the interrupt status 2 register.  See the datasheet for
     * details.
     *
     * @return a bitmask of INT_STATUS_2_BITS_T bits.
     */
    uint8_t getInterruptStatus2();

    /**
     * Return the interrupt status 3 register.  See the datasheet for
     * details.
     *
     * @return a bitmask of INT_STATUS_3_BITS_T bits.
     */
    uint8_t getInterruptStatus3();

    /**
     * Enable shadowing of the gyroscope output registers.  When
     * enabled, a read of an axis LSB register automatically locks the
     * MSB register of that axis until it has been read.  This is
     * usually a good thing to have enabled.  init() enables this by
     * default.  If disabled, then it becomes possible for part of an
     * axis value to change while another part is being read, causing
     * inconsistent data.
     *
     * @param shadow true to enable axis register shadowing, false otherwise.
     */
    void enableRegisterShadowing(bool shadow);

    /**
     * Enable filtering of the gyroscope axis data.  init()
     * enables this by default.  If disabled, then gyroscope data
     * that is read will be raw and unfiltered (rated R).  See the
     * datasheet for details.
     *
     * @param filter true to enable filtering, false to disable.
     */
    void enableOutputFiltering(bool filter);

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
                    jobject runnable);
#else
    /**
     * install an interrupt handler.
     *
     * @param intr one of the INTERRUPT_PINS_T values specifying which
     * interrupt pin you are installing.
     * @param gpio gpio pin to use as interrupt pin
     * @param level the interrupt trigger level (one of mraa::Edge
     * values).  Make sure that you have configured the interrupt pin
     * properly for whatever level you choose.
     * @param isr the interrupt handler, accepting a void * argument
     * @param arg the argument to pass the the interrupt handler
     */
    void installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
                    void (*isr)(void *), void *arg);
#endif

   /**
     * uninstall a previously installed interrupt handler
     *
     * @param intr one of the INTERRUPT_PINS_T values specifying which
     * interrupt pin you are removing.
     */
    void uninstallISR(INTERRUPT_PINS_T intr);

    /**
     * Read a register.
     *
     * @param reg The register to read.
     * @return The value of the register.
     */
    uint8_t readReg(uint8_t reg);

    /**
     * Read contiguous registers into a buffer.
     *
     * @param buffer The buffer to store the results.
     * @param len The number of registers to read.
     * @return The number of bytes read.
     */
    int readRegs(uint8_t reg, uint8_t *buffer, int len);

    /**
     * Write to a register
     *
     * @param reg The register to write to.
     * @param val The value to write.
     */
    void writeReg(uint8_t reg, uint8_t val);

  protected:
    mraa::I2c *m_i2c;
    mraa::Spi *m_spi;

    // spi chip select
    mraa::Gpio *m_gpioCS;

    mraa::Gpio *m_gpioIntr1;
    mraa::Gpio *m_gpioIntr2;

    uint8_t m_addr;

    // SPI chip select
    void csOn();
    void csOff();

    // acc data
    float m_gyrX;
    float m_gyrY;
    float m_gyrZ;

    float m_gyrScale;

    float m_temperature;

  private:
    bool m_isSPI;
    // use the FIFO by default?
    bool m_useFIFO;

    // return a reference to a gpio pin pointer depending on intr
    mraa::Gpio*& getPin(INTERRUPT_PINS_T intr);

    // Adding a private function definition for java bindings
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
    void installISR(INTERRUPT_PINS_T intr, int gpio, mraa::Edge level,
                    void (*isr)(void *), void *arg);
#endif
  };
}
