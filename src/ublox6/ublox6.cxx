/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include "ublox6.h"

using namespace upm;
using namespace std;

Ublox6::Ublox6(int uart)
{
  m_ttyFd = -1;

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
      string err = __FUNCTION__;
      err += ": open of " + std::string(devPath) + " failed: " + 
        std::string(strerror(errno));

      throw std::runtime_error(err);
      return;
    }
}

Ublox6::~Ublox6()
{
  if (m_ttyFd != -1)
    close(m_ttyFd);
}

bool Ublox6::dataAvailable()
{
  if (m_ttyFd == -1)
    return false;

  struct timeval timeout;

  // no waiting
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  int nfds;  
  fd_set readfds;

  FD_ZERO(&readfds);

  FD_SET(m_ttyFd, &readfds);
  
  if (select(m_ttyFd + 1, &readfds, NULL, NULL, &timeout) > 0)
    return true;                // data is ready
  else
    return false;
}

int Ublox6::readData(char *buffer, int len)
{
  if (m_ttyFd == -1)
    return(-1);

  int rv = read(m_ttyFd, buffer, len);

  if (rv < 0)
    {
      string err = string(__FUNCTION__) + ": read failed: " + 
        string(strerror(errno));

      throw std::runtime_error(err);
      return rv;
    }

  return rv;
}

int Ublox6::writeData(char * buffer, int len)
{
  if (m_ttyFd == -1)
    return(-1);

  int rv = write(m_ttyFd, buffer, len);

  if (rv < 0)
    {
      string err = string(__FUNCTION__) + ": write failed: " + 
        string(strerror(errno));

      throw std::runtime_error(err);
      return rv;
    }

  tcdrain(m_ttyFd);

  return rv;
}

bool Ublox6::setupTty(speed_t baud)
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
  int rv;
  if ( (rv = tcsetattr(m_ttyFd, TCSAFLUSH, &termio)) < 0)
    {
      string err = string(__FUNCTION__) + ": tcsetattr failed: " + 
        string(strerror(errno));

      throw std::runtime_error(err);
      return false;
    }

  return true;
}
