/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

#ifndef M24LR64E_H_
#define M24LR64E_H_

#include "upm.h"
#include "mraa/i2c.h"

#define M24LR64E_I2C_BUS 0
#define M24LR64E_DEFAULT_I2C_ADDR 0x53
#define M24LR64E_DEFAULT_I2C_ADDR_E2 (M24LR64E_DEFAULT_I2C_ADDR | 0x04)

#define M24LR64E_EEPROM_I2C_LENGTH 8192
#define M24LR64E_PASSWORD_LENGTH 4
#define M24LR64E_SECTOR_SECURITY_STATUS_BASE_ADDR 0x800
#define M24LR64E_LOCK_PROTECT_BIT 0x01
#define M24LR64E_WRITE_READ_PROTECT_BIT 0x02
#define M24LR64E_PASSWORD_CTRL_BIT 0x04

#define M24LR64E_UID_LENGTH 8
#define M24LR64E_I2C_WRITE_TIME 5

/**
 * @file m24lr64e.h
 * @library m24lr64e
 * @brief C API for M24LR64E based NFC Tags
 *
 * @include m24lr64e.c
 */

#include <stdint.h>

typedef enum {
    M24LR64E_I2C_PASSWORD_ADDRESS                  = 2304,
    M24LR64E_RF_PASSWORD_1_ADDRESS                 = 2308, // RF pwds not available in
    M24LR64E_RF_PASSWORD_2_ADDRESS                 = 2312, // I2C access modes
    M24LR64E_RF_PASSWORD_3_ADDRESS                 = 2316,
    M24LR64E_DSFID_ADDRESS                         = 2320, // 1 byte
    M24LR64E_AFI_ADDRESS                           = 2321, // 1 byte
    M24LR64E_RESV_ADDRESS                          = 2322, // 1 bytes
    M24LR64E_CONFIG_ADDRESS                        = 2323, // 1 bytes
    M24LR64E_UID_ADDRESS                           = 2324, // 8 bytes
    M24LR64E_MEM_SIZE_ADDRESS                      = 2332, // 3 bytes
    M24LR64E_IC_REF_ADDRESS                        = 2335, // 1 byte
    M24LR64E_PROG_COMP_ENERGY_HARVEST_ADDRESS      = 2339  // 1 byte
} M24LR64E_ADDR_T;

typedef enum {
    M24LR64E_USER_MODE = 0x0,   // offers simple read/write access right
    M24LR64E_ROOT_MODE = 0x1    // offers password change access right
} m24lr64e_access_mode;

typedef enum {
    //      **********************************
    //      *  submit passWd *   no submit   *
    //b2,b1 *  Read * Write  *  Read * Write *
    // 00   *    1       1        1      0   *
    // 01   *    1       1        1      1   *
    // 10   *    1       1        0      0   *
    // 11   *    0       1        0      0   *
    //      **********************************
    m24lr64e_access_1110 = 0,
    m24lr64e_access_1111 = 1,
    m24lr64e_access_1100 = 2,
    m24lr64e_access_0111 = 3,
} sector_access_right;

typedef enum {
    //00 => no passwd protect
    //01 => passWd 1
    //10 => passWd 2
    //11 => passwd 3
    m24lr64e_no_password = 0,
    m24lr64e_password_1 = 1,
    m24lr64e_password_2 = 2,
    m24lr64e_password_3 = 3,
} sector_select_password;

/*
 * device context
 */
typedef struct _m24lr64e_context {
    mraa_i2c_context            i2c;
    uint8_t                     bus;
    uint8_t                     address;
    m24lr64e_access_mode    mode;
} *m24lr64e_context;

/**
 * M24LR64E Init function
 *
 * @param bus I2C bus to use
 * @param mode Access mode (user or root) to use
 */
m24lr64e_context m24lr64e_init(int bus, m24lr64e_access_mode mode);

/**
 * M24LR64E close function
 *
 * @param dev void pointer to sensor struct
 */
void m24lr64e_close(m24lr64e_context dev);

/**
 * Submits an I2C access password
 *
 * @param dev void pointer to sensor struct
 * @param passwd 4-byte access password
 */
upm_result_t m24lr64e_submit_password(m24lr64e_context dev, uint32_t password);

/**
 * Writes a new I2C password
 *
 * @param dev void pointer to sensor struct
 * @param passwd 4-byte access password
 */
upm_result_t m24lr64e_write_password(m24lr64e_context dev, uint32_t password);

/**
 * Sets a protection bit for a sector. Must be in the root mode
 *
 * @param dev void pointer to sensor struct
 * @param sectorNumber Sector whose protection you are modifying
 * @param protectEnable True if you are enabling protection
 * @param accessRight Access rights to set
 * @param passwd Password number to enable, if any
 */
upm_result_t m24lr64e_sector_protect_config(m24lr64e_context dev,
                                            uint32_t sector_number,
                                            bool protect_enable,
                                            sector_access_right access_right,
                                            sector_select_password password);

/**
 * Clears sector protection bits. Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 */
upm_result_t m24lr64e_clear_sector_protect(m24lr64e_context dev);

/**
 * Sets or clears a sector security status lock bit for a sector.
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param sectorNumber Sector whose SSS you want to modify
 * @param sockEnable True to set the bit, false to clear it
 */
upm_result_t m24lr64e_sector_write_lock_bit(m24lr64e_context dev,
                                            uint32_t sector_number,
                                            bool sock_enable);

/**
 * Returns a data storage family identifier (DSFID)
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param dsfid pointer to hold return value
 */
upm_result_t m24lr64e_get_dsfid(m24lr64e_context dev, uint8_t* dsfid);

/**
 * Returns an application family identifier (AFI)
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param afi pointer to hold return value
 */
upm_result_t m24lr64e_get_afi(m24lr64e_context dev, uint8_t* afi);

/**
 * Returns a unique ID.
 * Must be in the root mode.
 * Maintained to preserve compatibility with older code.
 *
 * @param dev void pointer to sensor struct
 * @param uid pointer to hold return value
 */
upm_result_t m24lr64e_get_uid(m24lr64e_context dev, uint8_t* uid);

/**
 * Returns the memory size
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param memory_size pointer to hold return value
 */
upm_result_t m24lr64e_get_memory_size(m24lr64e_context dev, uint32_t* memory_size);

/**
 * Sets all memory to 0, if permissions allow
 *
 * @param dev void pointer to sensor struct
 */
upm_result_t m24lr64e_clear_memory(m24lr64e_context dev);

/**
 * Writes a byte to the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to write to
 * @param data Data to write
 */
upm_result_t m24lr64e_write_byte(m24lr64e_context dev, uint32_t address,
                                 uint8_t data);

/**
 * Writes bytes to the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to write to
 * @param data Data to write
 * @param data Length of the data buffer
 */
upm_result_t m24lr64e_write_bytes(m24lr64e_context dev, uint32_t address,
                                  uint8_t* buffer, int len);

/**
 * Reads a byte from the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to read from
 * @param data holds return value
 */
upm_result_t m24lr64e_read_byte(m24lr64e_context dev, uint32_t address,
                                uint8_t* data);

/**
 * Reads multiple bytes from the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to read from
 * @param buffer Buffer to store data
 * @param len Number of bytes to read
 */
upm_result_t m24lr64e_read_bytes(m24lr64e_context dev, uint32_t address,
                                 uint8_t* buffer, int len);

#endif /* M24LR64E_H_ */
