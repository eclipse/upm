/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

/*
 * The following section lists the EEPROM Addresses
 */
/*
 * RSC EEPROM Catalog listing
 * This part in the EEPROM stores the ascii representation of the sensor chip
 * number
 */
#define RSC_CATALOG_LISTING_MSB                0
#define RSC_CATALOG_LISTING_LSB                15

/*
 * RSC EEPROM Serial Number addresses
 */
#define RSC_SERIAL_NO_YYYY_MSB                 16
#define RSC_SERIAL_NO_YYYY_LSB                 19
#define RSC_SERIAL_NO_DDD_MSB                  20
#define RSC_SERIAL_NO_DDD_LSB                  22
#define RSC_SERIAL_NO_XXXX_MSB                 23
#define RSC_SERIAL_NO_XXXX_LSB                 26

/*
 * This area in the EEPROM contains the sensor's pressure range in float
 */
#define RSC_PRESSURE_RANGE_LSB                 27
#define RSC_PRESSURE_RANGE_MSB                 30

/*
 * This part in the EEPROM contains the lower pressure limit readable
 * Value is stored float
 */
#define RSC_PRESSURE_MINIMUM_LSB               31
#define RSC_PRESSURE_MINIMUM_MSB               34

/*
 * Unit of measurement of pressure for this particular sensor
 * Ex: Pascal, Bar, PSI, inH2O
 */
#define RSC_PRESSURE_UNIT_MSB                  35
#define RSC_PRESSURE_UNIT_LSB                  39

/*
 * Ex: Differential, Gauge, absolute
 */
#define RSC_PRESSURE_REFERENCE                 40

/*
 * ADC Configuration math
 */
#define RSC_ADC_CONDIG_00                      61
#define RSC_ADC_CONDIG_01                      63
#define RSC_ADC_CONDIG_02                      65
#define RSC_ADC_CONDIG_03                      67

/*
 * Offset Coefficient matrix
 */
#define RSC_OFFSET_COEFFICIENT_0_LSB           130
#define RSC_OFFSET_COEFFICIENT_0_MSB           133

#define RSC_OFFSET_COEFFICIENT_1_LSB           134
#define RSC_OFFSET_COEFFICIENT_1_MSB           137

#define RSC_OFFSET_COEFFICIENT_2_LSB           138
#define RSC_OFFSET_COEFFICIENT_2_MSB           141

#define RSC_OFFSET_COEFFICIENT_3_LSB           142
#define RSC_OFFSET_COEFFICIENT_3_MSB           145

/*
 * Span Coefficient Matrix
 */
#define RSC_SPAN_COEFFICIENT_0_LSB             210
#define RSC_SPAN_COEFFICIENT_0_MSB             213

#define RSC_SPAN_COEFFICIENT_1_LSB             214
#define RSC_SPAN_COEFFICIENT_1_MSB             217

#define RSC_SPAN_COEFFICIENT_2_LSB             218
#define RSC_SPAN_COEFFICIENT_2_MSB             221

#define RSC_SPAN_COEFFICIENT_3_LSB             222
#define RSC_SPAN_COEFFICIENT_3_MSB             225

/*
 * Shape Coefficient Matrix
 */
#define RSC_SHAPE_COEFFICIENT_0_LSB            290
#define RSC_SHAPE_COEFFICIENT_0_MSB            293

#define RSC_SHAPE_COEFFICIENT_1_LSB            294
#define RSC_SHAPE_COEFFICIENT_1_MSB            297

#define RSC_SHAPE_COEFFICIENT_2_LSB            298
#define RSC_SHAPE_COEFFICIENT_2_MSB            301

#define RSC_SHAPE_COEFFICIENT_3_LSB            302
#define RSC_SHAPE_COEFFICIENT_3_MSB            305

/*
 * Checksum addresses
 */
#define RSC_CHECKSUM_LSB                       450
#define RSC_CHECKSUM_MSB                       451

/*
 * The following section contains a list of variations useful for EEPROM
 * calculations and reads.
 */
#define RSC_EEPROM_ADDRESS_SPACE_SIZE             512
#define RSC_EEPROM_STANDARD_ARGUMENT_LENGTH       2
#define RSC_READ_EEPROM_INSTRUCTION               0x03
#define RSC_EEPROM_ADDRESS_LOWER_BYTE_MASK        0xff
#define RSC_EEPROM_ADDRESS_9TH_BIT_MASK           0x100
#define RSC_SENSOR_NAME_LEN                       16
#define RSC_SENSOR_NUMBER_LEN                     12
#define RSC_PRESSURE_RANGE_LEN                    4
#define RSC_PRESSURE_MINIMUM_LEN                  4
#define RSC_PRESSURE_UNIT_LEN                     6
#define RSC_SENSOR_TYPE_LEN                       1
// total types of coefficients
#define RSC_COEFF_T_ROW_NO                        3
// total no of coefficients in each type
#define RSC_COEFF_T_COL_NO                        4
// this can be calculated by using the LSB address of the 0th coefficient
// and the MSB of the 3rd coefficient
#define RSC_COEFF_ADDRESS_SPACE_SIZE              16

/*
 * The following section lists ADC Commands/Registers
 */
#define RSC_ADC_RESET_COMMAND                         0x06
#define RSC_DATA_RATE_SHIFT                           5
#define RSC_DATA_RATE_MASK                            0xe0
#define RSC_ADC_REG_MASK                              0x0C
#define RSC_ADC_NUM_BYTES_MASK                        0x03
#define RSC_OPERATING_MODE_MASK                       0x18
#define RSC_OPERATING_MODE_SHIFT                      3
#define RSC_SET_BITS_MASK                             0x04
#define RSC_ADC_WREG                                  0x40
#define MSEC_PER_SEC                                  1000
#define RSC_CREATE_WREG_COMMAND(x,y,z) (x|((y<<2)&RSC_ADC_REG_MASK)|(z&RSC_ADC_NUM_BYTES_MASK))

/*
 * Read/Write Enum
 */
typedef enum {
    READ = 0,
    WRITE } RSC_ADC_TX;

/*
 * Enum for the datarates supported by the sensor
 * N/F - Normal/Fast - 256 KHz/512 KHz
 * DR - Data Rate
 * SPS - Samples per second
 */
typedef enum {
    N_DR_20_SPS = 0,
    N_DR_45_SPS,
    N_DR_90_SPS,
    N_DR_175_SPS,
    N_DR_330_SPS,
    N_DR_600_SPS,
    N_DR_1000_SPS,
    N_DR_NA,
    F_DR_40_SPS,
    F_DR_90_SPS,
    F_DR_180_SPS,
    F_DR_350_SPS,
    F_DR_660_SPS,
    F_DR_1200_SPS,
    F_DR_2000_SPS,
    F_DR_NA } RSC_DATA_RATE;

/*
 * Enum for modes supported by the RSC sensor
 */
typedef enum {
    NORMAL_MODE = 0,
    NA_MODE,
    FAST_MODE } RSC_MODE;

/*
 * Enum for the different pressure units supported by the rsc class of sensors
 */
typedef enum {
    PASCAL = 0,
    KPASCAL,
    MPASCAL,
    PSI,
    INH2O,
    BAR,
    mBAR } PRESSURE_U;

/*
 * Enum for types of pressure that the rsc sensor class can calculate
 */
typedef enum {
    DIFFERENTIAL = 0,
    ABSOLUTE,
    GAUGE } PRESSURE_T;

/*
 * Enum for types of coefficients available from the rsc sensors
 */
typedef enum {
    OFFSET = 0,
    SPAN,
    SHAPE } COEFFICIENT_T;

/*
 * Enum for pressure/temperature reading
 */
typedef enum {
    PRESSURE = 0,
    TEMPERATURE } READING_T;

/*
 * Enum to access EEPROM/ADC
 */
typedef enum {
    EEPROM = 0,
    ADC } ACCESS_T;
