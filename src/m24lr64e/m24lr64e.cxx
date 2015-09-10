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

#include <unistd.h>
#include <math.h>
#include <iostream>
#include <string>

#include "m24lr64e.h"

using namespace upm;
using namespace std;


M24LR64E::M24LR64E(int bus, AccessMode mode):
  m_i2c(bus)
{
  if (mode == USER_MODE)
    m_addr = M24LR64E_DEFAULT_I2C_ADDR;
  else
    m_addr = M24LR64E_DEFAULT_I2C_ADDR_E2;

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }
}

M24LR64E::~M24LR64E()
{
}

bool M24LR64E::submitPasswd(uint32_t passwd)
{
  // this device actually uses two bytes to address a register
  const int pktLen = 11;
  uint8_t buf[pktLen];

  buf[0] = 0x09;
  buf[1] = 0x00;

  buf[2] = ((passwd >> 24) & 0xff);
  buf[3] = ((passwd >> 16) & 0xff);
  buf[4] = ((passwd >> 8) & 0xff);
  buf[5] = (passwd & 0xff);

  buf[6] = 0x09;

  // the password is written twice
  buf[7] = ((passwd >> 24) & 0xff);
  buf[8] = ((passwd >> 16) & 0xff);
  buf[9] = ((passwd >> 8) & 0xff);
  buf[10] = (passwd & 0xff);
  
  if (m_i2c.write(buf, pktLen))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.write() failed");
      return false;
    }

  return true;
}

bool M24LR64E::writePasswd(uint32_t passwd)
{
  const int pktLen = 11;
  uint8_t buf[pktLen];

  buf[0] = 0x09;
  buf[1] = 0x00;

  buf[2] = ((passwd >> 24) & 0xff);
  buf[3] = ((passwd >> 16) & 0xff);
  buf[4] = ((passwd >> 8) & 0xff);
  buf[5] = (passwd & 0xff);

  buf[6] = 0x07;

  // the password is written twice
  buf[7] = ((passwd >> 24) & 0xff);
  buf[8] = ((passwd >> 16) & 0xff);
  buf[9] = ((passwd >> 8) & 0xff);
  buf[10] = (passwd & 0xff);
  
  if (m_i2c.write(buf, pktLen))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.write() failed");
      return false;
    }

  return true;
}

void M24LR64E::sectorProtectConfig(unsigned int sectorNumber, 
                                   bool protectEnable, 
                                   SectorAccessRight accessRight, 
                                   SectorSelectPassWd passwd)
{
  if(!protectEnable) {
    EEPROM_Write_Byte(sectorNumber,0x0);
  } else {
    EEPROM_Write_Byte(sectorNumber,
                      protectEnable | (accessRight<<1) |(passwd<<2));
  }
}

void M24LR64E::clearSectorProtect(void)
{
  uint8_t buf[64]={0x0};
  EEPROM_Write_Bytes(0, buf, 64);
}


void M24LR64E::sectorWriteLockBit(unsigned int sectorNumber, 
                                  bool sockEnable)
{
  unsigned int sectorAddress = SECTOR_SECURITY_STATUS_BASE_ADDR 
    + (sectorNumber/8);
  uint8_t sectorBit = sectorNumber % 8;
  uint8_t preStatus = EEPROM_Read_Byte(sectorAddress);
  
  bool status = (preStatus >> sectorBit) & 0x01;
  if(status != sockEnable) {
    if(status == true) {
      writeByte(sectorAddress,preStatus&(~(1<<sectorBit)));
    } else {
      writeByte(sectorAddress,preStatus|(1<<sectorBit));
    }
  }
}

uint8_t M24LR64E::getDSFID()
{
  return EEPROM_Read_Byte(DSFID_ADDR);
}

uint8_t M24LR64E::getAFI()
{
  return EEPROM_Read_Byte(AFI_ADDR);
}

uint8_t *M24LR64E::getUID()
{
  uint8_t* buffer = new uint8_t[UID_LENGTH];
  EEPROM_Read_Bytes(UID_ADDR, buffer, UID_LENGTH);

  return buffer;
}

uint32_t M24LR64E::getMemorySize()
{
  uint32_t volume = 0x0;
  volume = EEPROM_Read_Byte(MEM_SIZE_ADDR); 
  volume = volume<<8|EEPROM_Read_Byte(MEM_SIZE_ADDR+1);
  volume = volume<<8|EEPROM_Read_Byte(MEM_SIZE_ADDR+2);
  return volume;
}

void M24LR64E::clearMemory()
{
  for(int i = 0; i < EEPROM_I2C_LENGTH; i++){
    writeByte(i,0x0);
  }
}

mraa::Result M24LR64E::writeByte(unsigned int address, uint8_t data)
{
  return EEPROM_Write_Byte(address, data);
}

mraa::Result M24LR64E::writeBytes(unsigned int address, uint8_t* buffer, int len)
{
  return EEPROM_Write_Bytes(address, buffer, len);
}

uint8_t M24LR64E::readByte(unsigned int address)
{
  return EEPROM_Read_Byte(address);
}

int M24LR64E::readBytes(unsigned int address, uint8_t* buffer, int len)
{
  return EEPROM_Read_Bytes(address, buffer, len);
}

mraa::Result M24LR64E::EEPROM_Write_Byte(unsigned int address, uint8_t data)
{
  const int pktLen = 3;
  uint8_t buf[pktLen];
  mraa::Result rv;
  
  buf[0] = ((address >> 8) & 0xff);
  buf[1] = (address & 0xff);
  buf[2] = data;

  if ((rv = m_i2c.write(buf, pktLen)))
    throw std::runtime_error(std::string(__FUNCTION__) +
                             ": I2c.write() failed");

  usleep(I2C_WRITE_TIME * 1000);
  return rv;
}

mraa::Result M24LR64E::EEPROM_Write_Bytes(unsigned int address, uint8_t* data,
                                  int len)
{
  const int pktLen = 2 + len;
  uint8_t buf[pktLen];
  mraa::Result rv;
  
  buf[0] = ((address >> 8) & 0xff);
  buf[1] = (address & 0xff);

  for (int i=0; i<len; i++)
    buf[2+i] = data[i];

  if ((rv = m_i2c.write(buf, pktLen)))
    throw std::runtime_error(std::string(__FUNCTION__) +
                             ": I2c.write() failed");

  usleep(I2C_WRITE_TIME * 1000);

  return rv;
}

uint8_t M24LR64E::EEPROM_Read_Byte(unsigned int address)
{
  const int apktLen = 2;
  uint8_t abuf[apktLen];

  abuf[0] = ((address >> 8) & 0xff);
  abuf[1] = (address & 0xff);
  
  if (m_i2c.write(abuf, apktLen))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.write() failed");
      return 0x00;
    }

  const int pktLen = 1;
  uint8_t buf[apktLen];

  buf[0] = 0;
  
  if (m_i2c.read(buf, pktLen) != pktLen)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.write() failed");
      return 0x00;
    }
  
  return buf[0];
}

int M24LR64E::EEPROM_Read_Bytes(unsigned int address, 
                                         uint8_t* buffer, int len)
{
  const int apktLen = 2;
  uint8_t abuf[apktLen];

  abuf[0] = ((address >> 8) & 0xff);
  abuf[1] = (address & 0xff);
  
  if (m_i2c.write(abuf, apktLen))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.write() failed");
      return false;
    }

  int rv = m_i2c.read(buffer, len);
  if (rv != len)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.read() failed");
    }

  return rv;
}
