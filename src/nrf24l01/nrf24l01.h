/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <maa/aio.h>
#include <maa/gpio.h>
#include <maa/spi.h>

/* Memory Map */
#define CONFIG              0x00
#define EN_AA               0x01
#define EN_RXADDR           0x02
#define SETUP_AW            0x03
#define SETUP_RETR          0x04
#define RF_CH               0x05
#define RF_SETUP            0x06
#define STATUS              0x07
#define OBSERVE_TX          0x08
#define CD                  0x09
#define RX_ADDR_P0          0x0A
#define RX_ADDR_P1          0x0B
#define RX_ADDR_P2          0x0C
#define RX_ADDR_P3          0x0D
#define RX_ADDR_P4          0x0E
#define RX_ADDR_P5          0x0F
#define TX_ADDR             0x10
#define RX_PW_P0            0x11
#define RX_PW_P1            0x12
#define RX_PW_P2            0x13
#define RX_PW_P3            0x14
#define RX_PW_P4            0x15
#define RX_PW_P5            0x16
#define FIFO_STATUS         0x17

/* Bit Mnemonics */
#define MASK_RX_DR          6
#define MASK_TX_DS          5
#define MASK_MAX_RT         4
#define EN_CRC              3
#define CRCO                2
#define PWR_UP              1
#define PRIM_RX             0
#define ENAA_P5             5
#define ENAA_P4             4
#define ENAA_P3             3
#define ENAA_P2             2
#define ENAA_P1             1
#define ENAA_P0             0
#define ERX_P5              5
#define ERX_P4              4
#define ERX_P3              3
#define ERX_P2              2
#define ERX_P1              1
#define ERX_P0              0
#define AW                  0
#define ARD                 4
#define ARC                 0
#define PLL_LOCK            4
#define RF_DR               3
#define RF_PWR              1
#define LNA_HCURR           0
#define RX_DR               6
#define TX_DS               5
#define MAX_RT              4
#define RX_P_NO             1
#define TX_FULL             0
#define PLOS_CNT            4
#define ARC_CNT             0
#define TX_REUSE            6
#define FIFO_FULL           5
#define TX_EMPTY            4
#define RX_FULL             1
#define RX_EMPTY            0

/* Instruction Mnemonics */
#define R_REGISTER            0x00
#define W_REGISTER            0x20
#define REGISTER_MASK         0x1F
#define R_RX_PAYLOAD          0x61
#define W_TX_PAYLOAD          0xA0
#define FLUSH_TX              0xE1
#define FLUSH_RX              0xE2
#define REUSE_TX_PL           0xE3
#define NOP                   0xFF

/* Nrf24l settings */
#define ADDR_LEN        5
#define NRF_CONFIG         ((1<<EN_CRC) | (0<<CRCO) )

#define MAX_BUFFER            32

#define HIGH                  1
#define LOW                    0

namespace upm {

typedef void (* funcPtrVoidVoid) ();

/**
 * @brief C++ API for NRF24l01 transceiver module
 *
 * This file defines the NRF24l01 C++ interface for libnrf24l01
 *
 * @snippet nrf_receiver.cxx Interesting
 * @snippet nrf_transmitter.cxx Interesting
 */
class NRF24l01 {
    public:
        /**
         * Instanciates a NRF24l01 object
         *
         * @param cs chip select pin
         */
        NRF24l01 (uint8_t cs);

        /**
         * NRF24l01 object destructor
         */
        ~NRF24l01 ();

        /**
         * Return name of the component
         */
        std::string name()
        {
            return m_name;
        }

        /**
         * Initialize needed Gpio pins and SPI interface
         *
         * @param chipSelect setting up the chip select pin
         * @param chipEnable setting up the chip enable pin
         */
        void nrfInitModule (uint8_t chipSelect, uint8_t chipEnable);

        /**
         * Configure NRF24l01 chip
         */
        void nrfConfigModule ();

        /**
         * Send the buffer data
         *
         * @param *value pointer to the buffer
         */
        void nrfSend (uint8_t *value);

        /**
         * Send the data located in inner bufer, user must fill the
         * m_txBuffer buffer
         */
        void nrfSend ();

        /**
         * Set recieving address of the device
         *
         * @param addr 5 bytes addres
         */
        void nrfSetRXaddr (uint8_t * addr);

        /**
         * Set recipient address. nrfSend method will send the data buffer
         * to this address
         *
         * @param addr 5 bytes addres
         */
        void nrfSetTXaddr (uint8_t * addr);

        /**
         * Set broadcasting address.
         *
         * @param addr 5 bytes addres
         */
        void nrfSetBroadcastAddr (uint8_t * addr);

        /**
         * Set payload size.
         *
         * @param load size of the payload (MAX 32)
         */
        void nrfSetPayload (uint8_t load);

        /**
         * Check if data arrived
         */
        bool nrfDataReady ();

        /**
         * Check if chip in sending mode
         */
        bool nrfIsSending ();

        /**
         * Check if recieving stack is empty
         */
        bool nrfRXFifoEmpty ();

        /**
         * Check if transmitting stack is empty
         */
        bool nrfTXFifoEmpty ();

        /**
         * Sink all arrived data into the provided buffer
         *
         * @param load size of the payload (MAX 32)
         */
        void nrfGetData (uint8_t * data);

        /**
         * Check the chip state
         */
        uint8_t nrfGetStatus ();

        /**
         * Transmit provided data to the chip
         *
         * @param *dataout pointer to the buffer with data
         * @param len length of the buffer
         */
        void nrfTransmitSync (uint8_t *dataout, uint8_t len);

        /**
         * Recieve data from the chip
         *
         * @param *dataout pointer to the buffer with data
         * @param *datain pointer to the buffer where the arrived data
         * will be sinked
         * @param len length of the buffer
         */
        void nrfTransferSync (uint8_t *dataout ,uint8_t *datain, uint8_t len);

        /**
         * Write byte value into a register
         *
         * @param reg register address
         * @param value the value to write
         */
        void nrfConfigRegister (uint8_t reg, uint8_t value);

        /**
         * Read continues data from register
         *
         * @param reg register address
         * @param *value pointer to the buffer
         * @param len length of the buffer
         */
        void nrfReadRegister (uint8_t reg, uint8_t * value, uint8_t len);

        /**
         * Write continues data to register
         *
         * @param reg register address
         * @param *value pointer to the buffer
         * @param len length of the buffer
         */
        void nrfWriteRegister (uint8_t reg, uint8_t * value, uint8_t len);

        /**
         * Power up reciever
         */
        void nrfPowerUpRX ();

        /**
         * Power up transmitter
         */
        void nrfPowerUpTX ();

        /**
         * Power down all
         */
        void nrfPowerDown ();

        /**
         * Set chip enable pin HIGH
         */
        maa_result_t nrfCEHigh ();

        /**
         * Set chip enable LOW
         */
        maa_result_t nrfCELow ();

        /**
         * Set chip select pin LOW
         */
        maa_result_t nrfCSOn ();

        /**
         * Set chip select pin HIGH
         */
        maa_result_t nrfCSOff ();

        /**
         * Flush reciver stack
         */
        void nrfFlushRX ();

        /**
         * Pulling method which listenning for arrived data, if data
         * arrived dataRecievedHandler will be triggered
         */
        void nrfListenForChannel();

        uint8_t                m_rxBuffer[MAX_BUFFER]; /**< Reciver buffer */
        uint8_t                m_txBuffer[MAX_BUFFER]; /**< Transmit buffer */

        funcPtrVoidVoid dataRecievedHandler; /**< Data arrived handler */
    private:
        maa_spi_context        m_spi;
        uint8_t                m_ce;
        uint8_t                m_csn;
        uint8_t                m_channel;
        uint8_t             m_ptx;
        uint8_t                m_payload;
        uint8_t                m_localAddress[5];

        maa_gpio_context     m_csnPinCtx;
        maa_gpio_context     m_cePinCtx;

        std::string         m_name;
};

}
