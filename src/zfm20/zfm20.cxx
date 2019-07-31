/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include "zfm20.hpp"

using namespace upm;
using namespace std;

static const int defaultDelay = 100;     // max wait time for read

ZFM20::ZFM20(int uart, int baud): m_uart(uart)
{
  // Set the default password and address
  setPassword(ZFM20_DEFAULT_PASSWORD);
  setAddress(ZFM20_DEFAULT_ADDRESS);

  initClock();

  if (!setupTty(baud))
    throw std::runtime_error(std::string(__FUNCTION__) +
            ": failed to set baud rate to " + std::to_string(baud));
}


ZFM20::ZFM20(std::string uart_raw, int baud) : m_uart(uart_raw)
{
  // Set the default password and address
  setPassword(ZFM20_DEFAULT_PASSWORD);
  setAddress(ZFM20_DEFAULT_ADDRESS);

  initClock();

  if (!setupTty(baud))
    throw std::runtime_error(std::string(__FUNCTION__) +
            ": failed to set baud rate to " + std::to_string(baud));
}

int ZFM20::readData(char *buffer, int len)
{
  if (!m_uart.dataAvailable(defaultDelay))
    return 0;               // timed out

  int rv = m_uart.read(buffer, len);

  if (rv < 0)
    throw std::runtime_error(std::string(__FUNCTION__) +
            ": Uart::read() failed: " + string(strerror(errno)));

  return rv;
}

int ZFM20::writeData(char *buffer, int len)
{
  int rv = m_uart.write(buffer, len);

  if (rv < 0)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Uart::write() failed: " +
                               string(strerror(errno)));

  if (rv == 0)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Uart::write() failed, no bytes written");

  return rv;
}

bool ZFM20::setupTty(uint32_t baud)
{
    return m_uart.setBaudRate(baud) == mraa::SUCCESS;
}

int ZFM20::writeCmdPacket(uint8_t *pkt, int len)
{
  uint8_t rPkt[ZFM20_MAX_PKT_LEN];

  rPkt[0] = ZFM20_START1;             // header bytes
  rPkt[1] = ZFM20_START2;

  rPkt[2] = (m_address >> 24) & 0xff; // address
  rPkt[3] = (m_address >> 16) & 0xff;
  rPkt[4] = (m_address >> 8) & 0xff;
  rPkt[5] = m_address & 0xff;

  rPkt[6] = PKT_COMMAND;

  rPkt[7] = ((len + 2) >> 8) & 0xff;  // length (+ len bytes)
  rPkt[8] = (len + 2) & 0xff;

  // compute the starting checksum
  uint16_t cksum = rPkt[7] + rPkt[8] + PKT_COMMAND;

  int j = 9;
  for (int i=0; i<len; i++)
    {
      rPkt[j] = pkt[i];
      cksum += rPkt[j];
      j++;
    }

  rPkt[j++] = (cksum >> 8) & 0xff;    // store the cksum
  rPkt[j++] = cksum & 0xff;

  return writeData((char *)rPkt, j);
}

void ZFM20::initClock()
{
  m_clock = upm_clock_init();
}

uint32_t ZFM20::getMillis()
{
  return upm_elapsed_ms(&m_clock);
}

bool ZFM20::verifyPacket(uint8_t *pkt, int len)
{
  // verify packet header
  if (pkt[0] != ZFM20_START1 || pkt[1] != ZFM20_START2)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Invalid packet header");

  // check the ack byte
  if (pkt[6] != PKT_ACK)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Invalid ACK code");

  return true;
}

bool ZFM20::getResponse(uint8_t *pkt, int len)
{
  char buf[ZFM20_MAX_PKT_LEN];

  initClock();

  int idx = 0;
  int timer = 0;
  int rv;

  while (idx < len)
    {
      // wait for some data
      if (!m_uart.dataAvailable(100))
        {
          timer += getMillis();
          if (timer > ZFM20_TIMEOUT)
              throw std::runtime_error(std::string(__FUNCTION__) +
                                       ": Timed out waiting for packet");

          continue;
        }

      if ((rv = readData(buf, ZFM20_MAX_PKT_LEN)) == 0)
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": readData() failed, no data returned");

      // copy it into the user supplied buffer
      for (int i=0; i<rv; i++)
        {
          pkt[idx++] = buf[i];
          if (idx >= len)
            break;
        }
    }

  // now verify it.
  return verifyPacket(pkt, len);
}

bool ZFM20::verifyPassword()
{
  const int pktLen = 5;
  uint8_t pkt[pktLen] = {CMD_VERIFY_PASSWORD,
                         static_cast<uint8_t>((m_password >> 24) & 0xff),
                         static_cast<uint8_t>((m_password >> 16) & 0xff),
                         static_cast<uint8_t>((m_password >> 8) & 0xff),
                         static_cast<uint8_t>(m_password & 0xff) };

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);


  return true;
}

int ZFM20::getNumTemplates()
{
  const int pktLen = 1;
  uint8_t pkt[pktLen] = {CMD_GET_TMPL_COUNT};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 14;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  // check confirmation code
  if (rPkt[9] != 0x00)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Invalid confirmation code");
  return ((rPkt[10] << 8) | rPkt[11]);
}

bool ZFM20::setNewPassword(uint32_t pwd)
{
  const int pktLen = 5;
  uint8_t pkt[pktLen] = {CMD_SET_PASSWORD,
                         static_cast<uint8_t>((pwd >> 24) & 0xff),
                         static_cast<uint8_t>((pwd >> 16) & 0xff),
                         static_cast<uint8_t>((pwd >> 8) & 0xff),
                         static_cast<uint8_t>(pwd & 0xff) };

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  // check confirmation code
  if (rPkt[9] != 0x00)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Invalid confirmation code");

  m_password = pwd;

  return true;
}

bool ZFM20::setNewAddress(uint32_t addr)
{
  const int pktLen = 5;
  uint8_t pkt[pktLen] = {CMD_SET_ADDRESS,
                         static_cast<uint8_t>((addr >> 24) & 0xff),
                         static_cast<uint8_t>((addr >> 16) & 0xff),
                         static_cast<uint8_t>((addr >> 8) & 0xff),
                         static_cast<uint8_t>(addr & 0xff) };

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  // check confirmation code
  if (rPkt[9] != 0x00)
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Invalid confirmation code");

  m_address = addr;

  return true;
}

uint8_t ZFM20::generateImage()
{
  const int pktLen = 1;
  uint8_t pkt[pktLen] = {CMD_GEN_IMAGE};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  return rPkt[9];
}

uint8_t ZFM20::image2Tz(int slot)
{
  if (slot != 1 && slot != 2)
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": slot must be 1 or 2");

  const int pktLen = 2;
  uint8_t pkt[pktLen] = {CMD_IMG2TZ,
                         static_cast<uint8_t>(slot & 0xff)};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  return rPkt[9];
}

uint8_t ZFM20::createModel()
{
  const int pktLen = 1;
  uint8_t pkt[pktLen] = {CMD_REGMODEL};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  return rPkt[9];
}

uint8_t ZFM20::storeModel(int slot, uint16_t id)
{
  if (slot != 1 && slot != 2)
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": slot must be 1 or 2");

  const int pktLen = 4;
  uint8_t pkt[pktLen] = {CMD_STORE,
                         static_cast<uint8_t>(slot & 0xff),
                         static_cast<uint8_t>((id >> 8) & 0xff),
                         static_cast<uint8_t>(id & 0xff)};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  return rPkt[9];
}

uint8_t ZFM20::deleteModel(uint16_t id)
{
  const int pktLen = 5;
  uint8_t pkt[pktLen] = {CMD_DELETE_TMPL,
                         static_cast<uint8_t>((id >> 8) & 0xff),
                         static_cast<uint8_t>(id & 0xff),
                         0x00,
                         0x01};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  return rPkt[9];
}

uint8_t ZFM20::deleteDB()
{
  const int pktLen = 1;
  uint8_t pkt[pktLen] = {CMD_EMPTYDB};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 12;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  return rPkt[9];
}

uint8_t ZFM20::search(int slot, uint16_t &id, uint16_t &score)
{
  id = 0;
  score = 0;

  if (slot != 1 && slot != 2)
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": slot must be 1 or 2");

  // search from page 0x0000 to page 0x00a3
  const int pktLen = 6;
  uint8_t pkt[pktLen] = {CMD_SEARCH,
                         static_cast<uint8_t>(slot & 0xff),
                         0x00,
                         0x00,
                         0x00,
                         0xa3};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 16;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  // if it was found, extract the location and the score
  if (rPkt[9] == ERR_OK)
    {
      id = ((rPkt[10] << 8) & 0xff) | (rPkt[11] & 0xff);
      score = ((rPkt[12] << 8) & 0xff) | (rPkt[13] & 0xff);
    }

  return rPkt[9];
}

uint8_t ZFM20::match(uint16_t &score)
{
  score = 0;

  const int pktLen = 1;
  uint8_t pkt[pktLen] = {CMD_MATCH};

  writeCmdPacket(pkt, pktLen);

  // now read a response
  const int rPktLen = 14;
  uint8_t rPkt[rPktLen];

  getResponse(rPkt, rPktLen);

  score = ((rPkt[10] << 8) & 0xff) | (rPkt[11] & 0xff);

  return rPkt[9];
}
