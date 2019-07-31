/*
 * Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * based on: RIOT-driver tcs37727 by Johann Fischer <j.fischer@phytec.de>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <mraa/i2c.hpp>
#include <stdint.h>
#include <stdbool.h>

#define TCS37727_NAME "TCS37727"
#define TCS37727_I2C_ADDRESS   0x29
#define TCS37727_DEVICE_ID     0x49
#define TCS37727_DEVICE_ID_REG 0x92

#define TCS37727_ATIME_DEFAULT     200000  /* Default RGBC integration time */
#define TCS37727_AG_THRESHOLD_LOW  200
#define TCS37727_AG_THRESHOLD_HIGH (65535 - TCS37727_AG_THRESHOLD_LOW)


/* TCS37727 Register Map */
#define TCS37727_ENABLE  0x80 /* Enables states and interrupts */
#define TCS37727_ATIME   0x81 /* RGBC time */
#define TCS37727_PTIME   0x82 /* Proximity time */
#define TCS37727_WTIME   0x83 /* Wait time */
#define TCS37727_AILTL   0x04 /* Clear interrupt low threshold low byte */
#define TCS37727_AILTH   0x05 /* Clear interrupt low threshold high byte */
#define TCS37727_AIHTL   0x06 /* Clear interrupt high threshold low byte */
#define TCS37727_AIHTH   0x07 /* Clear interrupt high threshold high byte */
#define TCS37727_PILTL   0x08 /* Proximity inter. low threshold low byte */
#define TCS37727_PILTH   0x09 /* Proximity inter. low threshold high byte */
#define TCS37727_PIHTL   0x0A /* Proximity inter. high threshold low byte */
#define TCS37727_PIHTH   0x0B /* Proximity inter. high threshold high byte */
#define TCS37727_PERS    0x8C /* Interrupt persistence filters */
#define TCS37727_CONFIG  0x8D /* Configuration */
#define TCS37727_PPULSE  0x8E /* Proximity pulse count */
#define TCS37727_CONTROL 0x8F /* Gain control register */
#define TCS37727_STATUS  0x93 /* Device status */
#define TCS37727_CDATA   0x14 /* Clear ADC data low byte */
#define TCS37727_CDATAH  0x15 /* Clear ADC data high byte */
#define TCS37727_RDATA   0x16 /* Red ADC data low byte */
#define TCS37727_RDATAH  0x17 /* Red ADC data high byte */
#define TCS37727_GDATA   0x18 /* Green ADC data low byte */
#define TCS37727_GDATAH  0x19 /* Green ADC data high byte */
#define TCS37727_BDATA   0x1A /* Blue ADC data low byte */
#define TCS37727_BDATAH  0x1B /* Blue ADC data high byte */
#define TCS37727_PDATA   0x1C /* Proximity ADC data low byte */
#define TCS37727_PDATAH  0x1D /* Proximity ADC data high byte */

/* TCS37727 Command Register */
#define TCS37727_BYTE_TRANS 0x80 /* Repeated byte protocol transaction */
#define TCS37727_INC_TRANS  0xA0 /* Auto-increment protocol transaction */
#define TCS37727_SF_PICLR   0xE5 /* Proximity interrupt clear */
#define TCS37727_SF_CICLR   0xE6 /* Clear channel interrupt clear */
#define TCS37727_SF_PCICLR  0xE7 /* Proximity & Clear channel inter. clear */

/* TCS37727 Enable Register */
#define TCS37727_ENABLE_PIEN (1 << 5) /* Proximity interrupt enable */
#define TCS37727_ENABLE_AIEN (1 << 4) /* Clear channel interrupt enable */
#define TCS37727_ENABLE_WEN  (1 << 3) /* Wait enable */
#define TCS37727_ENABLE_PEN  (1 << 2) /* Proximity enable */
#define TCS37727_ENABLE_AEN  (1 << 1) /* RGBC enable, actives 2-channel ADC */
#define TCS37727_ENABLE_PON  (1 << 0) /* Power ON */

/* TCS37727 Control Register */
#define TCS37727_CONTROL_PDRIVE_100  0x00 /* 100 mA LED Drive Strength */
#define TCS37727_CONTROL_PDRIVE_50   0x04 /* 50 mA LED Drive Strength */
#define TCS37727_CONTROL_PDRIVE_25   0x08 /* 25 mA LED Drive Strength */
#define TCS37727_CONTROL_PDRIVE_12   0x0C /* 12.5 mA LED Drive Strength */
#define TCS37727_CONTROL_PDRIVE_MASK 0x0C /* PDRIVE Mask */
#define TCS37727_CONTROL_AGAIN_1     0x00 /* 1 x gain RGBC Gain Value */
#define TCS37727_CONTROL_AGAIN_4     0x01 /* 4 x gain RGBC Gain Value */
#define TCS37727_CONTROL_AGAIN_16    0x02 /* 16 x gain RGBC Gain Value */
#define TCS37727_CONTROL_AGAIN_60    0x03 /* 60 x gain RGBC Gain Value */
#define TCS37727_CONTROL_AGAIN_MASK  0x03 /* AGAIN Mask */

/* TCS37727 ATIME register values */
#define TCS37727_ATIME_MIN 2400   /* 2.4ms integr. time, max count 1024 */
#define TCS37727_ATIME_MAX 614000 /* 614ms integr. time, max count 0xffff */

#define TCS37727_ATIME_TO_REG(val) (256 - (uint8_t)((val) / 2400))
#define TCS37727_ATIME_TO_US(reg)  ((256 - (uint8_t)(reg)) * 2400)

/* Coefficients for Lux and CT Equations (DN40) */
/* Coefficients in integer format, multiplied by 1000 */
#define DGF_IF        310
#define R_COEF_IF     136
#define G_COEF_IF    1000
#define B_COEF_IF    -444
#define CT_COEF_IF   3810
#define CT_OFFSET_IF 1391

namespace upm {

typedef struct {
    uint32_t red;           /**< IR compensated channels red */
    uint32_t green;         /**< IR compensated channels green */
    uint32_t blue;          /**< IR compensated channels blue */
    uint32_t clear;         /**< channels clear */
    uint32_t lux;           /**< Lux */
    uint32_t ct;            /**< Color temperature */
} tcs37727_data_t;

/**
 * @brief TCS37727 Color Light-To-Digital Converter
 * @defgroup tcs37727 libupm-tcs37727
 * @ingroup ams i2c color
 */

/**
 * @library tcs37727
 * @sensor tcs37727
 * @comname TCS37727 Color Light-To-Digital Converter
 * @type color
 * @man ams
 * @web http://ams.com/eng/Products/Light-Sensors
 * @con i2c
 *
 * @brief API for the TCS37727 Color Light-To-Digital Converter
 *
 * Description in web-link above:
 * The TCS3772 device family provides red, green, blue, and clear (RGBC) light
 * sensing and, when coupled with an external IR LED, proximity detection.
 * These devices detect light intensity under a variety of lighting conditions
 * and through a variety of attenuation materials, including dark glass. The
 * proximity detection feature allows a large dynamic range of operation for
 * accurate short distance detection, such as in a cell phone, for detecting
 * when the user positions the phone close to their ear. An internal state
 * machine provides the ability to put the device into a low power state in
 * between proximity and RGBC measurements providing very low average power
 * consumption.
 *
 * @snippet tcs37727.cxx Interesting
 */
class TCS37727 {
    public:
        /**
         *
         * Instantiates an TCS37727 object
         * Settings: Gain 4x, Proximity Detection off
         *
         * @param bus Number of the used bus
         * @param atime_us RGBC integration time
         * @param devAddr Address of the used I2C device
         */
        TCS37727 (int bus, int atime_us=TCS37727_ATIME_DEFAULT,
                               int devAddr=TCS37727_I2C_ADDRESS);

        /**
         * Check device_id of sensor
         *
         * @return  0 on success
         *         -1 on error
         */
        int checkID(void);

        /**
         * Set active mode, this enables periodic RGBC measurements
         *
         * @return  0 on success
         *         -1 on error
         */
        int setActive(void);

        /**
         * Set standby mode, this disables periodic RGBC measurements
         * Also turns off the sensor when proximity measurement is disabled
         *
         * @return  0 on success
         *         -1 on error
         */
        int setStandby(void);

        /**
         *
         */
        uint8_t trimGain(int rawc);

        /**
         * Read out data of regs and call further function
         * Also saves converted values to variables
         *
         * @return  0 on success
         *         -1 on error
         */
        int sampleData(void);

        /**
         * Get the calculated channel red color
         *
         * @param bSampleData Flag to read sensor
         * @return The channel red color on success
         *         999 on error
         */
        uint32_t getChannelRed(int bSampleData = 0);

        /**
         * Get the calculated channel green color
         *
         * @param bSampleData Flag to read sensor
         * @return The channel green color on success
         *         999 on error
         */
        uint32_t getChannelGreen(int bSampleData = 0);

        /**
         * Get the calculated channel blue color
         *
         * @param bSampleData Flag to read sensor
         * @return The channel blue color on success
         *         999 on error
         */
        uint32_t getChannelBlue(int bSampleData = 0);

        /**
         * Get the calculated channel clear color
         *
         * @param bSampleData Flag to read sensor
         * @return The channel clear color on success
         *         999 on error
         */
        uint32_t getChannelClear(int bSampleData = 0);

        /**
         * Get the calculated lux value
         *
         * @param bSampleData Flag to read sensor
         * @return The lux value on success
         *         999 on error
         */
        uint32_t getLux(int bSampleData = 0);

        /**
         * Get the calculated color temperature
         *
         * @param bSampleData Flag to read sensor
         * @return The color temperature on success
         *         999 on error
         */
        uint32_t getColorTemperature(int bSampleData = 0);

        /**
         * Get calculated sensor values
         *
         * @param data Calculated sensor values
         * @param bSampleData Flag to read sensor
         * @return  0 on success
         *         -1 on error
         */
        int getData(tcs37727_data_t* data, int bSampleData = 0);

    private:

        std::string m_name;

        int m_controlAddr;
        int m_bus;
        mraa::I2c m_i2ControlCtx;

        int s_atime_us;              /* atime value conveted to microseconds */
        int s_again;                 /* amount of gain */

        tcs37727_data_t s_data[1];
    };

}
