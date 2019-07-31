/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
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

#ifdef __cplusplus
extern "C" {
#endif


#define LSM303D_DEFAULT_I2C_BUS 0
#define LSM303D_DEFAULT_I2C_ADDR 0x1e

// from the WHO_AM_I_* register
#define LSM303D_CHIPID 0x49


    // Due to the fact that this chip is currently obsolete, we only
    // support minimum functionality.  This register map is not
    // complete.  While all registers are specified, bitfields and
    // enumerants are only specified for certain registers of
    // interest.  Feel free to add what you need.

    // NOTE: Reserved registers must not be written into or permanent
    // damage to the device can result.  Reading from them may return
    // indeterminate values.  Registers containing reserved bitfields
    // must be written as 0.

    /**
     * LSM303D registers
     */
    typedef enum {

        // 0x00-0x04 reserved

        LSM303D_REG_TEMP_OUT_L                   = 0x05,
        LSM303D_REG_TEMP_OUT_H                   = 0x06,

        LSM303D_REG_STATUS_M                     = 0x07,

        LSM303D_REG_OUT_X_L_M                    = 0x08,
        LSM303D_REG_OUT_X_H_M                    = 0x09,
        LSM303D_REG_OUT_Y_L_M                    = 0x0a,
        LSM303D_REG_OUT_Y_H_M                    = 0x0b,
        LSM303D_REG_OUT_Z_L_M                    = 0x0c,
        LSM303D_REG_OUT_Z_H_M                    = 0x0d,

        // 0x0e reserved

        LSM303D_REG_WHO_AM_I                     = 0x0f,

        // 0x10-0x11 reserved

        LSM303D_REG_INT_CTRL_M                   = 0x12,
        LSM303D_REG_INT_SRC_M                    = 0x13,
        LSM303D_REG_INT_THS_L_M                  = 0x14,
        LSM303D_REG_INT_THS_H_M                  = 0x15,

        LSM303D_REG_OFFSET_X_L_M                 = 0x16,
        LSM303D_REG_OFFSET_X_H_M                 = 0x17,
        LSM303D_REG_OFFSET_Y_L_M                 = 0x18,
        LSM303D_REG_OFFSET_Y_H_M                 = 0x19,
        LSM303D_REG_OFFSET_Z_L_M                 = 0x1a,
        LSM303D_REG_OFFSET_Z_H_M                 = 0x1b,

        LSM303D_REG_REFERENCE_X                  = 0x1c,
        LSM303D_REG_REFERENCE_Y                  = 0x1d,
        LSM303D_REG_REFERENCE_Z                  = 0x1e,

        LSM303D_REG_CTRL0                        = 0x1f,
        LSM303D_REG_CTRL1                        = 0x20,
        LSM303D_REG_CTRL2                        = 0x21,
        LSM303D_REG_CTRL3                        = 0x22,
        LSM303D_REG_CTRL4                        = 0x23,
        LSM303D_REG_CTRL5                        = 0x24,
        LSM303D_REG_CTRL6                        = 0x25,
        LSM303D_REG_CTRL7                        = 0x26,

        LSM303D_REG_STATUS_A                     = 0x27,

        LSM303D_REG_OUT_X_L_A                    = 0x28,
        LSM303D_REG_OUT_X_H_A                    = 0x29,
        LSM303D_REG_OUT_Y_L_A                    = 0x2a,
        LSM303D_REG_OUT_Y_H_A                    = 0x2b,
        LSM303D_REG_OUT_Z_L_A                    = 0x2c,
        LSM303D_REG_OUT_Z_H_A                    = 0x2d,

        LSM303D_REG_FIFO_CTRL                    = 0x2e,
        LSM303D_REG_FIFO_SRC                     = 0x2f,

        LSM303D_REG_IG_CFG1                      = 0x30,
        LSM303D_REG_IG_SRC1                      = 0x31,
        LSM303D_REG_IG_THS1                      = 0x32,
        LSM303D_REG_IG_DUR1                      = 0x33,
        LSM303D_REG_IG_CFG2                      = 0x34,
        LSM303D_REG_IG_SRC2                      = 0x35,
        LSM303D_REG_IG_THS2                      = 0x36,
        LSM303D_REG_IG_DUR2                      = 0x37,

        LSM303D_REG_CLICK_CFG                    = 0x38,
        LSM303D_REG_CLICK_SRC                    = 0x39,
        LSM303D_REG_CLICK_THS                    = 0x3a,

        LSM303D_REG_TIME_LIMIT                   = 0x3b,
        LSM303D_REG_TIME_LATENCY                 = 0x3c,
        LSM303D_REG_TIME_WINDOW                  = 0x3d,

        LSM303D_REG_ACT_THS                      = 0x3e,
        LSM303D_REG_ACT_DUR                      = 0x3f,
    } LSM303D_REGS_T;

    // Accelerometer registers

    /**
     * CTRL1 bits
     */
    typedef enum {
        LSM303D_CTRL1_AXEN                       = 0x01, // axis enables
        LSM303D_CTRL1_AYEN                       = 0x02,
        LSM303D_CTRL1_AZEN                       = 0x04,

        LSM303D_CTRL1_BDU                        = 0x08,

        LSM303D_CTRL1_AODR0                      = 0x10,
        LSM303D_CTRL1_AODR1                      = 0x20,
        LSM303D_CTRL1_AODR2                      = 0x40,
        LSM303D_CTRL1_AODR3                      = 0x80,
        _LSM303D_CTRL1_AODR_MASK                 = 15,
        _LSM303D_CTRL1_AODR_SHIFT                = 4,
    } LSM303D_CTRL1_BITS_T;

    /**
     * CTRL1_AODR values (and power mode)
     */
    typedef enum {
        LSM303D_AODR_POWER_DOWN                 = 0,
        LSM303D_AODR_3_125HZ                    = 1, // 3.125Hz
        LSM303D_AODR_6_25HZ                     = 2,
        LSM303D_AODR_12_5HZ                     = 3,
        LSM303D_AODR_25HZ                       = 4,
        LSM303D_AODR_50HZ                       = 5,
        LSM303D_AODR_100HZ                      = 6,
        LSM303D_AODR_200HZ                      = 7,
        LSM303D_AODR_400HZ                      = 8,
        LSM303D_AODR_800HZ                      = 9,
        LSM303D_AODR_1600HZ                     = 10,
    } LSM303D_AODR_T;

    /**
     * CTRL2 bits
     */
    typedef enum {
        LSM303D_CTRL2_SIM                        = 0x01,
        LSM303D_CTRL2_AST                        = 0x02,

        // 0x04 reserved

        LSM303D_CTRL2_AFS0                       = 0x08, // full scale
        LSM303D_CTRL2_AFS1                       = 0x10,
        LSM303D_CTRL2_AFS2                       = 0x20,
        _LSM303D_CTRL2_AFS_MASK                  = 7,
        _LSM303D_CTRL2_AFS_SHIFT                 = 3,

        LSM303D_CTRL2_ABW0                       = 0x40,
        LSM303D_CTRL2_ABW1                       = 0x80,
        _LSM303D_CTRL2_ABW_MASK                  = 3,
        _LSM303D_CTRL2_ABW_SHIFT                 = 6,
    } LSM303D_CTRL2_BITS_T;

    /**
     * CTRL2_AFS values (full scale)
     */
    typedef enum {
        LSM303D_AFS_2G                          = 0, // 2G
        LSM303D_AFS_4G                          = 1,
        LSM303D_AFS_6G                          = 2,
        LSM303D_AFS_8G                          = 3,
        LSM303D_AFS_16G                         = 4,
    } LSM303D_AFS_T;

    /**
     * CTRL5 bits
     */
    typedef enum {
        LSM303D_CTRL5_LIR1                       = 0x01,
        LSM303D_CTRL5_LIR2                       = 0x02,

        LSM303D_CTRL5_MODR0                      = 0x04, // mag odr
        LSM303D_CTRL5_MODR1                      = 0x08,
        LSM303D_CTRL5_MODR2                      = 0x10,
        _LSM303D_CTRL5_MODR_MASK                 = 7,
        _LSM303D_CTRL5_MODR_SHIFT                = 2,

        LSM303D_CTRL5_M_RES0                     = 0x20, // resolution
        LSM303D_CTRL5_M_RES1                     = 0x40,
        _LSM303D_CTRL5_MRES_MASK                 = 3,
        _LSM303D_CTRL5_MRES_SHIFT                = 6,

        LSM303D_CTRL5_TEMP_EN                    = 0x80,
    } LSM303D_CTRL5_BITS_T;

    /**
     * CTRL5_MODR values (mag output data rate)
     */
    typedef enum {
        LSM303D_MODR_3_125HZ                     = 0, // 3.125Hz
        LSM303D_MODR_6_25HZ                      = 1,
        LSM303D_MODR_12_5HZ                      = 2,
        LSM303D_MODR_25HZ                        = 3,
        LSM303D_MODR_50HZ                        = 4,
        LSM303D_MODR_100HZ                       = 5,
    } LSM303D_MODR_T;

    /**
     * CTRL5_M_RES values (resolution)
     */
    typedef enum {
        LSM303D_M_RES_LOW                        = 0,
        LSM303D_M_RES_HIGH                       = 3,
    } LSM303D_M_RES_T;

    /**
     * CTRL6 bits
     */
    typedef enum {
        // 0x01-0x10 reserved

        LSM303D_CTRL6_MFS0                       = 0x20,
        LSM303D_CTRL6_MFS1                       = 0x40,
        _LSM303D_CTRL6_MFS_MASK                  = 3,
        _LSM303D_CTRL6_MFS_SHIFT                 = 5,

        // 0x80 reserved
    } LSM303D_CTRL6_BITS_T;

    /**
     * CTRL6_MFS values (mag full scale)
     */
    typedef enum {
        LSM303D_MFS_2                            = 0, // 2 Gauss
        LSM303D_MFS_4                            = 1,
        LSM303D_MFS_8                            = 2,
        LSM303D_MFS_12                           = 3,
    } LSM303D_MFS_T;

    /**
     * CTRL7 bits
     */
    typedef enum {
        LSM303D_CTRL7_MD0                        = 0x01,
        LSM303D_CTRL7_MD1                        = 0x02,
        _LSM303D_CTRL7_MD_MASK                   = 3,
        _LSM303D_CTRL7_MD_SHIFT                  = 0,

        LSM303D_CTRL7_MLP                        = 0x04,

        // 0x08 reserved

        LSM303D_CTRL7_T_ONLY                     = 0x10,
        LSM303D_CTRL7_AFDS                       = 0x20,

        LSM303D_CTRL7_AHPM0                      = 0x40,
        LSM303D_CTRL7_AHPM1                      = 0x80,
        _LSM303D_CTRL7_AHPM_MASK                 = 3,
        _LSM303D_CTRL7_AHPM_SHIFT                = 6,
    } LSM303D_CTRL7_BITS_T;

    /**
     * CTRL7_MD values (power mode)
     */
    typedef enum {
        LSM303D_MD_CONTINUOUS                    = 0,
        LSM303D_MD_SINGLE                        = 1,
        LSM303D_MD_POWER_DOWN                    = 3, // 2 is pwr down too
    } LSM303D_MD_T;

#ifdef __cplusplus
}
#endif
