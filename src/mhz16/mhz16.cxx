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

#include "mhz16.hpp"

using namespace upm;
using namespace std;

static const int defaultDelay = 100;     // max wait time for read

MHZ16::MHZ16(int uart)
{
  m_ttyFd = -1;

  if ( !(m_uart = mraa_uart_init(uart)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_uart_init() failed");
    }

  // This requires a recent MRAA (1/2015)
  const char *devPath = mraa_uart_get_dev_path(m_uart);

  if (!devPath)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_uart_get_dev_path() failed");
    }

  // now open the tty
  if ( (m_ttyFd = open(devPath, O_RDWR)) == -1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": open of " +
                               string(devPath) + " failed: " +
                               string(strerror(errno)));
    }
}

MHZ16::MHZ16(const std::string& uart_raw)
{
  m_ttyFd = -1;

  if ( !(m_uart = mraa_uart_init_raw(uart_raw.c_str())) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_uart_init_raw() failed");
    }

  // This requires a recent MRAA (1/2015)
  const char *devPath = mraa_uart_get_dev_path(m_uart);

  if (!devPath)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_uart_get_dev_path() failed");
    }

  // now open the tty
  if ( (m_ttyFd = open(devPath, O_RDWR)) == -1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": open of " +
                               string(devPath) + " failed: " +
                               string(strerror(errno)));
    }
}

MHZ16::~MHZ16()
{
  if (m_ttyFd != -1)
    close(m_ttyFd);

  mraa_deinit();
}

bool MHZ16::dataAvailable(unsigned int millis)
{
  if (m_ttyFd == -1)
    return false;

  struct timeval timeout;

  // no waiting
  timeout.tv_sec = 0;
  timeout.tv_usec = millis * 1000;

  fd_set readfds;

  FD_ZERO(&readfds);

  FD_SET(m_ttyFd, &readfds);

  if (select(m_ttyFd + 1, &readfds, NULL, NULL, &timeout) > 0)
    return true;                // data is ready

  return false;
}

int MHZ16::readData(char *buffer, int len)
{
  if (m_ttyFd == -1)
    return(-1);

  if (!dataAvailable(defaultDelay))
    return 0;               // timed out

  int rv = read(m_ttyFd, buffer, len);

  if (rv < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": read() failed: " +
                               string(strerror(errno)));
    }

  return rv;
}

int MHZ16::writeData(char *buffer, int len)
{
  if (m_ttyFd == -1)
    return(-1);

  // first, flush any pending but unread input
  tcflush(m_ttyFd, TCIFLUSH);

  int rv = write(m_ttyFd, buffer, len);

  if (rv < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": write() failed: " +
                               string(strerror(errno)));
    }

  tcdrain(m_ttyFd);

  return rv;
}

bool MHZ16::setupTty(speed_t baud)
{
  if (m_ttyFd == -1)
    return(false);

  struct termios termio;

  // get current modes
  tcgetattr(m_ttyFd, &termio);

  // setup for a 'raw' mode.  81N, no echo or special character
  // handling, such as flow control.
  cfmakeraw(&termio);

  // set our baud rates
  cfsetispeed(&termio, baud);
  cfsetospeed(&termio, baud);

  // make it so
  if (tcsetattr(m_ttyFd, TCSAFLUSH, &termio) < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": tcsetattr() failed: " +
                               string(strerror(errno)));
    }

  return true;
}

bool MHZ16::verifyPacket(uint8_t *pkt, int len)
{
  if (pkt[0] != 0xff || pkt[1] != 0x86)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": invalid packet header received");
    }

  return true;
}

bool MHZ16::getData()
{
  // the query command
  const unsigned char cmd[9] =
    {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

  writeData((char *)cmd, 9);

  // wait up to one second for a response
  if (!dataAvailable(1000))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Timed out waiting for response");
    }

  // read the packet
  unsigned char packet[9];
  int rv;

  if ((rv = readData((char *)packet, 9)) != 9)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": Invalid packet size read");
    }

  // will throw an exception if it fails
  verifyPacket(packet, sizeof(packet));

  // pull out the data and return it.
  gas = (packet[2] << 8) | packet[3];
  temp = packet[4] - 40;

  return true;
}

int MHZ16::getGas()
{
  return gas;
}

float MHZ16::getConcentration()
{
  getData();
  return gas;
}

float MHZ16::getTemperature()
{
  getData();
  return temp;
}

void MHZ16::calibrateZeroPoint()
{
  // the query command
  const unsigned char cmd[9] =
    {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};

  writeData((char *)cmd, 9);

  // no response
}

