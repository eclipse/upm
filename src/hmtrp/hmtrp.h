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
 * @brief C++ API for the HMTRP Serial RF Pro
 *
 * UPM support for the HMTRP Serial RF Pro.  This was tested
 * specifically with the Grove Serial RF Pro module.  In theory,
 * this class should work with the following devices:
 *
 * HM-TRP-433: 414000000-454000000Hz
 * HM-TRP-470: 450000000-490000000Hz
 * HM-TRP-868: 849000000-889000000Hz
 * HM-TRP-915: 895000000-935000000Hz 
 *
 * The only difference being the transmit and receive frequencies
 * supported.
 *
 * By default, the device will simply send and receive any data
 * presented on it's UART interface.  It can be placed into a
 * configuration mode by grounding the CONFIG pin on the module.
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
     * HMTRP Serial RF Pro module constructor
     *
     * @param uart default uart to use (0 or 1)
     */
    HMTRP(int uart=HMTRP_DEFAULT_UART);

    /**
     * HMTRP Serial RF Pro module Destructor
     */
    ~HMTRP();

    /**
     * Check to see if there is data available for reading
     *
     * @param millis number of milliseconds to wait, 0 means no wait (default).
     * @return true if there is data available to be read
     */
    bool dataAvailable(unsigned int millis=0);

    /**
     * read any available data into a user-supplied buffer.
     *
     * @param buffer the buffer to hold the data read
     * @param len the length of the buffer
     * @param millis maxim time in milliseconds to wait for input. -1 means 
     * wait forever (default).
     * @return the number of bytes read, 0 if timed out and millis >= 0
     */
    int readData(char *buffer, size_t len, int millis=-1);

    /**
     * write the data in buffer to the device
     *
     * @param buffer the buffer to hold the data read
     * @param len the length of the buffer
     * @return the number of bytes written
     */
    int writeData(char *buffer, size_t len);

    /**
     * setup the proper tty i/o modes and the baudrate.  The default
     * baud rate is 9600 (B9600).
     *
     * @param baud the desired baud rate.  
     * @return true if successful
     */
    bool setupTty(speed_t baud=B9600);

    /**
     * Look for and verify an OK response. This will look like "OK\r\n"
     *
     * @return true if OK received
     */
    bool checkOK();
    
    /**
     * reset the device to default parameters, except for UART baud rate
     *
     * @return true if successful
     */
    bool reset();

    /**
     * Query the radio to determine it's configuration
     *
     * @param freq operating frequency
     * @param dataRate tx/rx bit rate
     * @param rxBandwidth receiving bandwidth in Khz
     * @param modulation modulation frequency in Khz
     * @param txPower transmission power (1-7)
     * @param uartBaud UART baud rate
     * @return true if successful
     */
    bool getConfig(uint32_t *freq, uint32_t *dataRate, uint16_t *rxBandwidth,
                   uint8_t *modulation, uint8_t *txPower, uint32_t *uartBaud);

    /**
     * set the frequency.  Note, this is limited depending on which
     * HM-TRP device you are using.  Consult the datasheet.
     *
     * @param freq operating frequency
     * @return true if successful
     */
    bool setFrequency(uint32_t freq);

    /**
     * set the RF data transmission rate.  Valid values are between
     * 1200-115200.
     *
     * @param rate radio transmission rate in baud (1200-115200)
     * @return true if successful
     */
    bool setRFDataRate(uint32_t rate);

    /**
     * set the RX bandwidth.  Valid values are between 30-620 (in Khz)
     *
     * @param rxBand set receive bandwidth (30-620) Khz
     * @return true if successful
     */
    bool setRXBandwidth(uint16_t rxBand);

    /**
     * set the frequency modulation.  Valid values are between 10-160 (in Khz)
     *
     * @param modulation frequency modulation to use (10-160) Khz
     * @return true if successful
     */
    bool setFrequencyModulation(uint8_t modulation);

    /**
     * set the transmit power level.  Valid values are between 0-7,
     * with 7 being maximum power.
     *
     * @param power power level to use during transmit.  Vaild values
     * are between 0-7.
     * @return true if successful
     */
    bool setTransmitPower(uint8_t power);

    /**
     * set the configured baud rate of the UART.  It is strongly
     * recommended that you do not change this or you may lose the
     * ability to communicate with the module.  Valid values are 1200-115200.
     *
     * @param speed desired baud rate to configure the device to use.
     * Valid values are between 1200-115200.
     * @return true if successful
     */
    bool setUARTSpeed(uint32_t speed);

    /**
     * get the RF signal strength.
     *
     * @param strength the returned strength
     * @return true if successful
     */
    bool getRFSignalStrength(uint8_t *strength);

    /**
     * get the Modulation signal strength.
     *
     * @param strength the returned strength
     * @return true if successful
     */
    bool getModSignalStrength(uint8_t *strength);


  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
  };
}


