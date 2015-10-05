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

#include <iostream>
#include <string>
#include <stdexcept>

#include "hmtrp.h"

using namespace upm;
using namespace std;

static const int defaultDelay = 100;     // max wait time for read

// protocol start code
const uint8_t HMTRP_START1 = 0xaa;
const uint8_t HMTRP_START2 = 0xfa;

HMTRP::HMTRP(int uart)
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
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": open of " + 
                               string(devPath) + " failed: " +
                               string(strerror(errno)));
      return;
    }
}

HMTRP::~HMTRP()
{
  if (m_ttyFd != -1)
    close(m_ttyFd);
}

bool HMTRP::dataAvailable(unsigned int millis)
{
  if (m_ttyFd == -1)
    return false;

  struct timeval timeout;

  timeout.tv_sec = 0;
  timeout.tv_usec = millis * 1000;

  int nfds;  
  fd_set readfds;

  FD_ZERO(&readfds);

  FD_SET(m_ttyFd, &readfds);
  
  if (select(m_ttyFd + 1, &readfds, NULL, NULL, &timeout) > 0)
    return true;                // data is ready
  else
    return false;
}

int HMTRP::readData(char *buffer, int len, int millis)
{
  if (m_ttyFd == -1)
    return(-1);

  // if specified, wait to see if input shows up, otherwise block
  if (millis >= 0)
    {
      if (!dataAvailable(millis))
        return 0;               // timed out
    }

  int rv = read(m_ttyFd, buffer, len);

  if (rv < 0)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": read() failed: " +
                               string(strerror(errno)));
      return rv;
    }

  return rv;
}

int HMTRP::writeData(char *buffer, int len)
{
  if (m_ttyFd == -1)
    return(-1);

  int rv = write(m_ttyFd, buffer, len);

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

bool HMTRP::setupTty(speed_t baud)
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

bool HMTRP::checkOK()
{
  char buf[4];

  int rv = readData(buf, 4, defaultDelay);
  
  if (rv != 4)
    {
      cerr << __FUNCTION__ << ": failed to receive OK response, rv = " 
           << rv << ", expected 4" << endl;
      return false;
    }

  // looking for "OK\r\n"
  if (buf[0] == 'O' && buf[1] == 'K' &&
      buf[2] == '\r' && buf[3] == '\n')
    return true;
  else
    return false;
}

bool HMTRP::reset()
{
  char pkt[3];

  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = RESET;

  writeData(pkt, 3);

  return checkOK();
}

bool HMTRP::getConfig(uint32_t *freq, uint32_t *dataRate, 
                      uint16_t *rxBandwidth, uint8_t *modulation, 
                      uint8_t *txPower, uint32_t *uartBaud)
{
  char pkt[3];
  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = GET_CONFIG;

  writeData(pkt, 3);
  usleep(100000);

  // now read back a 16 byte response
  char buf[16];
  int rv = readData(buf, 16, defaultDelay);

  if (rv != 16)
    {
      cerr << __FUNCTION__ << ": failed to receive correct response: rv = " 
           << rv << ", expected 16" << endl;
      return false;
    }

  // now decode
  if (freq)
    {
      *freq = ( ((buf[0] & 0xff) << 24) |
                ((buf[1] & 0xff) << 16) |
                ((buf[2] & 0xff) << 8)  |
                 (buf[3] & 0xff) );
    }

  if (dataRate)
    {
      *dataRate = ( ((buf[4] & 0xff) << 24) |
                    ((buf[5] & 0xff) << 16) |
                    ((buf[6] & 0xff) << 8)  |
                     (buf[7] & 0xff) );
    }

  if (rxBandwidth)
    {
      *rxBandwidth = ( ((buf[8] & 0xff) << 8) |
                        (buf[9] & 0xff) );
    }

  if (modulation)
    {
      *modulation = buf[10] & 0xff;
    }

  if (txPower)
    {
      *txPower = buf[11] & 0xff;
    }

  if (uartBaud)
    {
      *uartBaud = ( ((buf[12] & 0xff) << 24) |
                    ((buf[13] & 0xff) << 16) |
                    ((buf[14] & 0xff) << 8)  |
                     (buf[15] & 0xff) );
    } 

  return true;
}

bool HMTRP::setFrequency(uint32_t freq)
{
  char pkt[7];

  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = SET_FREQUENCY;

  pkt[3] = ( ((freq & 0xff000000) >> 24) & 0xff );
  pkt[4] = ( ((freq & 0x00ff0000) >> 16) & 0xff );
  pkt[5] = ( ((freq & 0x0000ff00) >> 8)  & 0xff );
  pkt[6] = ( (freq & 0x000000ff) & 0xff );

  writeData(pkt, 7);

  return checkOK();
}

bool HMTRP::setRFDataRate(uint32_t rate)
{
  //  Valid values are between 1200-115200

  if (rate < 1200 || rate > 115200)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": Valid rate values are between 1200-115200");
      return false;
    }

  char pkt[7];

  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = SET_RF_DATARATE;

  pkt[3] = ( ((rate & 0xff000000) >> 24) & 0xff );
  pkt[4] = ( ((rate & 0x00ff0000) >> 16) & 0xff );
  pkt[5] = ( ((rate & 0x0000ff00) >> 8)  & 0xff );
  pkt[6] = ( (rate & 0x000000ff) & 0xff );

  writeData(pkt, 7);

  return checkOK();
}

bool HMTRP::setRXBandwidth(uint16_t rxBand)
{
  //  Valid values are between 30-620 (in Khz)

  if (rxBand < 30 || rxBand > 620)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": Valid rxBand values are between 30-620");
      return false;
    }
        
  char pkt[5];

  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = SET_RX_BW;

  pkt[3] = ( ((rxBand & 0xff00) >> 8) & 0xff );
  pkt[4] = ( rxBand & 0xff );

  writeData(pkt, 5);

  return checkOK();
}

bool HMTRP::setFrequencyModulation(uint8_t modulation)
{
  //  Valid values are between 10-160 (in Khz)

  if (modulation < 10 || modulation > 160)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": Valid modulation values are between 10-160");
      return false;
    }
        
  char pkt[4];

  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = SET_FREQ_MODULATION;

  pkt[3] = modulation;

  writeData(pkt, 4);

  return checkOK();
}

bool HMTRP::setTransmitPower(uint8_t power)
{
  //  Valid values are between 0-7

  if (power > 7)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": Valid power values are between 0-7");
      return false;
    }
        
  char pkt[4];

  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = SET_TX_POWER;

  pkt[3] = power;

  writeData(pkt, 4);

  return checkOK();
}

bool HMTRP::setUARTSpeed(uint32_t speed)
{
  //  Valid values are between 1200-115200

  if (speed < 1200 || speed > 115200)
    {
      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": Valid speed values are between 1200-115200");
      return false;
    }

  char pkt[7];

  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = SET_UART_SPEED;

  pkt[3] = ( ((speed & 0xff000000) >> 24) & 0xff );
  pkt[4] = ( ((speed & 0x00ff0000) >> 16) & 0xff );
  pkt[5] = ( ((speed & 0x0000ff00) >> 8)  & 0xff );
  pkt[6] = ( (speed & 0x000000ff) & 0xff );

  writeData(pkt, 7);

  return checkOK();
}


bool HMTRP::getRFSignalStrength(uint8_t *strength)
{
  if (!strength)
    return false;

  *strength = 0;

  char pkt[3];
  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = GET_RF_SIGNAL_STR;

  writeData(pkt, 3);
  usleep(100000);

  // now read back a 1 byte response
  char buf;
  int rv = readData(&buf, 1, defaultDelay);

  if (rv != 1)
    {
      cerr << __FUNCTION__ << ": failed to receive correct response: rv = " 
           << rv << ", expected 1" << endl;
      return false;
    }

  // now decode
  *strength = (uint8_t)buf;

  return true;
}

bool HMTRP::getModSignalStrength(uint8_t *strength)
{
  if (!strength)
    return false;

  *strength = 0;

  char pkt[3];
  pkt[0] = HMTRP_START1;
  pkt[1] = HMTRP_START2;
  pkt[2] = GET_MOD_SIGNAL_STR;

  writeData(pkt, 3);
  usleep(100000);

  // now read back a 1 byte response
  char buf;
  int rv = readData(&buf, 1, defaultDelay);

  if (rv != 1)
    {
      cerr << __FUNCTION__ << ": failed to receive correct response: rv = " 
           << rv << ", expected 1" << endl;
      return false;
    }

  // now decode
  *strength = (uint8_t)buf;

  return true;
}

