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

// BMP280
#define BMP280_DEFAULT_I2C_BUS 0
#define BMP280_DEFAULT_SPI_BUS 0
#define BMP280_DEFAULT_ADDR 0x77
#define BMP280_CHIPID 0x58

// BME280
#define BME280_DEFAULT_I2C_BUS 0
#define BME280_DEFAULT_SPI_BUS 0
#define BME280_DEFAULT_ADDR 0x77
#define BME280_CHIPID 0x60

// default sea level pressure in hPA
#define BMP280_SEA_LEVEL_HPA  (1013.25)

// special reset byte, same for bme280
#define BMP280_RESET_BYTE 0xb6

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * BMP280 registers
     */
    typedef enum {
        // Do not write into reserved bits.

        // read-only factory calibration data
        BMP280_REG_CALIB00                      = 0x88,
        BMP280_REG_CALIB01                      = 0x89,
        BMP280_REG_CALIB02                      = 0x8a,
        BMP280_REG_CALIB03                      = 0x8b,
        BMP280_REG_CALIB04                      = 0x8c,
        BMP280_REG_CALIB05                      = 0x8d,
        BMP280_REG_CALIB06                      = 0x8e,
        BMP280_REG_CALIB07                      = 0x8f,
        BMP280_REG_CALIB08                      = 0x90,
        BMP280_REG_CALIB09                      = 0x91,
        BMP280_REG_CALIB10                      = 0x92,
        BMP280_REG_CALIB11                      = 0x93,
        BMP280_REG_CALIB12                      = 0x94,
        BMP280_REG_CALIB13                      = 0x95,
        BMP280_REG_CALIB14                      = 0x96,
        BMP280_REG_CALIB15                      = 0x97,
        BMP280_REG_CALIB16                      = 0x98,
        BMP280_REG_CALIB17                      = 0x99,
        BMP280_REG_CALIB18                      = 0x9a,
        BMP280_REG_CALIB19                      = 0x9b,
        BMP280_REG_CALIB20                      = 0x9c,
        BMP280_REG_CALIB21                      = 0x9d,
        BMP280_REG_CALIB22                      = 0x9e,
        BMP280_REG_CALIB23                      = 0x9f,
        BMP280_REG_CALIB24                      = 0xa0,
        BMP280_REG_CALIB25                      = 0xa1,

        BMP280_REG_CHIPID                       = 0xd0,
        BMP280_REG_RESET                        = 0xe0,
        BMP280_REG_STATUS                       = 0xf3,
        BMP280_REG_CTRL_MEAS                    = 0xf4,
        BMP280_REG_CONFIG                       = 0xf5,

        BMP280_REG_PRESSURE_MSB                 = 0xf7,
        BMP280_REG_PRESSURE_LSB                 = 0xf8,
        BMP280_REG_PRESSURE_XLSB                = 0xf9,

        BMP280_REG_TEMPERATURE_MSB              = 0xfa,
        BMP280_REG_TEMPERATURE_LSB              = 0xfb,
        BMP280_REG_TEMPERATURE_XLSB             = 0xfc
    } BMP280_REGS_T;

    /**
     * BMP280_REG_CONFIG bits
     */
    typedef enum {
        BMP280_CONFIG_SPI3W_EN                  = 0x01,

        // 0x02 reserved

        BMP280_CONFIG_FILTER0                   = 0x04,
        BMP280_CONFIG_FILTER1                   = 0x08,
        BMP280_CONFIG_FILTER2                   = 0x10,
        _BMP280_CONFIG_FILTER_MASK              = 7,
        _BMP280_CONFIG_FILTER_SHIFT             = 2,

        BMP280_CONFIG_T_SB0                     = 0x20,
        BMP280_CONFIG_T_SB1                     = 0x40,
        BMP280_CONFIG_T_SB2                     = 0x80,
        _BMP280_CONFIG_T_SB_MASK                = 7,
        _BMP280_CONFIG_T_SB_SHIFT               = 5
    } BMP280_CONFIG_BITS_T;

    /**
     * BMP280_FILTER values (samples to reach >= 75% of step response)
     */
    typedef enum {
        BMP280_FILTER_OFF                       = 0, // 1 samples
        BMP280_FILTER_2                         = 1, // 2
        BMP280_FILTER_4                         = 2, // 5
        BMP280_FILTER_8                         = 3, // 11
        BMP280_FILTER_16                        = 4  // 22
    } BMP280_FILTER_T;

    /**
     * BMP280_T_SB values (timer standby)
     */
    typedef enum {
        BMP280_T_SB_0_5                         = 0, // 0.5ms
        BMP280_T_SB_62_5                        = 1, // 62.5ms
        BMP280_T_SB_125                         = 2, // 125ms
        BMP280_T_SB_250                         = 3,
        BMP280_T_SB_500                         = 4,
        BMP280_T_SB_1000                        = 5,
        BMP280_T_SB_2000                        = 6, // bme280 - 10ms
        BMP280_T_SB_4000                        = 7  // bme280 - 20ms
    } BMP280_T_SB_T;


    /**
     * BMP280_REG_CTRL_MEAS bits
     */
    typedef enum {
        BMP280_CTRL_MEAS_MODE0                  = 0x01,
        BMP280_CTRL_MEAS_MODE1                  = 0x02,
        _BMP280_CTRL_MEAS_MODE_MASK             = 3,
        _BMP280_CTRL_MEAS_MODE_SHIFT            = 0,

        BMP280_CTRL_MEAS_OSRS_P0                = 0x04,
        BMP280_CTRL_MEAS_OSRS_P1                = 0x08,
        BMP280_CTRL_MEAS_OSRS_P2                = 0x10,
        _BMP280_CTRL_MEAS_OSRS_P_MASK           = 7,
        _BMP280_CTRL_MEAS_OSRS_P_SHIFT          = 2,

        BMP280_CTRL_MEAS_OSRS_T0                = 0x04,
        BMP280_CTRL_MEAS_OSRS_T1                = 0x08,
        BMP280_CTRL_MEAS_OSRS_T2                = 0x10,
        _BMP280_CTRL_MEAS_OSRS_T_MASK           = 7,
        _BMP280_CTRL_MEAS_OSRS_T_SHIFT          = 5
    } BMP280_CTRL_MEAS_T;

    /**
     * BMP280_CTRL_MEAS_MODE values
     */
    typedef enum {
        BMP280_MODE_SLEEP                       = 0,
        BMP280_MODE_FORCED                      = 1,
        // 2 is also FORCED mode
        BMP280_MODE_NORMAL                      = 3
    } BMP280_MODES_T;

    /**
     * BMP280_CTRL_MEAS_OSRS_P values
     */
    typedef enum {
        BMP280_OSRS_P_SKIPPED                   = 0,
        BMP280_OSRS_P_OVERSAMPLING_1            = 1, // x1
        BMP280_OSRS_P_OVERSAMPLING_2            = 2, // x2
        BMP280_OSRS_P_OVERSAMPLING_4            = 3,
        BMP280_OSRS_P_OVERSAMPLING_8            = 4,
        BMP280_OSRS_P_OVERSAMPLING_16           = 5
    } BMP280_OSRS_P_T;

    /**
     * BMP280_CTRL_MEAS_OSRS_T values
     */
    typedef enum {
        BMP280_OSRS_T_SKIPPED                   = 0,
        BMP280_OSRS_T_OVERSAMPLING_1            = 1, // x1
        BMP280_OSRS_T_OVERSAMPLING_2            = 2, // x2
        BMP280_OSRS_T_OVERSAMPLING_4            = 3,
        BMP280_OSRS_T_OVERSAMPLING_8            = 4,
        BMP280_OSRS_T_OVERSAMPLING_16           = 5
    } BMP280_OSRS_T_T;

    /**
     * BMP280_REG_STATUS bits
     */
    typedef enum {
        BMP280_STATUS_IM_UPDATE                 = 0x01,
        // 0x02-0x04 reserved
        BMP280_STATUS_MEASURING                 = 0x08
        // 0x10-0x80 reserved
    } BMP280_STATUS_T;

    /**
     * BMP280_USAGE_MODE values.  This is a fake specification to
     * configure the various knobs based on their typical use modes,
     * as recommended by Bosch.
     */
    typedef enum {
        BMP280_USAGE_MODE_HANDHELD_LOW_POWER    = 0,
        BMP280_USAGE_MODE_HANDHELD_DYNAMIC      = 1,
        BMP280_USAGE_MODE_WEATHER_MONITOR       = 2, // lowest power consumption
        BMP280_USAGE_MODE_FLOOR_CHG_DETECT      = 3,
        BMP280_USAGE_MODE_DROP_DETECT           = 4,
        BMP280_USAGE_MODE_INDOOR_NAV            = 5  // highest resolution
    } BMP280_USAGE_MODE_T;

    // The following are registers specific to the BME280.  The BME280
    // is identical to the BMP280 with the exception of humidity
    // sensor support and the corresponding registers needed to
    // support it.

    /**
     * BME280 registers
     */
    typedef enum {
        // Do not write into reserved bits.

        // We only specify those registers specific to the BME280.  The
        // rest of them can be found in the BMP280 header file.

        // read-only factory calibration data for humidity

        BME280_REG_CALIB_DIG_H1                 = 0xa1,

        BME280_REG_CALIB_DIG_H2_LSB             = 0xe1,
        BME280_REG_CALIB_DIG_H2_MSB             = 0xe2,

        BME280_REG_CALIB_DIG_H3                 = 0xe3,

        BME280_REG_CALIB_DIG_H4_0               = 0xe4, // bits 8 -> 11:4
        BME280_REG_CALIB_DIG_H4_1               = 0xe5, // bits 3:0 -> 3:0

        BME280_REG_CALIB_DIG_H5_0               = 0xe5, // bits 7:4 -> 3:0
        BME280_REG_CALIB_DIG_H5_1               = 0xe6, // bits 8 -> 11:4

        BME280_REG_CALIB_DIG_H6                 = 0xe7,

        BME280_REG_CTRL_HUM                     = 0xf2,

        BME280_REG_HUMIDITY_MSB                 = 0xfd,
        BME280_REG_HUMIDITY_LSB                 = 0xfe
    } BME280_REGS_T;

    /**
     * BME280_REG_CTRL_HUM bits
     */
    typedef enum {
        BME280_CTRL_HUM_OSRS_H0                 = 0x01,
        BME280_CTRL_HUM_OSRS_H1                 = 0x02,
        BME280_CTRL_HUM_OSRS_H2                 = 0x04,
        _BME280_CTRL_HUM_OSRS_H_MASK            = 3,
        _BME280_CTRL_HUM_OSRS_H_SHIFT           = 0

        // 0x08-0x80 reserved
    } BME280_CTRL_HUM_T;

    /**
     * BME280_CTRL_HUM_OSRS_H values
     */
    typedef enum {
        BME280_OSRS_H_SKIPPED                   = 0,
        BME280_OSRS_H_OVERSAMPLING_1            = 1, // x1
        BME280_OSRS_H_OVERSAMPLING_2            = 2, // x2
        BME280_OSRS_H_OVERSAMPLING_4            = 3,
        BME280_OSRS_H_OVERSAMPLING_8            = 4,
        BME280_OSRS_H_OVERSAMPLING_16           = 5
    } BME280_OSRS_H_T;

#ifdef __cplusplus
}
#endif
