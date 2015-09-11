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

#include <stdexcept>

#include "sm130.h"

using namespace upm;
using namespace std;

// Uncomment the below to see packaet data sent and received from the SM130
// #define SM130_DEBUG

static const int defaultDelay = 1000; // ms for read

static const int maxLen = 64; // max number of bytes to read

SM130::SM130(int uart, int reset) :
  m_uart(uart), m_gpioReset(reset)
{
  m_tagType = TAG_NONE;
  m_uidLen = 0;
  m_uid.clear();
  clearError();
  initClock();

  m_gpioReset.dir(mraa::DIR_OUT);
  m_gpioReset.write(0);
}

SM130::~SM130() 
{
}

mraa::Result SM130::setBaudRate(int baud)
{
  m_baud = baud;
  return m_uart.setBaudRate(baud);
}

string SM130::sendCommand(CMD_T cmd, string data)
{
  uint8_t cksum = 0;
  string command;

  // for uart, we need to add the sync bytes, 0xff, 0x00
  command.push_back(0xff);
  command.push_back(0x00);
  
  // compute the length - command + data
  uint8_t len = 1; // command
  if (!data.empty())
    len += data.size();

  command.push_back(len);

  cksum += len;

  // now the command
  command.push_back(cmd);
  cksum += cmd;

  // now the data if any
  if (!data.empty())
    {
      for (int i=0; i<data.size(); i++)
        {
          command.push_back(data[i]);
          cksum += (uint8_t)data[i];
        }
    }

  // now add the checksum
  command.push_back(cksum);

#ifdef SM130_DEBUG
  cerr << "CMD: " << string2HexString(command) << endl;
#endif // SM130_DEBUG

  // send it
  m_uart.writeStr(command);

  // if the command is SET_BAUD, then switch to the new baudrate here
  // before attempting to read the response (and hope it worked).
  if (cmd == CMD_SET_BAUD)
    {
      usleep(100000); // 100ms
      setBaudRate(m_baud);
    }

  // now wait for a response
  if (!m_uart.dataAvailable(defaultDelay))
    {
      cerr << __FUNCTION__ << ": timeout waiting for response" << endl;
      return "";
    }

  string resp = m_uart.readStr(maxLen);

#ifdef SM130_DEBUG
  cerr << "RSP: " << string2HexString(resp) << endl;
#endif // SM130_DEBUG

  if (!((uint8_t)resp[0] == 0xff && (uint8_t)resp[1] == 0x00))
    {
      cerr << __FUNCTION__ << ": invalid packet header" << endl;
      return "";
    }
    
  // check size - 2 header bytes + len + cksum.
  if (resp.size() != ((uint8_t)resp[2] + 2 + 1 + 1))
    {
      cerr << __FUNCTION__ << ": invalid packet length, expected " 
           << int((uint8_t)resp[2] + 2 + 1 + 1) 
           << ", got " << resp.size() << endl;
      return "";
    }

  // verify the cksum
  cksum = 0;
  for (int i=2; i<(resp.size() - 1); i++)
    cksum += (uint8_t)resp[i];

  if (cksum != (uint8_t)resp[resp.size() - 1])
    {
      cerr << __FUNCTION__ << ": invalid checksum, expected " 
           << int(cksum) << ", got " << (uint8_t)resp[resp.size()-1] << endl;
      return "";
    }

  // we could also verify that the command code returned was for the
  // command submitted...

  // now, remove the 2 header bytes and the checksum, leave the length
  // and command.
  resp.erase(resp.size() - 1, 1); // cksum
  resp.erase(0, 2);               // header bytes

  // return the rest
  return resp;
}

string SM130::getFirmwareVersion()
{
  clearError();

  string resp = sendCommand(CMD_VERSION, "");

  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return "";
    }

  // delete the len and cmd, return the rest
  resp.erase(0, 2);
  return resp;
}

bool SM130::reset()
{
  clearError();

  string resp = sendCommand(CMD_RESET, "");
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }

  return true;
}

void SM130::hardwareReset()
{
  m_gpioReset.write(1);
  usleep(100000);
  m_gpioReset.write(0);
}

bool SM130::select()
{
  clearError();

  m_tagType = TAG_NONE;
  m_uidLen = 0;
  m_uid.clear();

  string resp = sendCommand(CMD_SELECT_TAG, "");

  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }

  if ((uint8_t)resp[0] == 2)
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'N': m_lastErrorString = "No tag present";
          break;
        case 'U': m_lastErrorString = "Access failed, RF field is off";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return false;
    }

  // if we are here, then store the uid info and tag type.
  m_tagType = (TAG_TYPE_T)resp[2];

  if ((uint8_t)resp[0] == 6)
    m_uidLen = 4;               // 4 byte uid
  else
    m_uidLen = 7;               // 7 byte

  for (int i=0; i<m_uidLen; i++)
    m_uid.push_back(resp[i+3]);

  return true;
}

bool SM130::authenticate(uint8_t block, KEY_TYPES_T keyType, string key)
{
  clearError();

  // A little sanity checking...
  if (keyType == KEY_TYPE_A || keyType == KEY_TYPE_B)
    {
      if (key.empty())
        throw std::invalid_argument(string(__FUNCTION__) +
                                    ": You must specify a key for type A or B");
      if (key.size() != 6)
        throw std::invalid_argument(string(__FUNCTION__) +
                                    ": Key size must be 6");

      return false; // probably not reached :)
    }
  else
    {
      // make sure the key is empty for any other key type
      key.clear();
    }

  string data;
  data.push_back(block);
  data.push_back(keyType);
  data += key;

  string resp = sendCommand(CMD_AUTHENTICATE, data);
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }
  
  // response len is always 2, 'L' means auth was successful
  if (resp[2] != 'L')
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'N': m_lastErrorString = "No tag present, or login failed";
          break;
        case 'U': m_lastErrorString = "Login failed";
          break;
        case 'E': m_lastErrorString = "Invalid key format in EEPROM";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return false;
    }

  return true;
}

string SM130::readBlock16(uint8_t block)
{
  clearError();

  string data;

  data.push_back(block);

  string resp = sendCommand(CMD_READ16, data);

  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return "";
    }

  if ((uint8_t)resp[0] == 2)
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'N': m_lastErrorString = "No tag present";
          break;
        case 'F': m_lastErrorString = "Read failed";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }

      return "";
    }

  // trim off the len, cmd, and block # bytes and return the rest
  resp.erase(0, 3);
  return resp;
}

int32_t SM130::readValueBlock(uint8_t block)
{
  clearError();

  string data;

  data.push_back(block);

  string resp = sendCommand(CMD_READ_VALUE, data);

  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return 0;
    }

  if ((uint8_t)resp[0] == 2)
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'N': m_lastErrorString = "No tag present";
          break;
        case 'F': m_lastErrorString = "Read failed";
          break;
        case 'I': m_lastErrorString = "Invalid Value Block";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }

      return 0;
    }

  int32_t rv;
  rv = ((uint8_t)resp[3] |
        ((uint8_t)resp[4] << 8) |
        ((uint8_t)resp[5] << 16) |
        ((uint8_t)resp[6] << 24));

  return rv;
}

bool SM130::writeBlock16(uint8_t block, string contents)
{
  clearError();

  // A little sanity checking...
  if (contents.size() != 16)
    {
      throw std::invalid_argument(string(__FUNCTION__) +
                                  ": You must supply 16 bytes for block content");
      
      return false;
    }

  string data;
  data.push_back(block);
  data += contents;

  string resp = sendCommand(CMD_WRITE16, data);
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }
  
  if ((uint8_t)resp[0] == 2)
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'F': m_lastErrorString = "Write failed";
          break;
        case 'N': m_lastErrorString = "No tag present";
          break;
        case 'U': m_lastErrorString = "Read after write failed";
          break;
        case 'X': m_lastErrorString = "Unable to read after write";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return false;
    }

  return true;
}

bool SM130::writeValueBlock(uint8_t block, int32_t value)
{
  clearError();
  
  string data;
  data.push_back(block);
  // put the value in, LSB first
  data += (value & 0xff);
  data += ((value >> 8) & 0xff);
  data += ((value >> 16) & 0xff);
  data += ((value >> 24) & 0xff);

  string resp = sendCommand(CMD_WRITE_VALUE, data);
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }
  
  if ((uint8_t)resp[0] == 2)
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'F': m_lastErrorString = "Read failed during verification";
          break;
        case 'N': m_lastErrorString = "No tag present";
          break;
        case 'I': m_lastErrorString = "Invalid value block";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return false;
    }

  return true;
}

bool SM130::writeBlock4(uint8_t block, string contents)
{
  clearError();

  // A little sanity checking...
  if (contents.size() != 4)
    {
      throw std::invalid_argument(string(__FUNCTION__) +
                                  ": You must supply 4 bytes for block content");
      
      return false;
    }

  string data;
  data.push_back(block);
  data += contents;

  string resp = sendCommand(CMD_WRITE4, data);
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }
  
  if ((uint8_t)resp[0] == 2)
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'F': m_lastErrorString = "Write failed";
          break;
        case 'N': m_lastErrorString = "No tag present";
          break;
        case 'U': m_lastErrorString = "Read after write failed";
          break;
        case 'X': m_lastErrorString = "Unable to read after write";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return false;
    }

  return true;
}

bool SM130::writeKey(uint8_t eepromSector, KEY_TYPES_T keyType, string key)
{
  clearError();

  // A little sanity checking...
  eepromSector &= 0x0f; // Only 0x00-0x0f is valid

  if (!(keyType == KEY_TYPE_A || keyType == KEY_TYPE_B))
    {
      throw std::invalid_argument(string(__FUNCTION__) +
                                  ": Key type must be A or B");
      
      return false;
    }

  if (key.size() != 6)
    {
      throw std::invalid_argument(string(__FUNCTION__) +
                                  ": Key must be 6 bytes");
      
      return false;
    }

  string data;
  data.push_back(eepromSector);
  data += keyType;
  data += key;

  string resp = sendCommand(CMD_WRITE_KEY, data);
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }

  // reponse len is always 2
  if ((uint8_t)resp[2] != 'L')
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'N': m_lastErrorString = "Write master key failed";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return false;
    }

  return true;
}

int32_t SM130::adjustValueBlock(uint8_t block, int32_t value, bool incr)
{
  clearError();
  
  string data;
  data.push_back(block);
  // put the value in, LSB first
  data += (value & 0xff);
  data += ((value >> 8) & 0xff);
  data += ((value >> 16) & 0xff);
  data += ((value >> 24) & 0xff);

  string resp = sendCommand(((incr) ? CMD_INC_VALUE : CMD_DEC_VALUE), data);
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return 0;
    }
  
  if ((uint8_t)resp[0] == 2)
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'F': m_lastErrorString = "Read failed during verification";
          break;
        case 'N': m_lastErrorString = "No tag present";
          break;
        case 'I': m_lastErrorString = "Invalid value block";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return 0;
    }

  // now unpack the new value, LSB first
  int32_t rv;
  rv = ((uint8_t)resp[3] |
        ((uint8_t)resp[4] << 8) |
        ((uint8_t)resp[5] << 16) |
        ((uint8_t)resp[6] << 24));
  
  return rv;
}

bool SM130::setAntennaPower(bool on)
{
  clearError();
  
  string resp = sendCommand(CMD_ANTENNA_POWER, "");
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }
  
  return true;
}

uint8_t SM130::readPorts()
{
  clearError();
  
  string resp = sendCommand(CMD_READ_PORT, "");
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return 0;
    }
  
  // only the first 2 bits are valid
  return (resp[2] & 3);
}

bool SM130::writePorts(uint8_t val)
{
  clearError();
  
  // only the first 2 bits are valid
  val &= 3;

  string data;
  data.push_back(val);

  string resp = sendCommand(CMD_WRITE_PORT, data);
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }
  
  return true;
}

bool SM130::haltTag()
{
  clearError();

  string resp = sendCommand(CMD_HALT_TAG, "");
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }

  // reponse len is always 2
  if ((uint8_t)resp[2] != 'L')
    {
      // then we got an error of some sort, store the error code, str
      // and bail.
      m_lastErrorCode = resp[2];
      
      switch (m_lastErrorCode) 
        {
        case 'U': m_lastErrorString = "Can not halt, RF field is off";
          break;
        default: m_lastErrorString = "Unknown error code";
          break;
        }
      return false;
    }

  return true;
}

bool SM130::setSM130BaudRate(int baud)
{
  clearError();
  
  uint8_t newBaud;

  switch (baud)
    {
    case 9600: newBaud = 0x00;
      break;
    case 19200: newBaud = 0x01;
      break;
    case 38400: newBaud = 0x02;
      break;
    case 57600: newBaud = 0x03;
      break;
    case 115200: newBaud = 0x04;
      break;
    default:
      throw std::invalid_argument(string(__FUNCTION__) +
                                  ": Invalid SM130 baud rate specified");
    }

  // WARNING: this is a dangerous command
  int oldBaud = m_baud;
  m_baud = baud;
  
  string data;
  data.push_back(newBaud);

  string resp = sendCommand(CMD_SET_BAUD, data);

  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      cerr << __FUNCTION__ << ": restoring old baud rate" << endl;

      setBaudRate(oldBaud);
      return false;
    }

  // otherwise assume success, possibly incorrectly
  return true;
}

bool SM130::sleep()
{
  clearError();
  
  string resp = sendCommand(CMD_SLEEP, "");
  
  if (resp.empty())
    {
      cerr << __FUNCTION__ << ": failed" << endl;
      return false;
    }
  
  return true;
}

string SM130::string2HexString(string input)
{
  static const char* const lut = "0123456789abcdef";
  size_t len = input.size();
  
  string output;
  output.reserve(3 * len);
  for (size_t i = 0; i < len; ++i)
    {
      const unsigned char c = input[i];
      output.push_back(lut[c >> 4]);
      output.push_back(lut[c & 15]);
      output.push_back(' ');
    }

  return output;
}

void SM130::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t SM130::getMillis()
{
  struct timeval elapsed, now;
  uint32_t elapse;

  // get current time
  gettimeofday(&now, NULL);

  // compute the delta since m_startTime
  if( (elapsed.tv_usec = now.tv_usec - m_startTime.tv_usec) < 0 ) 
    {
      elapsed.tv_usec += 1000000;
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec - 1;
    } 
  else 
    {
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec;
    }

  elapse = (uint32_t)((elapsed.tv_sec * 1000) + (elapsed.tv_usec / 1000));

  // never return 0
  if (elapse == 0)
    elapse = 1;

  return elapse;
}

bool SM130::waitForTag(uint32_t timeout)
{
  initClock();

  do 
    {
      if (select())
        {
          // success
          return true;
        }
      else
        {
          // If there was an error, fail if it's anything other than a
          // tag not present
          if (getLastErrorCode() != 'N')
            return false;

          // otherwise, sleep for 100ms and try again
          usleep(100000);
        }
    } while (getMillis() <= timeout);

  return false;
}

string SM130::tag2String(TAG_TYPE_T tag)
{
  switch (tag)
    {
    case TAG_MIFARE_ULTRALIGHT: return "MiFare Ultralight";
    case TAG_MIFARE_1K:         return "MiFare 1K";
    case TAG_MIFARE_4K:         return "MiFare 4K";
    case TAG_UNKNOWN:           return "Unknown Tag Type";
    default:                    return "Invalid Tag Type";
    }
}



