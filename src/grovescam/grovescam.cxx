/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Seeed Studio for a working arduino sketch
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

#include <iostream>
#include <string>
#include <stdexcept>
#include <errno.h>

#include "grovescam.h"

using namespace upm;
using namespace std;

static const int maxRetries = 100;

GROVESCAM::GROVESCAM(int uart, uint8_t camAddr)
{
  m_ttyFd = -1;

  // save our shifted camera address, we'll need it a lot
  m_camAddr = (camAddr << 5);

  m_picTotalLen = 0;

  if ( !(m_uart = mraa_uart_init(uart)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_uart_init() failed");
      return;
    }

  // This requires a recent MRAA (1/2015)
  const char *devPath = mraa_uart_get_dev_path(m_uart);

  if (!devPath)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_uart_get_dev_path() failed");
      return;
    }

  // now open the tty
  if ( (m_ttyFd = open(devPath, O_RDWR)) == -1)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": open of " + 
                               string(devPath) + " failed:" +
                               string(strerror(errno)));
      return;
    }
}

GROVESCAM::~GROVESCAM()
{
  if (m_ttyFd != -1)
    close(m_ttyFd);
}

bool GROVESCAM::dataAvailable(unsigned int millis)
{
  if (m_ttyFd == -1)
    return false;

  struct timeval timeout;

  if (millis == 0) 
    {
      // no waiting
      timeout.tv_sec = 0;
      timeout.tv_usec = 0;
    }
  else 
    {
      timeout.tv_sec = millis / 1000;
      timeout.tv_usec = (millis % 1000) * 1000;
    }

  int nfds;  
  fd_set readfds;

  FD_ZERO(&readfds);

  FD_SET(m_ttyFd, &readfds);
  
  if (select(m_ttyFd + 1, &readfds, NULL, NULL, &timeout) > 0)
    return true;                // data is ready
  else
    return false;
}

int GROVESCAM::readData(uint8_t *buffer, int len)
{
  if (m_ttyFd == -1)
    return(-1);

  int rv = read(m_ttyFd, (char *)buffer, len);

  if (rv < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": read() failed: " +
                               string(strerror(errno)));
      return rv;
    }

  return rv;
}

int GROVESCAM::writeData(uint8_t *buffer, int len)
{
  if (m_ttyFd == -1)
    return(-1);

  // first, flush any pending but unread input

  tcflush(m_ttyFd, TCIFLUSH);

  int rv = write(m_ttyFd, (char *)buffer, len);

  if (rv < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": write() failed: " +
                               string(strerror(errno)));
      return rv;
    }

  tcdrain(m_ttyFd);

  return rv;
}

bool GROVESCAM::setupTty(speed_t baud)
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
      return false;
    }

  return true;
}

void GROVESCAM::drainInput()
{
  uint8_t ch;

  while (dataAvailable(0))
    readData(&ch, 1);
}

bool GROVESCAM::init()
{
  const unsigned int pktLen = 6;
  uint8_t cmd[pktLen] = {0xaa, 0x0d|m_camAddr, 0x00, 0x00, 0x00, 0x00};
  uint8_t resp[pktLen];
  int retries = 0;

  while (true)
    {
      if (retries++ > maxRetries)
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": maximum retries exceeded");
          return false;
        }

      writeData(cmd, pktLen);

      if (!dataAvailable(500))
        continue;

      if (readData(resp, pktLen) != pktLen)
        continue;

      if (resp[0] == 0xaa 
          && resp[1] == (0x0e | m_camAddr) 
          && resp[2] == 0x0d 
          && resp[4] == 0 
          && resp[5] == 0)
        {
          if (readData(resp, pktLen) != pktLen)
            continue;
          else
            {
              if (resp[0] == 0xaa 
                  && resp[1] == (0x0d | m_camAddr) 
                  && resp[2] == 0 
                  && resp[3] == 0 
                  && resp[4] == 0 
                  && resp[5] == 0) 
                break;
            }
        }
    }

  cmd[1] = 0x0e | m_camAddr;
  cmd[2] = 0x0d;
  writeData(cmd, pktLen);

  return true;
}

bool GROVESCAM::preCapture(PIC_FORMATS_T fmt)
{
  const unsigned int pktLen = 6;
  uint8_t cmd[pktLen] = { 0xaa, 0x01 | m_camAddr, 0x00, 0x07, 0x00, fmt };
  uint8_t resp[pktLen];
  int retries = 0;

  while (true)
    {
      if (retries++ > maxRetries)
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": maximum retries exceeded");
          return false;
        }

      drainInput();

      writeData(cmd, pktLen);

      if (!dataAvailable(100))
        continue;

      if (readData(resp, pktLen) != pktLen)
        continue;

      if (resp[0] == 0xaa 
          && resp[1] == (0x0e | m_camAddr) 
          && resp[2] == 0x01 
          && resp[4] == 0 
          && resp[5] == 0) break;
    }

  return true;
}

bool GROVESCAM::doCapture()
{
  const unsigned int pktLen = 6;
  uint8_t cmd[pktLen] = { 0xaa, 0x06 | m_camAddr, 0x08, MAX_PKT_LEN & 0xff, 
                          (MAX_PKT_LEN >> 8) & 0xff, 0};
  uint8_t resp[pktLen];
  int retries = 0;
  
  m_picTotalLen = 0;

  while (true)
    {
      if (retries++ > maxRetries)
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": maximum retries exceeded");
          return false;
        }

      drainInput();
      writeData(cmd, pktLen);
      usleep(100000);

      if (!dataAvailable(100))
        continue;

      if (readData(resp, pktLen) != pktLen)
        continue;

      if (resp[0] == 0xaa 
          && resp[1] == (0x0e | m_camAddr) 
          && resp[2] == 0x06 
          && resp[4] == 0 
          && resp[5] == 0)
        break;
    }

  cmd[1] = 0x05 | m_camAddr;
  cmd[2] = 0;
  cmd[3] = 0;
  cmd[4] = 0;
  cmd[5] = 0;

  retries = 0;
  while (true)
    {
      if (retries++ > maxRetries)
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": maximum retries exceeded");
          return false;
        }

      drainInput();
      writeData(cmd, pktLen);
      if (readData(resp, pktLen) != pktLen)
        continue;

      if (resp[0] == 0xaa 
          && resp[1] == (0x0e | m_camAddr) 
          && resp[2] == 0x05 
          && resp[4] == 0 
          && resp[5] == 0)
        break;
    }

  cmd[1] = 0x04 | m_camAddr;
  cmd[2] = 0x01;

  retries = 0;
  while (true)
    {
      if (retries++ > maxRetries)
        {
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": maximum retries exceeded");
          return false;
        }

      drainInput();
      writeData(cmd, 6);

      if (readData(resp, pktLen) != pktLen)
        continue;

      if (resp[0] == 0xaa 
          && resp[1] == (0x0e | m_camAddr) 
          && resp[2] == 0x04 
          && resp[4] == 0 
          && resp[5] == 0)
        {
          if (!dataAvailable(1000))
            continue;

          if (readData(resp, pktLen) != pktLen)
            continue;

          if (resp[0] == 0xaa
              && resp[1] == (0x0a | m_camAddr)
              && resp[2] == 0x01)
            {
              m_picTotalLen = (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
              break;
            }
        }
    }

  return true;
}

bool GROVESCAM::storeImage(const char *fname)
{
  if (!fname)
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": filename is NULL");
      return false;
    }

  if (!m_picTotalLen)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                    ": Picture length is zero, you need to capture first.");

      return false;
    }

  FILE *file = fopen(fname, "wb");

  if (!file)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": fopen() failed: " +
                               string(strerror(errno)));
      return false;
    }
  
  /// let the games begin...
  const unsigned int pktLen = 6;
  unsigned int pktCnt = (m_picTotalLen) / (MAX_PKT_LEN - 6);
  if ((m_picTotalLen % (MAX_PKT_LEN-6)) != 0) 
    pktCnt += 1;
  
  uint8_t cmd[pktLen] = { 0xaa, 0x0e | m_camAddr, 0x00, 0x00, 0x00, 0x00 };
  uint8_t pkt[MAX_PKT_LEN];
  int retries = 0;
  
  for (unsigned int i = 0; i < pktCnt; i++)
    {
      cmd[4] = i & 0xff;
      cmd[5] = (i >> 8) & 0xff;
      
      retries = 0;

    retry:

      usleep(10000);

      drainInput();
      writeData(cmd, pktLen);

      if (!dataAvailable(1000))
        {
          if (retries++ > maxRetries)
            {
              throw std::runtime_error(std::string(__FUNCTION__) +
                                       ": timeout, maximum retries exceeded");
              return false;
            }
          goto retry;
        }

      uint16_t cnt = readData(pkt, MAX_PKT_LEN);
      
      unsigned char sum = 0;
      for (int y = 0; y < cnt - 2; y++)
        {
          sum += pkt[y];
        }
      if (sum != pkt[cnt-2])
        {
          if (retries++ <= maxRetries)
            goto retry;
          else
            {
              throw std::runtime_error(std::string(__FUNCTION__) +
                                       ": cksum error, maximum retries exceeded");
              return false;
            }
        }

      fwrite((const uint8_t *)&pkt[4], cnt - 6, 1, file);
    }

  cmd[4] = 0xf0;
  cmd[5] = 0xf0;
  writeData(cmd, pktLen);

  fclose(file);

  // reset the pic length to 0 for another run.
  m_picTotalLen = 0;

  return true;
}
