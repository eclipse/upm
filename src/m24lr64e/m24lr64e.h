/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 *
 * This code was adapted from the Seeed Studio code at:
 * https://github.com/Seeed-Studio/NFC_Tag_M24LR6E  
 *
 * Copyright (c) 2014 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : lawliet zou
 * Create Time: March 2014
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
#include <mraa/i2c.hpp>

#define M24LR64E_I2C_BUS 0
#define M24LR64E_DEFAULT_I2C_ADDR 0x53
#define M24LR64E_DEFAULT_I2C_ADDR_E2 (M24LR64E_DEFAULT_I2C_ADDR | 0x04)

namespace upm {
  
  /**
   * @brief Grove NFC Tag
   * @defgroup m24lr64e libupm-m24lr64e
   * @ingroup seeed i2c other
   */

  /**
   * @library m24lr64e
   * @sensor m24lr64e
   * @comname Grove NFC Tag
   * @type other
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_NFC_Tag
   * @con i2c
   *
   * @brief C++ API for the M24LR64E based Grove NFC Tag
   *
   * The Grove NFC tag is, in essence, an 8KB EEPROM that can be written
   * to or read from using I2C and NFC equipped devices.
   *
   * The USER mode (default) allows read and write access to all 8KB
   * of space, provided the sector security status (SSS) allows it.
   * The ROOT mode allows modification of the SSS data and other
   * information, provided the proper password is submitted.  The
   * default password for a new tag is 0x00000000.  See the data sheet
   * for much more detailed information.
   *
   * The Seeed Studio wiki page for this device includes a link to an
   * Android application that can be used to also read and write the
   * device via NFC, as well as set the NFC passwords, which cannot be
   * done via I2C.
   *
   * @snippet m24lr64e.cxx Interesting
   */
  class M24LR64E {
  public:
    
    static const int EEPROM_I2C_LENGTH          = 8192;
    static const int PASSWORD_LENGTH            = 4;
    static const int SECTOR_SECURITY_STATUS_BASE_ADDR = 0x800; // 2048
    
    static const uint8_t LOCK_PROTECT_BIT       = 0x01;
    static const uint8_t WRITE_READ_PROTECT_BIT = 0x02;
    static const uint8_t PASSWORD_CTRL_BIT      = 0x04;
    
    static const int UID_LENGTH                 = 8; // bytes

    static const unsigned int I2C_WRITE_TIME    = 5; // 5ms

    /**
     * M24LR64E addresses, accessable only in root mode
     */
    typedef enum {
      I2C_PASSWORD_ADDR                  = 2304,
      RF_PASSWORD_1_ADDR                 = 2308, // RF pwds not available in
      RF_PASSWORD_2_ADDR                 = 2312, // i2c access modes
      RF_PASSWORD_3_ADDR                 = 2316,
      DSFID_ADDR                         = 2320, // 1 byte
      AFI_ADDR                           = 2321, // 1 byte
      RESV_ADDR                          = 2322, // 1 bytes
      CONFIG_ADDR                        = 2323, // 1 bytes
      UID_ADDR                           = 2324, // 8 bytes
      MEM_SIZE_ADDR                      = 2332, // 3 bytes
      IC_REF_ADDR                        = 2335, // 1 byte
      PROG_COMP_ENERGY_HARVEST_ADDR      = 2339  // 1 byte
    } M24LR64E_ADDR_T;

    enum AccessMode {
      USER_MODE = 0x0,   // offer simple read/write access right
      ROOT_MODE = 0x1    // offer password change access right
    };

    enum SectorAccessRight {
      //      **********************************
      //      *  submit passWd *   no submit   * 
      //b2,b1 *  Read * Write  *  Read * Write *
      // 00   *    1       1        1      0   *
      // 01   *    1       1        1      1   *
      // 10   *    1       1        0      0   *
      // 11   *    0       1        0      0   *
      //      **********************************
      Access_1110 = 0,
      Access_1111 = 1,
      Access_1100 = 2,
      Access_0111 = 3,
    };

    enum SectorSelectPassWd {
      //00 => no passwd protect
      //01 => passWd 1
      //10 => passWd 2
      //11 => passwd 3
      noPasswd = 0,
      passwd_1 = 1,
      passwd_2 = 2,
      passwd_3 = 3,
    };

    /**
     * m24lr64e constructor
     *
     * @param bus i2c bus to use
     * @param mode the access mode (user or root) to use
     */
    M24LR64E(int bus, AccessMode mode = USER_MODE);

    /**
     * M24LR64E Destructor
     */
    ~M24LR64E();
    
    /**
     * submit an i2c access password
     *
     * @param passwd the 4-byte access password
     */
    bool submitPasswd(uint32_t passwd);

    /**
     * write a new i2c password
     *
     * @param passwd the 4-byte access password
     */
    bool writePasswd(uint32_t passwd);

    /**
     * Set the protection bit for a sector. Must be in ROOT mode
     *
     * @param sectorNumber the sector whose protection you are modifying
     * @param protectEnable true if you are enabling protection
     * @param accessRight the access rights to set
     * @param passwd the password number to enable, if any
     */
    void sectorProtectConfig(unsigned int sectorNumber, 
                             bool protectEnable, 
                             SectorAccessRight accessRight, 
                             SectorSelectPassWd passwd);

    /**
     * Clear the sector protection bits.  Must be in ROOT mode.
     */
    void clearSectorProtect(void);

    /**
     * Set or clear the Sector Security Status Lock bit for a sector.  
     * Must be in ROOT mode.
     *
     * @param sectorNumber the sector who's SSS you want to modify
     * @param sockEnable true r false to set or clear the bit
     */
    void sectorWriteLockBit(unsigned int sectorNumber, 
                            bool sockEnable);

    /**
     * return the Data Storage Familiy Identifier
     * Must be in ROOT mode.
     *
     * @return the DSFID
     */
    uint8_t getDSFID();

    /**
     * return the Application Family Identifier
     * Must be in ROOT mode.
     *
     * @return the AFI
     */
    uint8_t getAFI();

    /**
     * return the Unique ID.
     * Must be in ROOT mode.
     *
     * @param buf buffer to hold returned UID.  Must be UID_LENGTH bytes.
     */
    void getUID(uint8_t* buf);

    /**
     * return the memory size
     * Must be in ROOT mode.
     *
     * @return the amount of memory present
     */
    uint32_t getMemorySize();

    /**
     * set all memory to 0, if permissions allow
     */
    void clearMemory();

    /**
     * write a byte to EEPROM
     *
     * @param address address to write to
     * @param data data to write
     */
    void writeByte(unsigned int address, uint8_t data);

    /**
     * write bytes to EEPROM
     *
     * @param address address to write to
     * @param data data to write
     * @param data length of data buffer
     */
    void writeBytes(unsigned int address, uint8_t* buf, unsigned int len);

    /**
     * read a byte from EEPROM
     *
     * @param address address to read from
     * @return data value read
     */
    uint8_t readByte(unsigned int address);

    /**
     * read multiple bytes from EEPROM
     *
     * @param address address to read from
     * @param buffer buffer to store data
     * @param len number of bytes to read
     */
    void readBytes(unsigned int address, uint8_t* buf, unsigned int len);

  protected:
    mraa::I2c m_i2c;
    void EEPROM_Write_Byte(unsigned int address, uint8_t data);
    void EEPROM_Write_Bytes(unsigned int address, uint8_t* data,
                            unsigned int len);
    uint8_t EEPROM_Read_Byte(unsigned int address);
    unsigned int EEPROM_Read_Bytes(unsigned int address, 
                                   uint8_t* buf, unsigned int len);

  private:
    uint8_t m_addr;
  };
}


