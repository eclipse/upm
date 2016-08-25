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

#define BMA250E_I2C_BUS 0
#define BMA250E_SPI_BUS 0
#define BMA250E_DEFAULT_ADDR 0x18


namespace upm {

  /**
   * @library bmx050
   * @sensor bma250e
   * @comname BMA250E 10 bit Trixial Accelerometer
   * @type accelerometer
   * @man bosch
   * @con i2c spi gpio
   *
   * @brief API for the BMA250E 10 bit Trixial Accelerometer
   *
   * The BMA250E is a triaxial, low-g acceleration sensor with digital
   * output for consumer applications. It allows measurements of
   * acceleration in three perpendicular axes. An evaluation circuitry
   * (ASIC) converts the output of a micromechanical
   * acceleration-sensing structure (MEMS) that works according to the
   * differential capacitance principle.
   *
   * Not all functionality of this chip has been implemented in this
   * driver, however all the pieces are present to add any desired
   * functionality.  This driver supports both I2C (default) and SPI
   * operation.
   *
   * This driver attempts to support verious flavors of this chip,
   * such as the version on the BMX050, BMI050 (chipid 0xfa) and the
   * version on the bmc050 (chipid 0x03).  Not all functionality is
   * appropriate, or even present on all chips.  Consult the relevant
   * datasheets.
   *
   * This device requires 3.3v operation.
   *
   * @snippet bma250e.cxx Interesting
   */

  class BMA250E {
  public:
    // special reset byte
    static const uint8_t BMA250E_RESET_BYTE = 0xb6;

    // NOTE: Reserved registers must not be written into.  Reading
    // from them may return indeterminate values.  Registers
    // containing reserved bitfields must be written as 0.  Reading
    // reserved bitfields may return indeterminate values.

    /**
     * BMA250E registers
     */
    typedef enum : uint8_t {
      REG_CHIP_ID                      = 0x00,

      // 0x01 reserved

      REG_ACCD_X_LSB                   = 0x02,
      REG_ACCD_X_MSB                   = 0x03,
      REG_ACCD_Y_LSB                   = 0x04,
      REG_ACCD_Y_MSB                   = 0x05,
      REG_ACCD_Z_LSB                   = 0x06,
      REG_ACCD_Z_MSB                   = 0x07,

      REG_TEMP                         = 0x08,

      REG_INT_STATUS_0                 = 0x09,
      REG_INT_STATUS_1                 = 0x0a,
      REG_INT_STATUS_2                 = 0x0b,
      REG_INT_STATUS_3                 = 0x0c,

      // 0x0d reserved

      REG_FIFO_STATUS                  = 0x0e,

      REG_PMU_RANGE                    = 0x0f,
      REG_PMU_BW                       = 0x10,
      REG_PMU_LPW                      = 0x11,
      REG_PMU_LOW_POWER                = 0x12,

      REG_ACC_HBW                      = 0x13,

      REG_SOFTRESET                    = 0x14,

      // 0x15 reserved

      REG_INT_EN_0                     = 0x16,
      REG_INT_EN_1                     = 0x17,
      REG_INT_EN_2                     = 0x18,

      REG_INT_MAP_0                    = 0x19,
      REG_INT_MAP_1                    = 0x1a,
      REG_INT_MAP_2                    = 0x1b,

      // 0x1c-0x1d reserved

      REG_INT_SRC                      = 0x1e,

      // 0x1f reserved

      REG_INT_OUT_CTRL                 = 0x20,
      REG_INT_RST_LATCH                = 0x21,

      REG_INT_0                        = 0x22,
      REG_INT_1                        = 0x23,
      REG_INT_2                        = 0x24,
      REG_INT_3                        = 0x25,
      REG_INT_4                        = 0x26,
      REG_INT_5                        = 0x27,
      REG_INT_6                        = 0x28,
      REG_INT_7                        = 0x29,
      REG_INT_8                        = 0x2a,
      REG_INT_9                        = 0x2b,
      REG_INT_A                        = 0x2c,
      REG_INT_B                        = 0x2d,
      REG_INT_C                        = 0x2e,
      REG_INT_D                        = 0x2f,

      REG_FIFO_CONFIG_0                = 0x30,

      // 0x31 reserved

      REG_PMU_SELFTEST                 = 0x32,

      REG_TRIM_NVM_CTRL                = 0x33,

      REG_SPI3_WDT                     = 0x34,

      // 0x35 reserved

      REG_OFC_CTRL                     = 0x36,
      REG_OFC_SETTING                  = 0x37,

      REG_OFC_OFFSET_X                 = 0x38,
      REG_OFC_OFFSET_Y                 = 0x39,
      REG_OFC_OFFSET_Z                 = 0x3a,

      REG_TRIM_GP0                     = 0x3b,
      REG_TRIM_GP1                     = 0x3c,

      // 0x3d reserved

      REG_FIFO_CONFIG_1                = 0x3e,
      REG_FIFO_DATA                    = 0x3f

    } BMA250E_REGS_T;

    /**
     * REG_ACCD_*_LSB bits - handle X, Y, and Z LSB regs, for 10 bit
     * resolution
     */
    typedef enum {
      ACCD10_LSB_NEW_DATA              = 0x01, // data updated since last read

      // 0x02-0x20 reserved

      ACCD10_LSB0                      = 0x40, // lower 2 bits of LSB data
      ACCD10_LSB1                      = 0x80,
      _ACCD10_LSB_MASK                 = 3,
      _ACCD10_LSB_SHIFT                = 6
    } ACCD10_LSB_BITS_T;

    /**
     * REG_ACCD_*_LSB bits - handle X, Y, and Z LSB regs, for 12 bit
     * resolution
     */
    typedef enum {
      ACCD12_LSB_NEW_DATA              = 0x01, // data updated since last read

      // 0x02-0x08 reserved

      ACCD12_LSB0                      = 0x10, // lower 4 bits of LSB data
      ACCD12_LSB1                      = 0x20,
      ACCD12_LSB2                      = 0x40,
      ACCD12_LSB3                      = 0x80,
      _ACCD12_LSB_MASK                 = 15,
      _ACCD12_LSB_SHIFT                = 4
    } ACCD12_LSB_BITS_T;

    /**
     * REG_INT_STATUS_0 bits
     */
    typedef enum {
      INT_STATUS_0_LOW                 = 0x01,
      INT_STATUS_0_HIGH                = 0x02,
      INT_STATUS_0_SLOPE               = 0x04,
      INT_STATUS_0_SLO_NOT_MOT         = 0x08,
      INT_STATUS_0_D_TAP               = 0x10,
      INT_STATUS_0_S_TAP               = 0x20,
      INT_STATUS_0_ORIENT              = 0x40,
      INT_STATUS_0_FLAT                = 0x80
    } INT_STATUS_0_BITS_T;

    /**
     * REG_INT_STATUS_1 bits
     */
    typedef enum {
      _INT_STATUS_1_RESERVED_BITS      = 0x0f | 0x10,
      // 0x01-0x10 reserved
      INT_STATUS_1_FIFO_FULL           = 0x20,
      INT_STATUS_1_FIFO_WM             = 0x40,
      INT_STATUS_1_DATA                = 0x80 // data ready int
    } INT_STATUS_1_BITS_T;

    /**
     * REG_INT_STATUS_2 bits
     */
    typedef enum {
      INT_STATUS_2_SLOPE_FIRST_X       = 0x01,
      INT_STATUS_2_SLOPE_FIRST_Y       = 0x02,
      INT_STATUS_2_SLOPE_FIRST_Z       = 0x04,
      INT_STATUS_2_SLOPE_SIGN          = 0x08,
      INT_STATUS_2_TAP_FIRST_X         = 0x10,
      INT_STATUS_2_TAP_FIRST_Y         = 0x20,
      INT_STATUS_2_TAP_FIRST_Z         = 0x40,
      INT_STATUS_2_TAP_SIGN            = 0x80
    } INT_STATUS_2_BITS_T;

    /**
     * REG_INT_STATUS_3 bits
     */
    typedef enum {
      INT_STATUS_3_HIGH_FIRST_X        = 0x01,
      INT_STATUS_3_HIGH_FIRST_Y        = 0x02,
      INT_STATUS_3_HIGH_FIRST_Z        = 0x04,
      INT_STATUS_3_HIGH_SIGN           = 0x08,

      INT_STATUS_3_ORIENT0             = 0x10,
      INT_STATUS_3_ORIENT1             = 0x20,
      INT_STATUS_3_ORIENT2             = 0x40,
      _INT_STATUS_3_ORIENT_MASK        = 7,
      _INT_STATUS_3_ORIENT_SHIFT       = 4,

      INT_STATUS_3_FLAT                = 0x80
    } INT_STATUS_3_BITS_T;

    /**
     * INT_STATUS_3_ORIENT values
     */
    typedef enum {
      ORIENT_POTRAIT_UPRIGHT           = 0,
      ORIENT_POTRAIT_UPSIDE_DOWN       = 1,
      ORIENT_LANDSCAPE_LEFT            = 2,
      ORIENT_LANDSCAPE_RIGHT           = 3,
    } ORIENT_T;

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
     * REG_PMU_RANGE bits
     */
    typedef enum {
      PMU_RANGE0                       = 0x01,
      PMU_RANGE1                       = 0x02,
      PMU_RANGE2                       = 0x04,
      PMU_RANGE3                       = 0x08,
      _PMU_RANGE_MASK                  = 15,
      _PMU_RANGE_SHIFT                 = 0

      // 0x10-0x80 reserved
    } PMU_RANGE_BITS_T;

    /**
     * PMU_RANGE (accelerometer g-range) values
     */
    typedef enum {
      RANGE_2G                         = 3,
      RANGE_4G                         = 5,
      RANGE_8G                         = 8,
      RANGE_16G                        = 12
    } RANGE_T;

    /**
     * REG_PMU_BW bits
     */
    typedef enum {
      PMU_BW0                          = 0x01,
      PMU_BW1                          = 0x02,
      PMU_BW2                          = 0x04,
      PMU_BW3                          = 0x08,
      PMU_BW4                          = 0x10,
      _PMU_BW_MASK                     = 31,
      _PMU_BW_SHIFT                    = 0

      // 0x20-0x80 reserved
    } PMU_BW_BITS_T;

    /**
     * PMU_BW (accelerometer filter bandwidth) values
     */
    typedef enum {
      BW_7_81                          = 8, // 7.81 Hz
      BW_15_63                         = 9,
      BW_31_25                         = 10,
      BW_62_5                          = 11,
      BW_125                           = 12,
      BW_250                           = 13,
      BW_500                           = 14,
      BW_1000                          = 15
    } BW_T;

    /**
     * REG_PMU_LPW bits
     */
    typedef enum {
      // 0x01 reserved
      _PMU_LPW_RESERVED_MASK           = 0x01,

      PMU_LPW_SLEEP_DUR0               = 0x02, // sleep dur in low power mode
      PMU_LPW_SLEEP_DUR1               = 0x04,
      PMU_LPW_SLEEP_DUR2               = 0x08,
      PMU_LPW_SLEEP_DUR3               = 0x10,
      _PMU_LPW_SLEEP_MASK              = 15,
      _PMU_LPW_SLEEP_SHIFT             = 1,

      // These are separate bits, deep_suspend, lowpower_en and
      // suspend (and if all 0, normal).  Since only specific
      // combinations are allowed, we will treat this as a 3 bit
      // bitfield called POWER_MODE.
      PMU_LPW_POWER_MODE0              = 0x20, // deep_suspend
      PMU_LPW_POWER_MODE1              = 0x40, // lowpower_en
      PMU_LPW_POWER_MODE2              = 0x80, // suspend
      _PMU_LPW_POWER_MODE_MASK         = 7,
      _PMU_LPW_POWER_MODE_SHIFT        = 5
    } PMU_LPW_BITS_T;

    /**
     * SLEEP_DUR values
     */
    typedef enum {
      SLEEP_DUR_0_5                    = 0, // 0.5ms
      SLEEP_DUR_1                      = 6,
      SLEEP_DUR_2                      = 7,
      SLEEP_DUR_4                      = 8,
      SLEEP_DUR_6                      = 9,
      SLEEP_DUR_10                     = 10,
      SLEEP_DUR_25                     = 11,
      SLEEP_DUR_50                     = 12,
      SLEEP_DUR_100                    = 13,
      SLEEP_DUR_500                    = 14,
      SLEEP_DUR_1000                   = 15
    } SLEEP_DUR_T;

    /**
     * POWER_MODE values
     */
    typedef enum {
      POWER_MODE_NORMAL                = 0,
      POWER_MODE_DEEP_SUSPEND          = 1,
      POWER_MODE_LOW_POWER             = 2,
      POWER_MODE_SUSPEND               = 4
    } POWER_MODE_T;

    /**
     * REG_PMU_LOW_POWER bits
     */
    typedef enum {
      _LOW_POWER_RESERVED_BITS         = 0x0f | 0x10 | 0x80,

      // 0x01-0x10 reserved
      LOW_POWER_SLEEPTIMER_MODE        = 0x20,
      LOW_POWER_LOWPOWER_MODE          = 0x40  // LPM1 or LPM2 mode. see DS.
      // 0x80 reserved
    } LOW_POWER_BITS_T;

    /**
     * REG_ACC_HBW bits
     */
    typedef enum {
      _ACC_HBW_RESERVED_BITS           = 0x0f | 0x10 | 0x20,

      // 0x01-0x20 reserved
      ACC_HBW_SHADOW_DIS               = 0x40,
      ACC_HBW_DATA_HIGH_BW             = 0x80
    } ACC_HBW_BITS_T;

    /**
     * REG_INT_EN_0 bits
     */
    typedef enum {
      _INT_EN_0_RESERVED_BITS          = 0x08,

      INT_EN_0_SLOPE_EN_X              = 0x01,
      INT_EN_0_SLOPE_EN_Y              = 0x02,
      INT_EN_0_SLOPE_EN_Z              = 0x04,

      // 0x08 reserved

      INT_EN_0_D_TAP_EN                = 0x10,
      INT_EN_0_S_TAP_EN                = 0x20,
      INT_EN_0_ORIENT_EN               = 0x40,
      INT_EN_0_FLAT_EN                 = 0x80
    } INT_EN_0_BITS_T;

    /**
     * REG_INT_EN_1 bits
     */
    typedef enum {
      _INT_EN_1_RESERVED_BITS          = 0x80,

      INT_EN_1_HIGH_EN_X               = 0x01,
      INT_EN_1_HIGH_EN_Y               = 0x02,
      INT_EN_1_HIGH_EN_Z               = 0x04,
      INT_EN_1_LOW_EN                  = 0x08,
      INT_EN_1_DATA_EN                 = 0x10,
      INT_EN_1_INT_FFULL_EN            = 0x20, // fifo full
      INT_EN_1_INT_FWM_EN              = 0x40  // fifo watermark

      // 0x80 reserved
    } INT_EN_1_BITS_T;

    /**
     * REG_INT_EN_2 bits
     */
    typedef enum {
      _INT_EN_2_RESERVED_BITS          = 0xf0,

      INT_EN_2_SLO_NO_MOT_EN_X         = 0x01,
      INT_EN_2_SLO_NO_MOT_EN_Y         = 0x02,
      INT_EN_2_SLO_NO_MOT_EN_Z         = 0x04,
      INT_EN_2_SLO_NO_MOT_SEL          = 0x08

      // 0x10-0x80 reserved
    } INT_EN_2_BITS_T;

    /**
     * REG_INT_MAP_0 bits
     */
    typedef enum {
      INT_MAP_0_INT1_LOW               = 0x01,
      INT_MAP_0_INT1_HIGH              = 0x02,
      INT_MAP_0_INT1_SLOPE             = 0x04,
      INT_MAP_0_INT1_SLO_NO_MOT        = 0x08,
      INT_MAP_0_INT1_D_TAP             = 0x10,
      INT_MAP_0_INT1_S_TAP             = 0x20,
      INT_MAP_0_INT1_ORIENT            = 0x40,
      INT_MAP_0_INT1_FLAT              = 0x80
    } INT_MAP_0_BITS_T;

    /**
     * REG_INT_MAP_1 bits
     */
    typedef enum {
      _INT_MAP_1_INT1_RESERVED_BITS    = 0x08 | 0x10,

      INT_MAP_1_INT1_DATA              = 0x01,
      INT_MAP_1_INT1_FWM               = 0x02,
      INT_MAP_1_INT1_FFULL             = 0x04,

      // 0x08-0x10 reserved

      INT_MAP_1_INT2_FFULL             = 0x20,
      INT_MAP_1_INT2_FWM               = 0x40,
      INT_MAP_1_INT2_DATA              = 0x80
    } INT_MAP_1_BITS_T;

    /**
     * REG_INT_MAP_2 bits
     */
    typedef enum {
      INT_MAP_2_INT2_LOW               = 0x01,
      INT_MAP_2_INT2_HIGH              = 0x02,
      INT_MAP_2_INT2_SLOPE             = 0x04,
      INT_MAP_2_INT2_SLO_NO_MOT        = 0x08,
      INT_MAP_2_INT2_D_TAP             = 0x10,
      INT_MAP_2_INT2_S_TAP             = 0x20,
      INT_MAP_2_INT2_ORIENT            = 0x40,
      INT_MAP_2_INT2_FLAT              = 0x80
    } INT_MAP_2_BITS_T;

    /**
     * REG_INT_SRC bits
     */
    typedef enum {
      _INT_SRC_RESERVED_BITS           = 0x40 | 0x80,

      INT_SRC_LOW                      = 0x01,
      INT_SRC_HIGH                     = 0x02,
      INT_SRC_SLO_NO_MOT               = 0x04,
      INT_SRC_SLOPE                    = 0x08,
      INT_SRC_TAP                      = 0x10,
      INT_SRC_DATA                     = 0x20

      // 0x40-0x80 reserved
    } INT_SRC_BITS_T;

    /**
     * REG_INT_OUT_CTRL bits
     */
    typedef enum {
      _INT_OUT_CTRL_INT1_RESERVED_BITS = 0xf0,

      INT_OUT_CTRL_INT1_LVL            = 0x01, // level or edge
      INT_OUT_CTRL_INT1_OD             = 0x02, // push-pull or open drain
      INT_OUT_CTRL_INT2_LVL            = 0x04,
      INT_OUT_CTRL_INT2_OD             = 0x08

      // 0x10-0x80 reserved
    } INT_OUT_CTRL_BITS_T;

    /**
     * REG_INT_RST_LATCH bits
     */
    typedef enum {
      _INT_RST_LATCH_RESERVED_BITS     = 0x10 | 0x20 | 0x40,

      INT_RST_LATCH0                   = 0x01,
      INT_RST_LATCH1                   = 0x02,
      INT_RST_LATCH2                   = 0x04,
      INT_RST_LATCH3                   = 0x08,
      _INT_RST_LATCH_MASK              = 15,
      _INT_RST_LATCH_SHIFT             = 0,

      // 0x10-0x40 reserved

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
     * REG_INT_2 bits
     */
    typedef enum {
      INT_2_LOW_HY0                    = 0x01,
      INT_2_LOW_HY1                    = 0x02,
      _INT_2_LOW_HY_MASK               = 3,
      _INT_2_LOW_HY_SHIFT              = 0,

      INT_2_LOW_MODE                   = 0x04,

      // 0x08-0x20 reserved

      INT_2_HIGH_HY0                   = 0x40,
      INT_2_HIGH_HY1                   = 0x80,
      _INT_2_HIGH_HY_MASK              = 3,
      _INT_2_HIGH_HY_SHIFT             = 6
    } INT_2_BITS_T;

    /**
     * REG_INT_5 bits
     */
    typedef enum {
      INT_5_SLOPE_DUR0                 = 0x01,
      INT_5_SLOPE_DUR1                 = 0x02,
      _INT_5_SLOPE_DUR_MASK            = 3,
      _INT_5_SLOPE_DUR_SHIFT           = 0,

      INT_5_SLO_NO_MOT_DUR0            = 0x04,
      INT_5_SLO_NO_MOT_DUR1            = 0x08,
      INT_5_SLO_NO_MOT_DUR2            = 0x10,
      INT_5_SLO_NO_MOT_DUR3            = 0x20,
      INT_5_SLO_NO_MOT_DUR4            = 0x40,
      INT_5_SLO_NO_MOT_DUR5            = 0x80,
      _INT_5_SLO_NO_MOT_DUR_MASK       = 63,
      _INT_5_SLO_NO_MOT_DUR_SHIFT      = 2
    } INT_5_BITS_T;

    /**
     * REG_INT_8 bits
     */
    typedef enum {
      INT_8_TAP_DUR0                   = 0x01,
      INT_8_TAP_DUR1                   = 0x02,
      INT_8_TAP_DUR2                   = 0x04,
      _INT_8_TAP_DUR_MASK              = 7,
      _INT_8_TAP_DUR_SHIFT             = 0,

      // 0x08-0x20 reserved

      INT_8_TAP_SHOCK                  = 0x40,
      INT_8_TAP_QUIET                  = 0x80
    } INT_8_BITS_T;

    /**
     * REG_INT_9 bits
     */
    typedef enum {
      INT_9_TAP_TH0                    = 0x01,
      INT_9_TAP_TH1                    = 0x02,
      INT_9_TAP_TH2                    = 0x04,
      INT_9_TAP_TH3                    = 0x08,
      INT_9_TAP_TH4                    = 0x10,
      _INT_5_TAP_TH_MASK               = 31,
      _INT_5_TAP_TH_SHIFT              = 0,

      // 0x20 reserved

      INT_9_TAP_SAMP0                  = 0x40,
      INT_9_TAP_SAMP1                  = 0x80,
      INT_9_TAP_SAMP1_MASK             = 3,
      INT_9_TAP_SAMP1_SHIFT            = 6
    } INT_9_BITS_T;

    /**
     * REG_INT_A bits
     */
    typedef enum {
      INT_A_ORIENT_MODE0               = 0x01,
      INT_A_ORIENT_MODE1               = 0x02,
      _INT_A_ORIENT_MODE_MASK          = 3,
      _INT_A_ORIENT_MODE_SHIFT         = 0,

      INT_A_ORIENT_BLOCKING0           = 0x04,
      INT_A_ORIENT_BLOCKING1           = 0x08,
      _INT_A_ORIENT_BLOCKING_MASK      = 3,
      _INT_A_ORIENT_BLOCKING_SHIFT     = 2,

      INT_A_ORIENT_HYST0               = 0x10,
      INT_A_ORIENT_HYST1               = 0x20,
      INT_A_ORIENT_HYST2               = 0x40,
      _INT_A_ORIENT_HYST_MASK          = 7,
      _INT_A_ORIENT_HYST_SHIFT         = 4

      // 0x80 reserved
    } INT_A_BITS_T;

    /**
     * INT_A_ORIENT_MODE values
     */
    typedef enum {
      ORIENT_MODE_SYMETRICAL           = 0,
      ORIENT_MODE_HIGH_ASYMETRICAL     = 1,
      ORIENT_MODE_LOW_ASYMETRICAL      = 2
    } ORIENT_MODE_T;

    /**
     * INT_A_ORIENT_BLOCKING values
     */
    typedef enum {
      ORIENT_BLOCKING_NONE             = 0,
      ORIENT_BLOCKING_THETA_ACC_1_5G   = 1,
      ORIENT_BLOCKING_THETA_ACC_0_2G_1_5G = 2,
      ORIENT_BLOCKING_THETA_ACC_0_4G_1_5G = 3
    } ORIENT_BLOCKING_T;

    /**
     * REG_INT_B bits
     */
    typedef enum {
      INT_B_ORIENT_THETA0              = 0x01,
      INT_B_ORIENT_THETA1              = 0x02,
      INT_B_ORIENT_THETA2              = 0x04,
      INT_B_ORIENT_THETA3              = 0x08,
      INT_B_ORIENT_THETA4              = 0x10,
      INT_B_ORIENT_THETA5              = 0x20,
      _INT_B_ORIENT_THETA_MASK         = 63,
      _INT_B_ORIENT_THETA_SHIFT        = 0,

      INT_B_ORIENT_UD_EN               = 0x40
      // 0x80 reserved
    } INT_B_BITS_T;

    /**
     * REG_INT_C bits
     */
    typedef enum {
      INT_B_FLAT_THETA0               = 0x01,
      INT_B_FLAT_THETA1               = 0x02,
      INT_B_FLAT_THETA2               = 0x04,
      INT_B_FLAT_THETA3               = 0x08,
      INT_B_FLAT_THETA4               = 0x10,
      INT_B_FLAT_THETA5               = 0x20,
      _INT_B_FLAT_THETA_MASK          = 63,
      _INT_B_FLAT_THETA_SHIFT         = 0,

      // 0x40-0x80 reserved
    } INT_C_BITS_T;

    /**
     * REG_INT_D bits
     */
    typedef enum {
      INT_D_FLAT_HY0                  = 0x01,
      INT_D_FLAT_HY1                  = 0x02,
      INT_D_FLAT_HY2                  = 0x04,
      _INT_B_FLAT_HY_MASK             = 7,
      _INT_B_FLAT_HY_SHIFT            = 0,

      // 0x08 reserved

      INT_D_FLAT_HOLD_TIME0           = 0x10,
      INT_D_FLAT_HOLD_TIME1           = 0x20,
      _INT_B_FLAT_HOLD_TIME_MASK      = 3,
      _INT_B_FLAT_HOLD_TIME_SHIFT     = 4

      // 0x40-0x80 reserved
    } INT_D_BITS_T;

    /**
     * REG_FIFO_CONFIG_0 bits
     */
    typedef enum {
      _FIFO_CONFIG_0_RESERVED_BITS    = 0x80 | 0x40,

      FIFO_CONFIG_0_WATER_MARK0       = 0x01,
      FIFO_CONFIG_0_WATER_MARK1       = 0x02,
      FIFO_CONFIG_0_WATER_MARK2       = 0x04,
      FIFO_CONFIG_0_WATER_MARK3       = 0x08,
      FIFO_CONFIG_0_WATER_MARK4       = 0x10,
      FIFO_CONFIG_0_WATER_MARK5       = 0x20,
      _FIFO_CONFIG_0_WATER_MARK_MASK  = 63,
      _FIFO_CONFIG_0_WATER_MARK_SHIFT = 0
    } FIFO_CONFIG_0_BITS_T;

    /**
     * REG_PMU_SELFTTEST bits
     */
    typedef enum {
      PMU_SELFTTEST_AXIS0             = 0x01,
      PMU_SELFTTEST_AXIS1             = 0x02,
      _PMU_SELFTTEST_AXIS_MASK        = 3,
      _PMU_SELFTTEST_AXIS_SHIFT       = 0,

      PMU_SELFTTEST_SIGN              = 0x04,

      // 0x08 reserved

      PMU_SELFTTEST_AMP               = 0x10,

      // 0x20-0x80 reserved
    } PMU_SELFTTEST_BITS_T;

    /**
     * PMU_SELFTTEST_AXIS values
     */
    typedef enum {
      SELFTTEST_AXIS_NONE             = 0,
      SELFTTEST_AXIS_X                = 1,
      SELFTTEST_AXIS_Y                = 2,
      SELFTTEST_AXIS_Z                = 3,
    } SELFTTEST_AXIS_T;

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
     * REG_OFC_CTRL bits
     */
    typedef enum {
      OFC_CTRL_HP_X_EN                = 0x01,
      OFC_CTRL_HP_Y_EN                = 0x02,
      OFC_CTRL_HP_Z_EN                = 0x04,

      // 0x08 reserved

      OFC_CTRL_CAL_RDY                = 0x10,

      OFC_CTRL_CAL_TRIGGER0           = 0x20,
      OFC_CTRL_CAL_TRIGGER1           = 0x40,
      _OFC_CTRL_CAL_TRIGGER_MASK      = 3,
      _OFC_CTRL_CAL_TRIGGER_SHIFT     = 5,

      OFC_CTRL_OFFSET_RESET           = 0x80

    } OFC_CTRL_BITS_T;

    /**
     * OFC_CTRL_CAL_TRIGGER values
     */
    typedef enum {
      CAL_TRIGGER_NONE                = 0,
      CAL_TRIGGER_X                   = 1,
      CAL_TRIGGER_Y                   = 2,
      CAL_TRIGGER_Z                   = 3
    } CAL_TRIGGER_T;

    /**
     * REG_OFC_SETTING bits
     */
    typedef enum {
      OFC_SETTING_CUT_OFF             = 0x01,

      OFC_SETTING_OFFSET_TARGET_X0    = 0x02,
      OFC_SETTING_OFFSET_TARGET_X1    = 0x04,
      _OFC_SETTING_OFFSET_TARGET_X_MASK = 3,
      _OFC_SETTING_OFFSET_TARGET_X_SHIFT = 1,

      OFC_SETTING_OFFSET_TARGET_Y0    = 0x08,
      OFC_SETTING_OFFSET_TARGET_Y1    = 0x10,
      _OFC_SETTING_OFFSET_TARGET_Y_MASK = 3,
      _OFC_SETTING_OFFSET_TARGET_Y_SHIFT = 3,

      OFC_SETTING_OFFSET_TARGET_Z0    = 0x20,
      OFC_SETTING_OFFSET_TARGET_Z1    = 0x40,
      _OFC_SETTING_OFFSET_TARGET_Z_MASK = 3,
      _OFC_SETTING_OFFSET_TARGET_Z_SHIFT = 5

      // 0x80 reserved
    } OFC_SETTING_BITS_T;

    /**
     * OFC_SETTING_OFFSET_TARGET (for X, Y and Z axis) values
     */
    typedef enum {
      OFFSET_TARGET_0G                = 0,
      OFFSET_TARGET_PLUS_1G           = 1,
      OFFSET_TARGET_MINUS_1G          = 2,
      // 3 == 0G
    } OFFSET_TARGET_T;

    /**
     * REG_FIFO_CONFIG_1 bits
     */
    typedef enum {
      FIFO_CONFIG_1_FIFO_DATA_SEL0     = 0x01,
      FIFO_CONFIG_1_FIFO_DATA_SEL1     = 0x02,
      _FIFO_CONFIG_1_FIFO_DATA_SEL     = 3,
      _FIFO_CONFIG_1_FIFO_DATA_SHIFT   = 0,

      // 0x04-0x20 reserved

      FIFO_CONFIG_1_FIFO_MODE0         = 0x40,
      FIFO_CONFIG_1_FIFO_MODE1         = 0x80,
      _FIFO_CONFIG_1_FIFO_MODE_MASK    = 3,
      _FIFO_CONFIG_1_FIFO_MODE_SHIFT   = 5
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

      // 3 == reserved (execute self-destruct :)
    } FIFO_MODE_T;

    // interrupt selection for installISR() and uninstallISR()
    typedef enum {
      INTERRUPT_INT1,
      INTERRUPT_INT2
    } INTERRUPT_PINS_T;

    // Different variants of this chip support different resolutions.
    // The 0xf9 variant supports 10b, while the 0xfa variant (bmx050)
    // supports 12 bits.
    typedef enum {
      RESOLUTION_10BITS,
      RESOLUTION_12BITS
    } RESOLUTION_T;


    /**
     * BMA250E constructor.
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
     */
    BMA250E(int bus=BMA250E_I2C_BUS, int addr=BMA250E_DEFAULT_ADDR,
            int cs=-1);

    /**
     * BMA250E Destructor.
     */
    ~BMA250E();

    /**
     * Update the internal stored values from sensor data.
     */
    void update();

    /**
     * Return the chip ID.
     *
     * @return The chip ID (BMA250E_CHIPID).
     */
    uint8_t getChipID();

    /**
     * Return accelerometer data in gravities.  update() must have
     * been called prior to calling this method.
     *
     * @param x Pointer to a floating point value that will have the
     * current x component placed into it.
     * @param y Pointer to a floating point value that will have the
     * current y component placed into it.
     * @param z Pointer to a floating point value that will have the
     * current z component placed into it.
     */
    void getAccelerometer(float *x, float *y, float *z);

    /**
     * Return accelerometer data in gravities in the form of a
     * floating point array.  The pointer returned by this function is
     * statically allocated and will be rewritten on each call.
     * update() must have been called prior to calling this method.
     *
     * @return A floating point array containing x, y, and z in
     * that order.
     */
    float *getAccelerometer();

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
     * RANGE_2G.
     * @param bw One of the filtering BW_T values.  The default is
     * BW_250.
     */
    void init(POWER_MODE_T pwr=POWER_MODE_NORMAL,
              RANGE_T range=RANGE_2G, BW_T bw=BW_250);

    /**
     * Reset the device as if during a power on reset.  All configured
     * values are lost when this happens.  You should call init()
     * afterwards, or at least perform the same initialization init()
     * does before continuing.
     */
    void reset();

    /**
     * Set the acceleration scaling range.  This device supports 2, 4,
     * 8, and 16g ranges.
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
     * setting a low power or suspend mode.  By default init() calls
     * setLowPowerMode2() to ensure that if any of these modes are
     * entered we can still talk to the device.  The default low power
     * mode is LPM1, which requires slowing down register writes,
     * which we cannot support.  setLowPowerMode2() enables LPM2 which
     * keeps the digital interface operational in low power or suspend
     * modes.  See the datasheet for details.
     *
     * So if you reset your device and don't call init() or
     * setLowPowerMode2(), you could lose control of the device by
     * calling this function with anything other than
     * POWER_MODE_NORMAL.  You've been warned :)
     *
     * @param power One of the POWER_MODE_T values.
     */
    void setPowerMode(POWER_MODE_T power);

    /**
     * Enable update() to read from the FIFO rather than the
     * acceleration axis registers directly.  init() enables this mode
     * by default if the chip variant supports a FIFO.  An advantage
     * to this mode that all axis data is sampled from the same
     * timeslice.  When reading directly from the acceleration output
     * registers, it's possible for one axis to be updated while
     * another is being read, causing a temporal anomaly that even
     * Captain Picard can't resolve.  If there is no FIFO present,
     * this call is ignored.
     *
     * Using the FIFO removes this problem.
     *
     * @param useFIFO true to enable update() to read from the FIFO.
     * When false, update will read from the acceleration output
     * registers directly.
     */
    void enableFIFO(bool useFIFO);

    /**
     * Set the FIFO watermark.  When the watermark is reached an
     * interrupt (if enabled) will be generated.  If there is no FIFO
     * present, this call is ignored.
     *
     * @param wm The FIFO watermark to use.  The maximum value is 63.
     */
    void fifoSetWatermark(int wm);

    /**
     * Set the FIFO configuration.  init() uses the FIFO_MODE_BYPASS
     * mode with axes set to FIFO_DATA_SEL_XYZ by default.  If there
     * is no FIFO present, this call is ignored.
     *
     * @param mode One of the FIFO_MODE_T values.
     * @param axes One of the FIFO_DATA_SEL_T values.
     */
    void fifoConfig(FIFO_MODE_T mode, FIFO_DATA_SEL_T axes);

    /**
     * Enable, disable, and configure the built in self test on a per
     * axis basis.  See the datasheet for details.
     *
     * @param sign true for a positive deflection, false for negative
     * @param amp true for a high deflection, false for a low deflection
     * @param axis One of the SELFTTEST_AXIS_T values.  Note, only one
     * axis at a time can be tested.  Accelerometer output for other
     * axes should be ignored.
     */
    void setSelfTest(bool sign, bool amp, SELFTTEST_AXIS_T axis);

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
     * Return the Interrupt Enables 1 register.  See the datasheet for
     * details.
     *
     * @return A bitmask of INT_EN_1_BITS_T bits.
     */
    uint8_t getInterruptEnable1();

    /**
     * Set the Interrupt Enables 1 register.  See the datasheet for
     * details.
     *
     * @param bits A bitmask of INT_EN_1_BITS_T bits.
     */
    void setInterruptEnable1(uint8_t bits);

    /**
     * Return the Interrupt Enables 2 register.  See the datasheet for
     * details.
     *
     * @return A bitmask of INT_EN_2_BITS_T bits.
     */
    uint8_t getInterruptEnable2();

    /**
     * Set the Interrupt Enables 2 register.  See the datasheet for
     * details.
     *
     * @param bits A bitmask of INT_EN_2_BITS_T bits.
     */
    void setInterruptEnable2(uint8_t bits);

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
     * Return the Interrupt Map 2 register.  See the datasheet for
     * details.
     *
     * @return A bitmask of INT_MAP_2_BITS_T bits.
     */
    uint8_t getInterruptMap2();

    /**
     * Set the Interrupt Map 2 register.  See the datasheet for
     * details.
     *
     * @param A bitmask of INT_MAP_2_BITS_T bits.
     */
    void setInterruptMap2(uint8_t bits);

    /**
     * Return the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @return A bitmask of INT_SRC_BITS_T bits.
     */
    uint8_t getInterruptSrc();

    /**
     * Set the Interrupt source register.  This register allows
     * determining where data comes from (filtered/unfiltered) for
     * those interrupt sources where this is selectable.  See the
     * datasheet for details.
     *
     * @param bits A bitmask of INT_SRC_BITS_T bits.
     */
    void setInterruptSrc(uint8_t bits);

    /**
     * Return the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @return A bitmask of INT_OUT_CTRL_BITS_T bits.
     */
    uint8_t getInterruptOutputControl();

    /**
     * Set the Interrupt output control register.  This register
     * allows determining the electrical characteristics of the 2
     * interrupt pins (open-drain/push-pull and level/edge
     * triggering).  See the datasheet for details.
     *
     * @param bits A bitmask of INT_OUT_CTRL_BITS_T bits.
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
     * Return the interrupt status 3 register bitfields.  See the
     * datasheet for details.  The Orientation value is not returned by
     * this function, see getInterruptStatus3Orientation() for that
     * information.
     *
     * @return a bitmask of INT_STATUS_3_BITS_T bits ONLY.
     */
    uint8_t getInterruptStatus3Bits();

    /**
     * Return the interrupt status 3 register Orientation value.  See the
     * datasheet for details.
     *
     * @return one of the ORIENT_T values.
     */
    ORIENT_T getInterruptStatus3Orientation();

    /**
     * Enable shadowing of the accelerometer output registers.  When
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
     * Enable filtering of the accelerometer axis data.  init()
     * enables this by default.  If disabled, then accelerometer data
     * that is read will be raw and unfiltered (rated R).  See the
     * datasheet for details.
     *
     * @param filter true to enable filtering, false to disable.
     */
    void enableOutputFiltering(bool filter);

    /**
     * Make sure low power mode config (LPM2) is set in case we later
     * go into the low power or suspend power modes.  LPM1 mode (the
     * default) requires drastically slowed register writes which we
     * cannot handle.
     */
    void setLowPowerMode2();


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
    RESOLUTION_T m_resolution;

    // does this chip support the fifo?
    bool m_fifoAvailable;

    // SPI chip select
    void csOn();
    void csOff();

    // acc data
    float m_accX;
    float m_accY;
    float m_accZ;

    float m_accScale;

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
