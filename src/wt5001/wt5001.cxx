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
#include <sstream>
#include <string>
#include <stdexcept>

#include "wt5001.h"

using namespace upm;
using namespace std;

static const int defaultDelay = 100;     // max wait time for read

WT5001::WT5001(int uart)
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

WT5001::~WT5001()
{
  if (m_ttyFd != -1)
    close(m_ttyFd);

  mraa_deinit();
}

bool WT5001::dataAvailable(unsigned int millis)
{
  if (m_ttyFd == -1)
    return false;

  struct timeval timeout;

  // no waiting
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

int WT5001::readData(char *buffer, int len)
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
      return rv;
    }

  return rv;
}

int WT5001::writeData(char *buffer, int len)
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
      return rv;
    }

  tcdrain(m_ttyFd);

  return rv;
}

bool WT5001::setupTty(speed_t baud)
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

bool WT5001::checkResponse(WT5001_OPCODE_T opcode)
{
  char resp;
  char fopcode = (char)opcode;

  int rv = readData(&resp, 1);

  // check for wrong response byte, or timeout
  if ((resp != fopcode) || rv == 0 )
    return false;

  return true;
}

bool WT5001::play(WT5001_PLAYSOURCE_T psrc, uint16_t index)
{
  char pkt[6];
  WT5001_OPCODE_T opcode = PLAY_SD;

  pkt[0] = WT5001_START;
  pkt[1] = 0x04;                // length

  switch (psrc)                 // src
    {
    case SD:
      opcode = PLAY_SD;
      break;

    case SPI:
      opcode = PLAY_SPI;
      break;

    case UDISK:
      opcode = PLAY_UDISK;
      break;
    }      

  pkt[2] = opcode;
  pkt[3] = (index >> 8) & 0xff; // index hi
  pkt[4] = index & 0xff;        // index lo
  pkt[5] = WT5001_END;

  writeData(pkt, 6);

  return checkResponse(opcode);
}

bool WT5001::stop()
{
  char pkt[4];
  WT5001_OPCODE_T opcode = STOP;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  return checkResponse(opcode);
}

bool WT5001::next()
{
  char pkt[4];
  WT5001_OPCODE_T opcode = NEXT;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  return checkResponse(opcode);
}

bool WT5001::previous()
{
  char pkt[4];
  WT5001_OPCODE_T opcode = PREVIOUS;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  return checkResponse(opcode);
}

bool WT5001::pause()
{
  char pkt[4];
  WT5001_OPCODE_T opcode = PAUSE;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  return checkResponse(opcode);
}

bool WT5001::setVolume(uint8_t vol)
{
  if (vol > WT5001_MAX_VOLUME)
    {
      // C++11 std::to_string() would be nice, but...
      std::ostringstream str;
      str << WT5001_MAX_VOLUME;

      throw std::out_of_range(std::string(__FUNCTION__) +
                              ": angle must be between 0 and " +
                              str.str());
      return false;
    }
  
  char pkt[5];
  WT5001_OPCODE_T opcode = SET_VOLUME;

  pkt[0] = WT5001_START;
  pkt[1] = 0x03;                // length
  pkt[2] = opcode;
  pkt[3] = vol;
  pkt[4] = WT5001_END;

  writeData(pkt, 5);

  return checkResponse(opcode);
}

bool WT5001::queue(uint16_t index)
{
  char pkt[6];
  WT5001_OPCODE_T opcode = QUEUE;

  pkt[0] = WT5001_START;
  pkt[1] = 0x04;                // length
  pkt[2] = opcode;
  pkt[3] = (index >> 8) & 0xff; // index hi
  pkt[4] = index & 0xff;        // index lo
  pkt[5] = WT5001_END;

  writeData(pkt, 6);

  return checkResponse(opcode);
}

bool WT5001::setPlayMode(WT5001_PLAYMODE_T pm)
{
  char pkt[5];
  WT5001_OPCODE_T opcode = PLAY_MODE;

  pkt[0] = WT5001_START;
  pkt[1] = 0x03;                // length
  pkt[2] = opcode;
  pkt[3] = pm;
  pkt[4] = WT5001_END;

  writeData(pkt, 5);

  return checkResponse(opcode);
}

bool WT5001::insert(uint16_t index)
{
  char pkt[6];
  WT5001_OPCODE_T opcode = INSERT_SONG;

  pkt[0] = WT5001_START;
  pkt[1] = 0x04;                // length
  pkt[2] = opcode;
  pkt[3] = (index >> 8) & 0xff; // index hi
  pkt[4] = index & 0xff;        // index lo
  pkt[5] = WT5001_END;

  writeData(pkt, 6);

  return checkResponse(opcode);
}

bool WT5001::setDate(uint16_t year, uint8_t month, uint8_t day)
{
  char pkt[8];
  WT5001_OPCODE_T opcode = SET_DATE;

  pkt[0] = WT5001_START;
  pkt[1] = 0x06;                // length
  pkt[2] = opcode;
  pkt[3] = (year >> 8) & 0xff;  // year hi
  pkt[4] = year & 0xff;         // year lo
  pkt[5] = month;               // month
  pkt[6] = day;                 // day
  pkt[7] = WT5001_END;

  writeData(pkt, 8);

  return checkResponse(opcode);
}

bool WT5001::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  char pkt[7];
  WT5001_OPCODE_T opcode = SET_TIME;

  pkt[0] = WT5001_START;
  pkt[1] = 0x05;                // length
  pkt[2] = opcode;
  pkt[3] = hour;                // hour
  pkt[4] = minute;              // minute
  pkt[5] = second;              // second
  pkt[6] = WT5001_END;

  writeData(pkt, 7);

  return checkResponse(opcode);
}

bool WT5001::setAlarm(uint8_t hour, uint8_t minute, uint8_t second)
{
  char pkt[7];
  WT5001_OPCODE_T opcode = SET_ALARM;

  pkt[0] = WT5001_START;
  pkt[1] = 0x05;                // length
  pkt[2] = opcode;
  pkt[3] = hour;                // hour
  pkt[4] = minute;              // minute
  pkt[5] = second;              // second
  pkt[6] = WT5001_END;

  writeData(pkt, 7);

  return checkResponse(opcode);
}

bool WT5001::clearAlarm()
{
  char pkt[4];
  WT5001_OPCODE_T opcode = CLEAR_ALARM;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  return checkResponse(opcode);
}

bool WT5001::getVolume(uint8_t *vol)
{
  char pkt[4];
  WT5001_OPCODE_T opcode = READ_VOLUME;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  if (!checkResponse(opcode))
    return false;

  // there should be a byte waiting for us, the volume
  int rv = readData((char *)vol, 1);
  if (rv != 1)
    return false;

  return true;
}

bool WT5001::getPlayState(uint8_t *ps)
{
  char pkt[4];
  WT5001_OPCODE_T opcode = READ_PLAY_STATE;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  if (!checkResponse(opcode))
    return false;

  // there should be a byte waiting for us, the play state
  int rv = readData((char *)ps, 1);
  if (rv != 1)
    return false;

  return true;
}

bool WT5001::getNumFiles(WT5001_PLAYSOURCE_T psrc, uint16_t *numf)
{
  char pkt[4];
  WT5001_OPCODE_T opcode;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length

  switch (psrc)                 // src
    {
    case SD:
      opcode = READ_SD_NUMF;
      break;

    case SPI:
      opcode = READ_SPI_NUMF;
      break;

    case UDISK:
      opcode = READ_UDISK_NUMF;
      break;
    }      

  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  if (!checkResponse(opcode))
    return false;

  // read the two byte response, and encode them
  char buf[2];
  int rv = readData(buf, 2);
  if (rv != 2)
    return false;

  *numf = (buf[0] << 8) | buf[1];

  return true;
}

bool WT5001::getCurrentFile(uint16_t *curf)
{
  char pkt[4];
  WT5001_OPCODE_T opcode = READ_CUR_FNAME;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  if (!checkResponse(opcode))
    return false;

  // read the two byte response, and encode them
  char buf[2];
  int rv = readData(buf, 2);
  if (rv != 2)
    return false;

  *curf = (buf[0] << 8) | (buf[1] & 0xff);

  return true;
}

bool WT5001::getDate(uint16_t *year, uint8_t *month, uint8_t *day)
{
  char pkt[4];
  WT5001_OPCODE_T opcode = READ_DATE;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  if (!checkResponse(opcode))
    return false;

  // read the 4 byte response
  char buf[4];
  int rv = readData(buf, 4);
  if (rv != 4)
    return false;

  *year = (buf[0] << 8) | (buf[1] & 0xff);
  *month = buf[2];
  *day = buf[3];
  return true;
}

bool WT5001::getTime(uint8_t *hour, uint8_t *minute, uint8_t *second)
{
  char pkt[4];
  WT5001_OPCODE_T opcode = READ_TIME;

  pkt[0] = WT5001_START;
  pkt[1] = 0x02;                // length
  pkt[2] = opcode;
  pkt[3] = WT5001_END;

  writeData(pkt, 4);

  if (!checkResponse(opcode))
    return false;

  // read the 3 byte response
  char buf[3];
  int rv = readData(buf, 3);
  if (rv != 3)
    return false;

  *hour = buf[0];
  *minute = buf[1];
  *second = buf[2];
  return true;
}

