/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
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

#define ZFM20_DEFAULT_UART 0

// protocol start codes
#define ZFM20_START1 0xef
#define ZFM20_START2 0x01

#define ZFM20_MAX_PKT_LEN 256

#define ZFM20_TIMEOUT 5000 // in ms

#define ZFM20_DEFAULT_PASSWORD 0x00000000
#define ZFM20_DEFAULT_ADDRESS  0xffffffff


namespace upm {
    /**
     * @brief ZFM-20 Fingerprint Sensor Module library
     * @defgroup zfm20 libupm-zfm20
     * @ingroup seeed uart touch
     */

    /**
     * @library zfm20
     * @sensor zfm20
     * @comname ZFM-20 Fingerprint Sensor
     * @altname Grove Fingerprint Sensor
     * @type touch
     * @man seeed
     * @con uart
     *
     * @brief API for the ZFM-20 Fingerprint Sensor Module
     *
     * This class was tested on the Grove Fingerprint Sensor
     * Module. It can store up to 163 fingerprints.
     *
     * It is connected via a UART at 57,600 baud.
     *
     * @image html zfm20.jpg
     * This example demonstrates how to register and store a new fingerprint
     * @snippet zfm20-register.cxx Interesting
     * This example demonstrates reading a fingerprint and locating it in the DB
     * @snippet zfm20.cxx Interesting
     */
  class ZFM20 {
  public:

    // commands
    typedef enum {
      CMD_GEN_IMAGE                     = 0x01,
      CMD_IMG2TZ                        = 0x02,
      CMD_MATCH                         = 0x03,
      CMD_SEARCH                        = 0x04,
      CMD_REGMODEL                      = 0x05,
      CMD_STORE                         = 0x06,
      CMD_LOAD_TMPL                     = 0x07,
      CMD_UPLOAD_TMPL                   = 0x08,
      CMD_DOWNLOAD_TMPL                 = 0x09,
      CMD_UPLOAD_IMAGE                  = 0x0a,
      CMD_DOWNLOAD_IMAGE                = 0x0b,
      CMD_DELETE_TMPL                   = 0x0c,
      CMD_EMPTYDB                       = 0x0d,
      CMD_SET_SYSPARAMS                 = 0x0e,
      CMD_GET_SYSPARAMS                 = 0x0f,
      CMD_SET_PASSWORD                  = 0x12,
      CMD_VERIFY_PASSWORD               = 0x13,
      CMD_GET_RANDOM_NUMBER             = 0x14,
      CMD_SET_ADDRESS                   = 0x15,
      CMD_GET_TMPL_COUNT                = 0x1d,
      CMD_GET_INDEX_TABLE               = 0x1f
    } ZFM20_COMMAND_T;

    // Error response codes
    typedef enum {
      ERR_OK                            = 0x00,
      ERR_PACKET_RX_ERROR               = 0x01,
      ERR_NO_FINGER                     = 0x02,
      ERR_FP_IMAGE_FAILED               = 0x03,
      ERR_FP_TOO_MESSY                  = 0x06,
      ERR_FP_IMAGE_FEW_FEATURES         = 0x07,
      ERR_FP_NOMATCH                    = 0x08,
      ERR_FP_NOTFOUND                   = 0x09,
      ERR_FP_ENROLLMISMATCH             = 0x0a,
      ERR_BAD_LOCATION                  = 0x0b,
      ERR_DB_ERROR                      = 0x0c,
      ERR_UPLOAD_FEAT_FAILED            = 0x0d,
      ERR_NO_MORE_PACKETS               = 0x0e,
      ERR_UPLOAD_IMG_FAILED             = 0x0f,
      ERR_RM_TMPL_FAILED                = 0x10,
      ERR_EMPTY_DB_FAILED               = 0x11,
      ERR_INVALID_PWD                   = 0x13,
      ERR_INVALID_IMAGE                 = 0x15,
      ERR_RW_FLASH_ERROR                = 0x18,
      ERR_INVALID_REG                   = 0x1a,
      ERR_INVALID_ADDR                  = 0x20,
      ERR_NEEDS_PWD                     = 0x21,
      // end of module-specific errors
      ERR_INTERNAL_ERR                  = 0xff  // API internal error
    } ZFM20_ERRORS_T;

    typedef enum {
      PKT_COMMAND                       = 0x01,
      PKT_DATA                          = 0x02,
      PKT_ACK                           = 0x07,
      PKT_END_DATA                      = 0x08
    } ZFM20_PKTCODES_T;

    /**
     * ZFM20 constructor
     *
     * @param uart Default UART to use (0 or 1)
     */
    ZFM20(int uart);

    /**
     * ZFM20 destructor
     */
    ~ZFM20();

    /**
     * Checks to see if there is data available for reading
     *
     * @param millis Number of milliseconds to wait; 0 means no waiting
     * @return true if there is data available for reading
     */
    bool dataAvailable(unsigned int millis);

    /**
     * Reads any available data in a user-supplied buffer. Note: the
     * call blocks until data is available to be read. Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes read
     */
    int readData(char *buffer, int len);

    /**
     * Writes the data in the buffer to the device
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(char *buffer, int len);

    /**
     * Sets up proper tty I/O modes and the baud rate. For this device,
     * the default baud rate is 57,600 (B57600).
     *
     * @param baud Desired baud rate.
     * @return True if successful
     */
    bool setupTty(speed_t baud=B57600);

    /**
     * Composes and writes a command packet
     *
     * @param pkt Packet
     * @param len Length of packet
     * @return Number of bytes written
     */
    int writeCmdPacket(uint8_t *pkt, int len);

    /**
     * Verifies the packet header and indicates its validity
     *
     * @param pkt Packet to check
     * @param len Length of packet
     * @return True if the packet is valid, false otherwise
     */
    bool verifyPacket(uint8_t *pkt, int len);

    /**
     * Returns the number of milliseconds elapsed since initClock()
     * was last called
     *
     * @return Elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Resets the clock
     *
     */
    void initClock();

    /**
     * Sets the address that should be used to access the module
     *
     * @param addr Address to use
     */
    void setAddress(uint32_t addr) { m_address = addr; };

    /**
     * Sets the password that should be used to access the module
     *
     * @param pw Password to use
     */
    void setPassword(uint32_t pw) { m_password = pw; };

    /**
     * Gets the returned data from a request
     *
     * @param pkt Buffer to store the returned data
     * @param len Expected response length; pkt should be at least this
     * large
     * @return True if successful
     */
    bool getResponse(uint8_t *pkt, int len);

    /**
     * Verifies and authenticates to the module. The password used is
     * the last one set by setPassword().
     *
     * @return True if successful
     */
    bool verifyPassword();

    /**
     * Queries the module for the number of stored templates
     * (fingerprints).
     *
     * @return Number of currently stored templates
     */
    int getNumTemplates();

    /**
     * Sets a new password for the module. This passowrd is
     * stored in the module, and is required to access
     * the module in the future.
     *
     * @param pwd New password to set on the module
     * @return True if successful
     */
    bool setNewPassword(uint32_t pwd);

    /**
     * Sets a new address for the module. This address is
     * stored in the module, and is required to access
     * the module in the future.
     *
     * @param addr New address to set on the module
     * @return True if successful
     */
    bool setNewAddress(uint32_t addr);

    /**
     * Generates a new fingerprint image (scans a fingerprint)
     *
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t generateImage();

    /**
     * Converts the image in the image buffer (generated by
     * generateImage()) and stores it in one of the two characteristics
     * buffers, 1 or 2
     *
     * @param slot Characteristics buffer to use; must be 1 or 2
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t image2Tz(int slot);

    /**
     * Based on the two characteristics buffers (1 & 2), creates a
     * fingerprint model. Once a model is successfully created,
     * it can be stored in the module with storeModel().
     *
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t createModel();

    /**
     * Once a fingerprint model is created, this method can be
     * used to store it (via one of the characteristics buffers) in a
     * given location.
     *
     * @param slot Characteristics buffer to store the model, 1 or 2
     * @param id Location to store the model
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t storeModel(int slot, uint16_t id);

    /**
     * Deletes a stored model
     *
     * @param id Location containing the model to delete
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t deleteModel(uint16_t id);

    /**
     * Deletes the model database (DB)
     *
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t deleteDB();

    /**
     * Searches the fingerprint DB and returns an ID and score, if found
     *
     *
     * @param slot Slot containing a converted image to search for
     * @param id ID if found, 0 otherwise
     * @param score Score if found, 0 otherwise
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t search(int slot, uint16_t *id, uint16_t *score);

    /**
     * Compares the features in characteristics buffers 1 and 2 and
     * returns a score if they match
     *
     * @param score Score
     * @return One of the ZFM20_ERRORS_T values
     */
    uint8_t match(uint16_t *score);


  protected:
    int ttyFd() { return m_ttyFd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
    uint32_t m_password;
    uint32_t m_address;
    struct timeval m_startTime;
  };
}


