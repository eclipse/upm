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

#include "m24lr64e.h"

// forward declarations - these were protected methods in original C++
// code.  Might need to expose them?
upm_result_t m24lr64e_eeprom_read_byte(m24lr64e_context dev,
                                       uint32_t address, uint8_t* data);
upm_result_t m24lr64e_eeprom_read_bytes(m24lr64e_context dev,
                                        uint32_t address,
                                        uint8_t* data, int len);
upm_result_t m24lr64e_eeprom_write_byte(m24lr64e_context dev,
                                        uint32_t address, uint8_t data);
upm_result_t m24lr64e_eeprom_write_bytes(m24lr64e_context dev,
                                         uint32_t address,
                                         uint8_t* data, int len);

m24lr64e_context m24lr64e_init(int bus, m24lr64e_access_mode mode){
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    m24lr64e_context dev =
      (m24lr64e_context)malloc(sizeof(struct _m24lr64e_context));

    if (!dev)
      return NULL;

    dev->bus = bus;
    dev->mode = mode;

    if (dev->mode == M24LR64E_USER_MODE)
        dev->address = M24LR64E_DEFAULT_I2C_ADDR;
    else
        dev->address = M24LR64E_DEFAULT_I2C_ADDR_E2;

    dev->i2c = mraa_i2c_init(dev->bus);
    if (mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS) {
        return NULL;
    }

    return dev;
}

void m24lr64e_close(m24lr64e_context dev){
    mraa_i2c_stop(dev->i2c);
    free(dev);
}

upm_result_t m24lr64e_submit_password(m24lr64e_context dev,
                                      uint32_t password){
    // this device actually uses two bytes to address a register
    const int pktLen = 11;
    uint8_t buf[pktLen];

    buf[0] = 0x09;
    buf[1] = 0x00;

    buf[2] = ((password >> 24) & 0xff);
    buf[3] = ((password >> 16) & 0xff);
    buf[4] = ((password >> 8) & 0xff);
    buf[5] = (password & 0xff);

    buf[6] = 0x09;

    // the password is written twice
    buf[7] = ((password >> 24) & 0xff);
    buf[8] = ((password >> 16) & 0xff);
    buf[9] = ((password >> 8) & 0xff);
    buf[10] = (password & 0xff);

    if (mraa_i2c_write(dev->i2c, buf, pktLen) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_write_password(m24lr64e_context dev, uint32_t password){
    const int pktLen = 11;
    uint8_t buf[pktLen];

    buf[0] = 0x09;
    buf[1] = 0x00;

    buf[2] = ((password >> 24) & 0xff);
    buf[3] = ((password >> 16) & 0xff);
    buf[4] = ((password >> 8) & 0xff);
    buf[5] = (password & 0xff);

    buf[6] = 0x07;

    // the password is written twice
    buf[7] = ((password >> 24) & 0xff);
    buf[8] = ((password >> 16) & 0xff);
    buf[9] = ((password >> 8) & 0xff);
    buf[10] = (password & 0xff);

    if(mraa_i2c_write(dev->i2c, buf, pktLen) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_sector_protect_config(m24lr64e_context dev,
                                            uint32_t sector_number,
                                            bool protect_enable,
                                            sector_access_right access_right,
                                            sector_select_password password){
    if(!protect_enable){
        m24lr64e_write_byte(dev, sector_number, 0x0);
    }
    else{
        m24lr64e_write_byte(dev, sector_number,
                            (protect_enable | (access_right << 1) |
                             (password << 2)));
    }
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_clear_sector_protect(m24lr64e_context dev){
    uint8_t buf[64]={0x0};
    return m24lr64e_eeprom_write_bytes(dev, 0, buf, 64);
}

upm_result_t m24lr64e_sector_write_lock_bit(m24lr64e_context dev,
                                                uint32_t sector_number,
                                                bool sock_enable){
    uint32_t sector_address = M24LR64E_SECTOR_SECURITY_STATUS_BASE_ADDR
        + (sector_number/8);

    uint8_t sector_bit = sector_number % 8;
    // byte read from the EEPROM into pre_status
    uint8_t pre_status;
    m24lr64e_eeprom_read_byte(dev, sector_address, &pre_status);

    bool status = (pre_status >> sector_bit) & 0x01;
    if(status != sock_enable){
        if(status == true){
            // Call to write byte function
            m24lr64e_write_byte(dev, sector_address, pre_status&(~(1<<sector_bit)));
        }
        else{
            // another call to write byte function
            m24lr64e_write_byte(dev, sector_address, pre_status|(1<<sector_bit));
        }
    }
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_get_dsfid(m24lr64e_context dev, uint8_t* dsfid){
    // EEPROM read byte function call
    return m24lr64e_eeprom_read_byte(dev, M24LR64E_DSFID_ADDRESS, dsfid);
}

upm_result_t m24lr64e_get_afi(m24lr64e_context dev, uint8_t* afi){
    // call to EEPROM read byte
    return m24lr64e_eeprom_read_byte(dev, M24LR64E_AFI_ADDRESS, afi);
}

upm_result_t m24lr64e_get_uid(m24lr64e_context dev, uint8_t* uid){
    //uint8_t* buffer;
    //uint8_t arr[M24LR64E_UID_LENGTH];
    //buffer = arr;
    // call to EEPROM read bytes
    m24lr64e_eeprom_read_bytes(dev, M24LR64E_UID_ADDRESS, uid,
                               M24LR64E_UID_LENGTH);
    // not so sure about this
//#warning "THIS NEEDS FIXING"
    //*uid = *buffer;
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_get_memory_size(m24lr64e_context dev,
                                      uint32_t* memory_size){
    uint32_t volume = 0x0;
    uint8_t temp_vol = 0x0;
    // us the EEPROM read byte function to calculate the volume
    if (m24lr64e_eeprom_read_byte(dev, M24LR64E_MEM_SIZE_ADDRESS,
                                 &temp_vol) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    volume = temp_vol;
    if (m24lr64e_eeprom_read_byte(dev, M24LR64E_MEM_SIZE_ADDRESS+1,
                                  &temp_vol) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    volume = volume<<8|temp_vol;
    temp_vol = 0x0;
    if (m24lr64e_eeprom_read_byte(dev, M24LR64E_MEM_SIZE_ADDRESS+2,
                                  &temp_vol) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    volume = volume << 8 | temp_vol;
    *memory_size = volume;
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_clear_memory(m24lr64e_context dev){
    int i=0;
    for(i = 0; i < M24LR64E_EEPROM_I2C_LENGTH; i++){
        // call to write byte function
        m24lr64e_write_byte(dev, i, 0x0);
    }
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_write_byte(m24lr64e_context dev, uint32_t address,
                                 uint8_t data){
    // call to EEPROM write byte
    return m24lr64e_eeprom_write_byte(dev, address, data);
}

upm_result_t m24lr64e_write_bytes(m24lr64e_context dev,
                                  uint32_t address, uint8_t* buffer, int len){
    // call to EEPROM write bytes
    return m24lr64e_eeprom_write_bytes(dev, address, buffer, len);
}

upm_result_t m24lr64e_read_byte(m24lr64e_context dev, uint32_t address,
                                uint8_t* data){
    // call to EEPROM read byte
    return m24lr64e_eeprom_read_byte(dev, address, data);
}

upm_result_t m24lr64e_read_bytes(m24lr64e_context dev, uint32_t address,
                                 uint8_t* buffer, int len){
    // call to EEPROM read bytes
    return m24lr64e_eeprom_write_bytes(dev, address, buffer, len);
}

upm_result_t m24lr64e_eeprom_write_byte(m24lr64e_context dev, uint32_t address,
                                        uint8_t data){
    int pkt_len = 3;
    uint8_t buf[pkt_len];
    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);
    buf[2] = data;

    if (mraa_i2c_write(dev->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_us(M24LR64E_I2C_WRITE_TIME*1000);
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_eeprom_write_bytes(m24lr64e_context dev,
                                         uint32_t address, uint8_t* data,
                                         int len){
    uint32_t pkt_len = 2 + len;
    uint8_t buf[pkt_len];

    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);

    int i = 0;
    for (i=0; i<len; i++)
        buf[2+i] = data[i];

    if (mraa_i2c_write(dev->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    upm_delay_us(M24LR64E_I2C_WRITE_TIME*1000);
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_eeprom_read_byte(m24lr64e_context dev, uint32_t address,
                                       uint8_t* data){
    uint32_t pkt_len = 2;
    uint8_t buf[pkt_len];

    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);

    if (mraa_i2c_write(dev->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    pkt_len = 1;
    uint8_t abuf[pkt_len];
    abuf[0] = 0;

    if (mraa_i2c_read(dev->i2c, abuf, pkt_len) != pkt_len){
        return UPM_ERROR_OPERATION_FAILED;
    }
    *data = abuf[0];
    return UPM_SUCCESS;
}

upm_result_t m24lr64e_eeprom_read_bytes(m24lr64e_context dev, uint32_t address,
                                        uint8_t* data, int len){
    uint32_t pkt_len = 2;
    uint8_t buf[pkt_len];

    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);

    if (mraa_i2c_write(dev->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(mraa_i2c_read(dev->i2c, data, len) != len){
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}
