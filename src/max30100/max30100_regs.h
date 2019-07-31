/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX30100_I2C_ADDRESS 0x57

/* Single IR/R sample */
typedef struct {
    /* Raw IR (pulse) read value */
    uint16_t IR;
    /* Raw R (O2) read value */
    uint16_t R;
} max30100_value;

/* Function pointer for returning 1 IR/R sample */
typedef void (*func_sample_ready_handler)(max30100_value sample, void* arg);

/* Sample state */
typedef enum {
    /* NOT sampling */
    MAX30100_SAMPLE_STATE_IDLE,
    /* Take one sample/currently taking one sample */
    MAX30100_SAMPLE_STATE_ONE_SHOT,
    /* Sample continuously/currently sampling continuously */
    MAX30100_SAMPLE_STATE_CONTINUOUS,
    /* Sample continuously using buffer/currently sampling continuously using buffer*/
    MAX30100_SAMPLE_STATE_CONTINUOUS_BUFFERED
} MAX30100_SAMPLE_STATE;

/* Pulse oximeter and heart-rate sensor I2C registers */
typedef enum {
    /* Interrupt status (RO) */
    MAX30100_REG_INTERRUPT_STATUS = 0x00,
    /* Interrupt enable */
    MAX30100_REG_INTERRUPT_ENABLE = 0x01,
    /* FIFO write pointer */
    MAX30100_REG_FIFO_WR_PTR = 0x02,
    /* FIFO overflow counter */
    MAX30100_REG_FIFO_OVF_COUNTER = 0x03,
    /* FIFO read pointer */
    MAX30100_REG_FIFO_RD_PTR = 0x04,
    /* FIFO data */
    MAX30100_REG_FIFO_DATA = 0x05,
    /* Mode configuration */
    MAX30100_REG_MODE_CONFIG = 0x06,
    /* SPO2 configuration */
    MAX30100_REG_SPO2_CONFIG = 0x07,
    /* LED configuration */
    MAX30100_REG_LED_CONFIG = 0x09,
    /* Temperature integer (2's compliment) */
    MAX30100_REG_TEMP_INTEGER = 0x16,
    /* Temperature fraction) */
    MAX30100_REG_TEMP_FRACTION = 0x17,
    /* Revision ID (RO)*/
    MAX30100_REG_REV_ID = 0xFE,
    /* Part ID */
    MAX30100_REG_PART_ID = 0xFF
} MAX30100_REG;

/* MAX30100_REG_INTERRUPT_STATUS register fields */
/* FIFO almost full, set to 1 when WR_PTR == RD_PTR - 1 */
#define MAX30100_A_FULL         (1 << 7)
/* Temperature date ready flag */
#define MAX30100_TEMP_RDY       (1 << 6)
/* Heartrate data ready flag */
#define MAX30100_HR_RDY         (1 << 5)
/* HR and O2 data ready flag */
#define MAX30100_SPO2_RDY       (1 << 4)
/* Power ready after brownout flag */
#define MAX30100_PWR_RDY        (1 << 0)

/* MAX30100_REG_INTERRUPT_ENABLE register fields */
/* Enable interrupt on FIFO almost full */
#define MAX30100_EN_A_FULL      (1 << 7)
/* Enable interrupt on temperature date ready */
#define MAX30100_EN_TEMP_RDY    (1 << 6)
/* Enable interrupt on HR data ready */
#define MAX30100_EN_HR_RDY      (1 << 5)
/* Enable interrupt on HR and O2 data ready */
#define MAX30100_EN_SPO2_RDY    (1 << 4)

/* MAX30100_REG_MODE_CONFIG register fields */
/* Enable power-save mode */
#define MAX30100_SHDN           (1 << 7)
/* Reset device */
#define MAX30100_RESET          (1 << 6)
/* Initiate temperature reading */
#define MAX30100_TEMP_EN        (1 << 3)
/* Device sample mode (HR, vs SpO2) */
typedef enum _MAX30100_MODE {
/* Turn off sampling */
    MAX30100_MODE_DISABLED = 0x00,
/* Enable heartrate ONLY sampling */
    MAX30100_MODE_HR_EN = 0x02,
/* Enable SpO2 sampling */
    MAX30100_MODE_SPO2_EN = 0x03
} MAX30100_MODE;

/* MAX30100_REG_SPO2_CONFIG register fields */
#define MAX30100_SPO2_HI_RES_EN (1 << 6)
typedef enum _MAX30100_SR {
    MAX30100_SR_50_HZ = 0x00,
    MAX30100_SR_100_HZ = 0x01,
    MAX30100_SR_167_HZ = 0x02,
    MAX30100_SR_200_HZ = 0x03,
    MAX30100_SR_400_HZ = 0x04,
    MAX30100_SR_600_HZ = 0x05,
    MAX30100_SR_900_HZ = 0x06,
    MAX30100_SR_1000_HZ = 0x07
} MAX30100_SR;
/* LED pulse width (microseconds) */
typedef enum _MAX30100_LED_PW {
    MAX30100_LED_PW_200_US_13_BITS = 0x00,
    MAX30100_LED_PW_400_US_14_BITS = 0x01,
    MAX30100_LED_PW_800_US_15_BITS = 0x02,
    MAX30100_LED_PW_1600_US_16_BITS = 0x03
} MAX30100_LED_PW;

/* MAX30100_REG_LED_CONFIG register fields */
/* LED (IR and R) current (milliamps) */
typedef enum _MAX30100_LED_CURRENT {
    MAX30100_LED_CURRENT_0_0_MA = 0x00,
    MAX30100_LED_CURRENT_4_4_MA = 0x01,
    MAX30100_LED_CURRENT_7_6_MA = 0x02,
    MAX30100_LED_CURRENT_11_0_MA = 0x03,
    MAX30100_LED_CURRENT_14_2_MA = 0x04,
    MAX30100_LED_CURRENT_17_4_MA = 0x05,
    MAX30100_LED_CURRENT_20_8_MA = 0x06,
    MAX30100_LED_CURRENT_24_0_MA = 0x07,
    MAX30100_LED_CURRENT_27_1_MA = 0x08,
    MAX30100_LED_CURRENT_30_6_MA = 0x09,
    MAX30100_LED_CURRENT_33_8_MA = 0x0a,
    MAX30100_LED_CURRENT_37_0_MA = 0x0b,
    MAX30100_LED_CURRENT_40_2_MA = 0x0c,
    MAX30100_LED_CURRENT_43_6_MA = 0x0d,
    MAX30100_LED_CURRENT_46_8_MA = 0x0e,
    MAX30100_LED_CURRENT_50_0_MA = 0x0f
} MAX30100_LED_CURRENT;

#ifdef __cplusplus
}
#endif
