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

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mraa/common.hpp>
#include <mraa/uart.hpp>
#include <mraa/gpio.hpp>

#define SM130_DEFAULT_UART 0
#define SM130_DEFAULT_RESET_PIN 13

namespace upm {
  
  /**
   * @brief SM130 RFID Reader Module library
   * @defgroup sm130 libupm-sm130
   * @ingroup sparkfun uart gpio rfid
   */

  /**
   * @library sm130
   * @sensor sm130
   * @comname SM130 RFID Reader
   * @type rfid
   * @man sparkfun
   * @web https://www.sparkfun.com/products/10126
   * @con uart gpio
   *
   * @brief API for the SM130 RFID Reader Module
   * 
   * This module defines the SM130 interface for the sm130 RFID library
   *
   * This module was developed using an SM130 and a Sparkfun RFID
   * Evaluation shield using a UART for communications.  It should be
   * fairly trivial to add support for I2C communication in the
   * future, if you have the correct firmware on the SM130.
   *
   * @image html sm130.jpg
   * <br><em>SM130 RFID Reader image provided by SparkFun* under
   * <a href=https://creativecommons.org/licenses/by-nc-sa/3.0/>
   * CC BY-NC-SA-3.0</a>.</em>
   *
   * @snippet sm130.cxx Interesting
   */

  class SM130 {
    
  public:

    // Valid commands
    typedef enum {
      CMD_RESET                  = 0x80,
      CMD_VERSION                = 0x81,
      CMD_SEEK_TAG               = 0x82,
      CMD_SELECT_TAG             = 0x83,
      CMD_AUTHENTICATE           = 0x85,
      CMD_READ16                 = 0x86,
      CMD_READ_VALUE             = 0x87,
      CMD_WRITE16                = 0x89,
      CMD_WRITE_VALUE            = 0x8a,
      CMD_WRITE4                 = 0x8b,
      CMD_WRITE_KEY              = 0x8c,
      CMD_INC_VALUE              = 0x8d,
      CMD_DEC_VALUE              = 0x8e,
      CMD_ANTENNA_POWER          = 0x90,
      CMD_READ_PORT              = 0x91,
      CMD_WRITE_PORT             = 0x92,
      CMD_HALT_TAG               = 0x93,
      CMD_SET_BAUD               = 0x94,
      CMD_SLEEP                  = 0x96
    } CMD_T;

    // valid tag types.
    typedef enum {
      TAG_NONE                   = 0x00, // error/invalid

      TAG_MIFARE_ULTRALIGHT      = 0x01,
      TAG_MIFARE_1K              = 0x02,
      TAG_MIFARE_4K              = 0x03,
      TAG_UNKNOWN                = 0xff
    } TAG_TYPE_T;    

    // Valid authentication keys
    typedef enum {
      KEY_TYPE_EEPROM_A0         = 0x10,
      KEY_TYPE_EEPROM_A1         = 0x11,
      KEY_TYPE_EEPROM_A2         = 0x12,
      KEY_TYPE_EEPROM_A3         = 0x13,
      KEY_TYPE_EEPROM_A4         = 0x14,
      KEY_TYPE_EEPROM_A5         = 0x15,
      KEY_TYPE_EEPROM_A6         = 0x16,
      KEY_TYPE_EEPROM_A7         = 0x17,
      KEY_TYPE_EEPROM_A8         = 0x18,
      KEY_TYPE_EEPROM_A9         = 0x19,
      KEY_TYPE_EEPROM_A10        = 0x1a,
      KEY_TYPE_EEPROM_A11        = 0x1b,
      KEY_TYPE_EEPROM_A12        = 0x1c,
      KEY_TYPE_EEPROM_A13        = 0x1d,
      KEY_TYPE_EEPROM_A14        = 0x1e,
      KEY_TYPE_EEPROM_A15        = 0x1f,

      KEY_TYPE_EEPROM_B0         = 0x20,
      KEY_TYPE_EEPROM_B1         = 0x21,
      KEY_TYPE_EEPROM_B2         = 0x22,
      KEY_TYPE_EEPROM_B3         = 0x23,
      KEY_TYPE_EEPROM_B4         = 0x24,
      KEY_TYPE_EEPROM_B5         = 0x25,
      KEY_TYPE_EEPROM_B6         = 0x26,
      KEY_TYPE_EEPROM_B7         = 0x27,
      KEY_TYPE_EEPROM_B8         = 0x28,
      KEY_TYPE_EEPROM_B9         = 0x29,
      KEY_TYPE_EEPROM_B10        = 0x2a,
      KEY_TYPE_EEPROM_B11        = 0x2b,
      KEY_TYPE_EEPROM_B12        = 0x2c,
      KEY_TYPE_EEPROM_B13        = 0x2d,
      KEY_TYPE_EEPROM_B14        = 0x2e,
      KEY_TYPE_EEPROM_B15        = 0x2f,

      KEY_TYPE_A                 = 0xaa,
      KEY_TYPE_B                 = 0xbb,

      KEY_TYPE_A_AND_TRANSPORT_F = 0xff
    } KEY_TYPES_T;

    /**
     * Instantiates an SM130 object
     *
     * @param uart The UART port.  Default is 0.
     * @param reset The Reset pin.  Default is 13.
     */
    SM130 (int uart=SM130_DEFAULT_UART, int reset=SM130_DEFAULT_RESET_PIN);

    /**
     * SM130 object destructor
     */
    ~SM130 ();

    /**
     * Sets the baud rate for the device.  The default is 19200.
     *
     * @param baud Desired baud rate, default 19200
     * @return mraa::Result value
     */
    mraa::Result setBaudRate(int baud=19200);

    /**
     * Gets the firmware version string.
     *
     * @return The firmware revision
     */
    std::string getFirmwareVersion();

    /**
     * Issues a reset command to the device.
     *
     * @return true if successful
     */
    bool reset();

    /**
     * Resets the device using the hardware RESET pin.  This is
     * required if the device has been put to sleep using the sleep()
     * method.
     */
    void hardwareReset();

    /**
     * Checks to see if a tag is in the RF field, and selects it if
     * one is present.
     *
     * @return true if a tag was detected, false if no tag is present
     * or an error was detected.
     */
    bool select();

    /**
     * Waits for a tag to enter the RF field for up to 'timeout'
     * milliseconds. It will call select() every 100ms until 'timeout'
     * has been exceeded.
     *
     * @param timeout The number of milliseconds to wait for a tag to appear
     * @return true if a tag was detected, false if no tag was
     * detected within the timeout value, or an error occurred
     */
    bool waitForTag(uint32_t timeout);

    /**
     * Set the authentication key for a block.  Depending on the
     * permissions on the tag, the correct key must be authenticated
     * for that block in order to perform read and write operations.
     *
     * @param block The block to authenticate for
     * @param keyType one of the KEY_TYPE_T values
     * @param key The 6 byte key to use for Type A and Type B keys
     * @return true if authentication was successful, false otherwise
     */
    bool authenticate(uint8_t block, KEY_TYPES_T keyType, std::string key="");

    /**
     * Read a 16 byte block.  Depending on the tag, authentication of
     * the block may be required for this method to succeed.
     *
     * @param block The block to read
     * @return The 16 byte block if successful, an empty string otherwise
     */
    std::string readBlock16(uint8_t block);

    /**
     * Read a 4 byte value block.  Depending on the tag, authentication of
     * the block may be required for this method to succeed.
     *
     * @param block The block to read
     * @return The 4 byte signed integer value block if successful, 0 otherwise
     */
    int32_t readValueBlock(uint8_t block);

    /**
     * Write 16 bytes to a block.  Depending on the tag, authentication of
     * the block may be required for this method to succeed.
     *
     * @param block The block to write
     * @param contents A 16 byte string containing the data to write
     * @return true if successful, false otherwise
     */
    bool writeBlock16(uint8_t block, std::string contents);

    /**
     * Write to a 4 byte value block.  Depending on the tag,
     * authentication of the block may be required for this method to
     * succeed.
     *
     * @param block The block to write
     * @param value the signed 4 byte integer to write to the value block
     * @return true if successful, false otherwise
     */
    bool writeValueBlock(uint8_t block, int32_t value);

    /**
     * Write 4 bytes to a block.  This is typically used for
     * Ultralight tags. Depending on the tag, authentication of the
     * block may be required for this method to succeed.
     *
     * @param block The block to write
     * @param contents A 4 byte string containing the data to write
     * @return true if successful, false otherwise
     */
    bool writeBlock4(uint8_t block, std::string contents);

    /**
     * Write a key into one of the 16 EEPROM key slots.  This can be a
     * Type A or Type B key.  It is not possible to read these keys
     * once written.  Once stored, the key can be used for
     * authentication without having to send the key itself.  You can
     * then use the appropriate KEY_TYPE_EEPROM_* keyTypes in a call
     * to authenticate().
     *
     * @param eepromSector A number between 0 and 15, indicating the
     * EEPROM sector you want to store the key in
     * @param keyType Either KEY_TYPE_A or KEY_TYPE_B
     * @param key The 6 byte key to store in the EEPROM
     * @return true if successful, false otherwise
     */
    bool writeKey(uint8_t eepromSector, KEY_TYPES_T keyType, std::string key);

    /**
     * Increment or decrement a value block.
     *
     * @param block The block to adjust
     * @param value The number to increment or decrement the value block by
     * @param incr true to increment, false to decrement
     * @return The contents of the value block after the operation has
     * completed.
     */
    int32_t adjustValueBlock(uint8_t block, int32_t value, bool incr);

    /**
     * Turn the antenna power on or off.  The power is on by default
     * after a reset.  If you turn off the antenna, and methods used
     * for interacting with tags will fail until power is re-enabled.
     *
     * @param on true to enable antenna power, false to disable
     * @return true if successful, false otherwise
     */
    bool setAntennaPower(bool on);

    /**
     * Read the status of the 2 onboard GPIO input pins.  Bit 0 is for
     * input 0, bit 1 for input 1.  All other bits will be 0.
     *
     * @return bitmask of input port status values
     */
    uint8_t readPorts();

    /**
     * Set the output status of the 2 onboard gpio outputs.  Bit 0 is for
     * output 0, bit 1 for output 1.  All other bits will be discarded.
     *
     * @param val bitmask of output status bits to write
     * @return true if successful, false otherwise
     */
    bool writePorts(uint8_t val);

    /**
     * Halts a tag.  Once a tag is halted, it cannot be accessed until
     * it is removed and reinserted into the RF field and selected.
     *
     * @return true if successful, false otherwise
     */
    bool haltTag();

    /**
     * Changes the baud rate of the SM130.  WARNING: This is a
     * potentially dangerous command that could cause you to lose
     * contact with the device.  Once the command is validated and
     * issued, the host baudrate will be changed to match, and this
     * method will wait for a response at the new baudrate for up to 1
     * second. 
     * 
     * If this response does not arrive, the old baudrate will be
     * restored, though there is no way to know whether the SM130
     * actually succeessfully executed the baudrate change.
     *
     * Once the SM130 has changed it's baudrate, the new value will be
     * stored in it's EEPROM, and any further access to the device
     * will need to use the new baudrate.  This is true even after a
     * power on reset.
     *
     * @param baud The new baud rate to set.  Valid values are 9600,
     * 19200, 38400, 57600, and 115200.
     * @return true if successful, false otherwise
     */
    bool setSM130BaudRate(int baud);

    /**
     * Put the SM130 to sleep.  Once the device has been put to sleep,
     * the only way to wake it is via hardwareReset() or a power
     * cycle.
     *
     * @return true if successful, false otherwise
     */
    bool sleep();

    /**
     * Get the last error that occurred.  After a successful
     * operation, this will be 0. See the datasheet for the various
     * errors that can occur in various circumstances.
     *
     * @return The last error code, or 0 if the last operation succeeded.
     */
    char getLastErrorCode() { return m_lastErrorCode; };

    /**
     * Get the text representation of the last error that occurred.
     * The returned string is empty if the last operation completed
     * successfully.
     *
     * @return The last error string if an error occurred, or an empty
     * string if the last operation succeeded.
     */
    std::string getLastErrorString() { return m_lastErrorString; };

    /**
     * Get the UID length of the currently selected tag.
     *
     * @return The UID length of the currently selected tag, or 0 if
     * no tag is currently selected.
     */
    int getUIDLen() { return m_uidLen; };

    /**
     * Get the UID of the currently selected tag.
     *
     * @return The UID of the currently selected tag, or an empty string if
     * no tag is currently selected.
     */
    std::string getUID() { return m_uid; };

    /**
     * Get the tag type of the currently selected tag.
     *
     * @return The tag type of the currently selected tag, or TAG_NONE
     * if no tag is currently selected.
     */
    TAG_TYPE_T getTagType() { return m_tagType; };

    /**
     * Convert the supplied tag type into a human readable string.
     *
     * @param tag One of the TAG_TYPE_T values
     * @return A string representation of the supplied tag type
     */
    std::string tag2String(TAG_TYPE_T tag);

    /**
     * This is a convenience function that converts a supplied string
     * into a space separated hex formatted string.  This can be
     * useful for printing out binary data in a human readable format,
     * like the UID.
     *
     * @param input The string to convert
     * @return A string representation of the input in space separated
     * hex values
     */
    std::string string2HexString(std::string input);

  protected:
    mraa::Uart m_uart;
    mraa::Gpio m_gpioReset;

    std::string sendCommand(CMD_T cmd, std::string data);
    void initClock();
    uint32_t getMillis();

  private:
    int m_uidLen;
    std::string m_uid;

    char m_lastErrorCode;
    std::string m_lastErrorString;

    TAG_TYPE_T m_tagType;

    int m_baud;

    struct timeval m_startTime;

    void clearError()
    { 
      m_lastErrorCode = 0;
      m_lastErrorString.clear();
    }
  };

}
