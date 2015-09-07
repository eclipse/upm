/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mraa/uart.h>

#define HMTRP_DEFAULT_UART 0

namespace upm {
/**
 * @brief HMTRP Serial RF Pro library
 * @defgroup hmtrp libupm-hmtrp
 * @ingroup seeed uart wifi
 */

/**
 * @library hmtrp
 * @sensor hmtrp
 * @comname Grove Serial RF Pro
 * @altname HMTRP-433 HMTRP-470 HMTRP-868 HMTRP-915
 * @type wifi
 * @man seeed
 * @con uart
 *
 * @brief API for the HM-TRP Serial RF Pro transceiver
 *
 * UPM support for the HM-TRP Serial RF Pro transceiver. This was tested
 * specifically with the Grove Serial RF Pro transceiver. In theory,
 * this class should work with the following devices:
 *
 * HM-TRP-433: 414000000-454000000Hz
 * HM-TRP-470: 450000000-490000000Hz
 * HM-TRP-868: 849000000-889000000Hz
 * HM-TRP-915: 895000000-935000000Hz 
 *
 * The only difference is the transmit and receive frequencies
 * supported.
 *
 * By default, the device simply sends and receives any data
 * presented on its UART interface. It can be put into a
 * configuration mode by grounding the CONFIG pin on the transceiver.
 *
 * @image html hmtrp.jpg
 * @snippet hmtrp.cxx Interesting
 */
  class HMTRP {
  public:

    // HMTRP opcodes
    typedef enum { RESET               = 0xf0,
                   GET_CONFIG          = 0xe1,
                   SET_FREQUENCY       = 0xd2,
                   SET_RF_DATARATE     = 0xc3, // 1200-115200 (baud)
                   SET_RX_BW           = 0xb4, // 30-620 (Khz)
                   SET_FREQ_MODULATION = 0xa5, // 10-160 (KHz)
                   SET_TX_POWER        = 0x96, // 0-7
                   SET_UART_SPEED      = 0x1e, // recommended not to change
                   GET_RF_SIGNAL_STR   = 0xa7,
                   GET_MOD_SIGNAL_STR  = 0x78
    } HMTRP_OPCODE_T;
    
    /**
     * HMTRP Serial RF Pro transceiver constructor
     *
     * @param uart Default UART to use (0 or 1)
     */
    HMTRP(int uart=HMTRP_DEFAULT_UART);

    /**
     * HMTRP destructor
     */
    ~HMTRP();

    /**
     * Checks to see if there is data available for reading
     *
     * @param millis Number of milliseconds to wait; 0 means no waiting (default).
     * @return True if there is data available for reading
     */
    bool dataAvailable(unsigned int millis=0);

    /**
     * Reads any available data in a user-supplied buffer
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @param millis Maximum time in milliseconds to wait for input. -1 means 
     * waiting forever (default).
     * @return Number of bytes read; 0 if timed out and millis is >= 0
     */
    int readData(char *buffer, int len, int millis=-1);

    /**
     * Writes the data in the buffer to the device
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(char *buffer, int len);

    /**
     * Sets up proper tty I/O modes and the baud rate. The default
     * baud rate is 9,600 (B9600).
     *
     * @param baud Desired baud rate.  
     * @return True if successful
     */
    bool setupTty(speed_t baud=B9600);

    /**
     * Looks for and verifies an OK response. This looks like "OK\r\n"
     *
     * @return True if OK received
     */
    bool checkOK();
    
    /**
     * Resets the device to default parameters, except for the UART baud rate
     *
     * @return True if successful
     */
    bool reset();

    /**
     * Queries the radio to determine its configuration
     *
     * @param freq Operating frequency
     * @param dataRate TX/RX bit rate
     * @param rxBandwidth Receiving bandwidth in Khz
     * @param modulation Modulation frequency in Khz
     * @param txPower Transmission power (1-7)
     * @param uartBaud UART baud rate
     * @return True if successful
     */
    bool getConfig(uint32_t *freq, uint32_t *dataRate, uint16_t *rxBandwidth,
                   uint8_t *modulation, uint8_t *txPower, uint32_t *uartBaud);

    /**
     * Sets the frequency. Note: this is limited depending on which
     * HM-TRP device you are using. Consult the datasheet.
     *
     * @param freq Operating frequency
     * @return True if successful
     */
    bool setFrequency(uint32_t freq);

    /**
     * Sets the RF data transmission rate. Valid values are between
     * 1,200 and 115,200.
     *
     * @param rate Radio transmission rate in baud (1,200-115,200)
     * @return True if successful
     */
    bool setRFDataRate(uint32_t rate);

    /**
     * Sets the RX bandwidth. Valid values are between 30 and 620 (in Khz)
     *
     * @param rxBand RX bandwidth in Khz (30-620)
     * @return True if successful
     */
    bool setRXBandwidth(uint16_t rxBand);

    /**
     * Sets the frequency modulation. Valid values are between 10 and 160 (in Khz)
     *
     * @param modulation Frequency modulation to use, in Khz (10-160)
     * @return True if successful
     */
    bool setFrequencyModulation(uint8_t modulation);

    /**
     * Sets the transmit power level. Valid values are between 0 and 7,
     * 7 being the maximum power.
     *
     * @param power Power level to use during transmission. Valid values
     * are between 0 and 7.
     * @return True if successful
     */
    bool setTransmitPower(uint8_t power);

    /**
     * Sets the configured baud rate of the UART. It is strongly
     * recommended you do not change this, or you may lose the
     * ability to communicate with the transceiver. Valid values are 1,200-115,200.
     *
     * @param speed Desired baud rate to configure the device to use
     * Valid values are between 1,200 and 115,200.
     * @return True if successful
     */
    bool setUARTSpeed(uint32_t speed);

    /**
     * Gets the RF signal strength
     *
     * @param strength Returned strength
     * @return True if successful
     */
    bool getRFSignalStrength(uint8_t *strength);

    /**
     * Gets the modulation signal strength.
     *
     * @param strength Returned strength
     * @return True if successful
     */
    bool getModSignalStrength(uint8_t *strength);


  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
  };
}


