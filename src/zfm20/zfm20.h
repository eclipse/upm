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
     * @brief UPM library for the ZFM-20 fingerprint sensor module
     * @defgroup zfm20 libupm-zfm20
     * @ingroup seeed serial gas
     */

    /**
     * @library zfm20
	 * @sensor zfm20
     * @comname ZFM-20 Fingerprint Sensor Module
	 * @type other
     * @man seeed
     * @con uart
     *
     * @brief C++ API for the ZFM-20 fingerprint sensor module
     *
     * This class was tested on the Grove Fingerprint Sensor
     * Module. It can store up to 163 fingerprints.
     *
     * It is connected via a UART at 57600 baud.
     * 
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
      // end of module specific errors
      ERR_INTERNAL_ERR                  = 0xff  // API internal error
    } ZFM20_ERRORS_T;
    
    typedef enum {
      PKT_COMMAND                       = 0x01,
      PKT_DATA                          = 0x02,
      PKT_ACK                           = 0x07,
      PKT_END_DATA                      = 0x08
    } ZFM20_PKTCODES_T;

    /**
     * ZFM20 module constructor
     *
     * @param uart default uart to use (0 or 1)
     */
    ZFM20(int uart);

    /**
     * ZFM20 module Destructor
     */
    ~ZFM20();

    /**
     * check to see if there is data available for reading
     *
     * @param millis number of milliseconds to wait, 0 means no wait.
     * @return true if there is data available to be read
     */
    bool dataAvailable(unsigned int millis);

    /**
     * read any available data into a user-supplied buffer.  Note, the
     * call will block until data is available to be read.  Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer the buffer to hold the data read
     * @param len the length of the buffer
     * @return the number of bytes read
     */
    int readData(char *buffer, size_t len);

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
     * baud rate is 57600 (B57600) for this device.
     *
     * @param baud the desired baud rate.  
     * @return true if successful
     */
    bool setupTty(speed_t baud=B57600);

    /**
     * compose and write a command packet
     *
     * @param pkt the packet
     * @param len length of packet
     * @return the number of bytes written
     */
    int writeCmdPacket(unsigned char *pkt, int len);

    /**
     * verify the packet header and indicate it's validity
     *
     * @param pkt the packet to check
     * @return true if valid checksum, false otherwise
     */
    bool verifyPacket(unsigned char *pkt);

    /**
     * Return the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Reset the Clock
     *
     */
    void initClock();

    /**
     * set the address that should be used to access the module
     *
     * @param addr the address to use
     */
    void setAddress(uint32_t addr) { m_address = addr; };

    /**
     * set the password that should be used to access the module
     *
     * @param pw password to use
     */
    void setPassword(uint32_t pw) { m_password = pw; };

    /**
     * get the returned data from a request
     *
     * @param pkt the buffer to store the returned data into
     * @param len the expected response length.  pkt should be at least this 
     * big.
     * @return true if successful
     */
    bool getResponse(unsigned char *pkt, int len);

    /**
     * verify and authenticate to the module.  The password used is
     * the one last set by setPassword().
     *
     * @return true if successful
     */
    bool verifyPassword();

    /**
     * query the module for the number of stored templates
     * (fingerprints).
     *
     * @return the number of currently stored templates
     */
    int getNumTemplates();

    /**
     * set a new password for the module.  This passowrd will be
     * stored on the module, and will be required in order to access
     * the module in the future.
     *
     * @param pwd the new password to set on the module
     * @return true if successful
     */
    bool setNewPassword(uint32_t pwd);

    /**
     * set a new address for the module.  This address will be
     * stored on the module, and will be required in order to access
     * the module in the future.
     *
     * @param addr the new address to set on the module
     * @return true if successful
     */
    bool setNewAddress(uint32_t addr);

    /**
     * generate a new fingerprint image (scan a fingerprint)
     *
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t generateImage();

    /**
     * convert the image in the image buffer (generated by
     * generateImage()) and store it in one of the two characteristics
     * buffers, 1 or 2
     *
     * @param slot the characteristics buffer to use.  Must be 1 or 2.
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t image2Tz(int slot);

    /**
     * based on the two characteristics bufferes (1 & 2), create a
     * fingerprint model.  Once a model has been successfully created,
     * it can be stored in the module with storeModel().
     *
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t createModel();

    /**
     * once a fingerprint model has been created, this method can be
     * used to store it (via one of the characteristics buffers) in a
     * given location.
     *
     * @param slot the characteristics buffer to store (1 or 2)
     * @param id the location in which to store the model
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t storeModel(int slot, uint16_t id);

    /**
     * delete a stored model.
     *
     * @param id the location containing the model to delete
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t deleteModel(uint16_t id);

    /**
     * delete the model database
     *
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t deleteDB();

    /**
     * search the fingerprint DB and return the id and score, if found
     *
     *
     * @param slot the slot containing a converted image to search for
     * @param id the returned id if found, 0 otherwise
     * @param score the returned score if found, 0 otherwise
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t search(int slot, uint16_t *id, uint16_t *score);

    /**
     * compares the features in characteristic buffers 1 and 2 and
     * returns a score if they match
     *
     * @param score the returned score
     * @return one of the ZFM20_ERRORS_T values
     */
    uint8_t match(uint16_t *score);


  protected:
    int ttyFd() { return m_ttyFd; };
    int setTtyFd(int fd) { m_ttyFd = fd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
    uint32_t m_password;
    uint32_t m_address;
    struct timeval m_startTime;
  };
}


