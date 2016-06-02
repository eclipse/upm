/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "bacnetutil.hpp"

using namespace upm;
using namespace std;


BACNETUTIL::BACNETUTIL(uint32_t targetDeviceObjectID) :
  m_instance(0)
{
  // Save our device's ID
  m_targetDeviceObjectID = targetDeviceObjectID;

  // create the BACNETMSTP instance here if it does not already exist,
  // and store the pointer in our class to save on some typing.

  m_instance = BACNETMSTP::instance();

  // now see if it has been initialized yet for init()
  m_initialized = m_instance->isInitialized();

  setDebug(false);

  // we disable this by default for performance reasons
  checkReliability(false);

  // empty our unit caches
  m_avUnitCache.clear();
  m_aiUnitCache.clear();
}

BACNETUTIL::~BACNETUTIL()
{
}

void BACNETUTIL::initMaster(std::string port, int baudRate,
                            int deviceInstanceID, int macAddr, int maxMaster,
                            int maxInfoFrames)
{
  // first we check to see if the bacnetmstp instance has already been
  // initialized (determined in the ctor).  If not, we will do so here
  // with the arguments specified.  If it has already been
  // initialized, then we do not bother calling bacnetmstp's init
  // again as it will just be ignored.

  if (!m_initialized)
    m_instance->initMaster(port, baudRate, deviceInstanceID,
                           macAddr, maxMaster, maxInfoFrames);

  // either it threw an exception, was already initialized or it's
  // initialized now...
  m_initialized = true;
}

void BACNETUTIL::setDebug(bool enable)
{
  m_debugging = enable;

  // we also enable/disable debugging in BACNETMSTP
  m_instance->setDebug(enable);
}

float BACNETUTIL::getAnalogValue(uint32_t objInstance)
{
  // check reliability first, if enabled
  if (m_checkReliability)
    {
      if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                   objInstance, PROP_RELIABILITY))
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": (reliability): " << getAllErrorString()
                 << endl;

          throw std::runtime_error(std::string(__FUNCTION__)
                                   + getAllErrorString());
        }

      BACNET_RELIABILITY reliable =
        static_cast<BACNET_RELIABILITY>(m_instance->getDataTypeEnum());

      if (reliable != RELIABILITY_NO_FAULT_DETECTED)
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": Reliability check failed" << endl;
          
          throw std::runtime_error(std::string(__FUNCTION__)
                                   + ": Reliability check failed");
        }
    }

  // now get the value
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                               objInstance, PROP_PRESENT_VALUE))
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": (value): " << getAllErrorString()
             << endl;
      
      throw std::runtime_error(std::string(__FUNCTION__)
                               + getAllErrorString());
    }

  return m_instance->getDataTypeReal();
}

void BACNETUTIL::setAnalogValue(uint32_t objInstance, 
                                float value)
{
  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(value);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                objInstance, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + getAllErrorString());
    }
}

string BACNETUTIL::getAnalogValueUnits(uint32_t objInstance)
{
  // see if it exists
  if (m_avUnitCache.count(objInstance) == 0)
    {
      // then we need to fetch it
      if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                   objInstance, PROP_UNITS))
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;
          // set to empty string
          m_avUnitCache[objInstance] = string("");
        }
      else
        {
          // cache it for future calls
          m_avUnitCache[objInstance] =
            string(bactext_engineering_unit_name(m_instance->getDataTypeEnum()));
        }
    }

  return m_avUnitCache[objInstance];
}

float BACNETUTIL::getAnalogInput(uint32_t objInstance)
{
  // check reliability first, if enabled
  if (m_checkReliability)
    {
      if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_INPUT,
                                   objInstance, PROP_RELIABILITY))
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": (reliability): "
                 << getAllErrorString() << endl;

          throw std::runtime_error(std::string(__FUNCTION__)
                                   + getAllErrorString());
        }

      BACNET_RELIABILITY reliable =
        static_cast<BACNET_RELIABILITY>(m_instance->getDataTypeEnum());

      if (reliable != RELIABILITY_NO_FAULT_DETECTED)
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": Reliability check failed" << endl;

          throw std::runtime_error(std::string(__FUNCTION__)
                                   + ": Reliability check failed");
        }
    }

  // now get the value
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_INPUT,
                               objInstance, PROP_PRESENT_VALUE))
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": (value): " << getAllErrorString() << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + getAllErrorString());
    }

  return m_instance->getDataTypeReal();
}

bool BACNETUTIL::getBinaryInput(uint32_t objInstance)
{
  // check reliability first, if enabled
  if (m_checkReliability)
    {
      if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_BINARY_INPUT,
                                   objInstance, PROP_RELIABILITY))
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": (reliability): "
                 << getAllErrorString() << endl;

          throw std::runtime_error(std::string(__FUNCTION__)
                                   + getAllErrorString());
        }

      BACNET_RELIABILITY reliable =
        static_cast<BACNET_RELIABILITY>(m_instance->getDataTypeEnum());

      if (reliable != RELIABILITY_NO_FAULT_DETECTED)
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": Reliability check failed" << endl;

          throw std::runtime_error(std::string(__FUNCTION__)
                                   + ": Reliability check failed");
        }
    }

  // now get the value
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_BINARY_INPUT,
                               objInstance, PROP_PRESENT_VALUE))
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": (value): " << getAllErrorString() << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + getAllErrorString());
    }

  BACNET_BINARY_PV bpv = 
    static_cast<BACNET_BINARY_PV>(m_instance->getDataTypeEnum());

  return (bpv == BINARY_INACTIVE) ? false : true;
}

bool BACNETUTIL::getBinaryValue(uint32_t objInstance)
{
  // check reliability first, if enabled
  if (m_checkReliability)
    {
      if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_BINARY_VALUE,
                                   objInstance, PROP_RELIABILITY))
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": (reliability): "
                 << getAllErrorString() << endl;

          throw std::runtime_error(std::string(__FUNCTION__)
                                   + getAllErrorString());
        }

      BACNET_RELIABILITY reliable =
        static_cast<BACNET_RELIABILITY>(m_instance->getDataTypeEnum());

      if (reliable != RELIABILITY_NO_FAULT_DETECTED)
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": Reliability check failed" << endl;

          throw std::runtime_error(std::string(__FUNCTION__)
                                   + ": Reliability check failed");
        }
    }

  // now get the value
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_BINARY_VALUE,
                               objInstance, PROP_PRESENT_VALUE))
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": (value): " << getAllErrorString() << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + getAllErrorString());
    }

  BACNET_BINARY_PV bpv = 
    static_cast<BACNET_BINARY_PV>(m_instance->getDataTypeEnum());

  return (bpv == BINARY_INACTIVE) ? false : true;
}

void BACNETUTIL::setBinaryValue(uint32_t objInstance, 
                                bool value)
{
  BACNET_BINARY_PV bpv = (value) ? BINARY_ACTIVE : BINARY_INACTIVE;

  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataEnum(bpv);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_BINARY_VALUE,
                                objInstance, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + getAllErrorString());
    }
}

string BACNETUTIL::getAnalogInputUnits(uint32_t objInstance)
{
  // see if it exists
  if (m_aiUnitCache.count(objInstance) == 0)
    {
      // then we need to fetch it
      if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_INPUT,
                                   objInstance, PROP_UNITS))
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;
          // set to empty string
          m_aiUnitCache[objInstance] = string("");
        }
      else
        {
          // cache it for future calls
          m_aiUnitCache[objInstance] =
            string(bactext_engineering_unit_name(m_instance->getDataTypeEnum()));
        }
    }

  return m_aiUnitCache[objInstance];
}

BACNETMSTP::BACERR_TYPE_T BACNETUTIL::getErrorType()
{
  return m_instance->getErrorType();
}

uint8_t BACNETUTIL::getRejectReason()
{
  return m_instance->getRejectReason();
}

std::string BACNETUTIL::getRejectString()
{
  return m_instance->getRejectString();
}

uint8_t BACNETUTIL::getAbortReason()
{
  return m_instance->getAbortReason();
}

std::string BACNETUTIL::getAbortString()
{
  return m_instance->getAbortString();
}

BACNET_ERROR_CLASS BACNETUTIL::getErrorClass()
{
  return m_instance->getErrorClass();
}

BACNET_ERROR_CODE BACNETUTIL::getErrorCode()
{
  return m_instance->getErrorCode();
}

std::string BACNETUTIL::getUPMErrorString()
{
  return m_instance->getUPMErrorString();
}

std::string BACNETUTIL::getErrorString()
{
  return m_instance->getErrorString();
};

string BACNETUTIL::getAllErrorString()
{
  switch (m_instance->getErrorType())
    {
    case BACNETMSTP::BACERR_TYPE_NONE:
      return string("No Error");
      break;

    case BACNETMSTP::BACERR_TYPE_REJECT:
      return string("Reject: ") + getRejectString();
      break;

    case BACNETMSTP::BACERR_TYPE_ABORT:
      return string("Abort: ") + getAbortString();
      break;

    case BACNETMSTP::BACERR_TYPE_ERROR:
      return string("Error: ") + getErrorString();
      break;

    case BACNETMSTP::BACERR_TYPE_UPM:
      return string("UPM Error: ") + getUPMErrorString();
      break;
    }
}

string BACNETUTIL::getDescription()
{
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_DEVICE,
                               m_targetDeviceObjectID, PROP_DESCRIPTION))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return "";
    }

  return m_instance->getDataTypeString();
}

string BACNETUTIL::getLocation()
{
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_DEVICE,
                               m_targetDeviceObjectID, PROP_LOCATION))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return "";
    }

  return m_instance->getDataTypeString();
}

bool BACNETUTIL::setLocation(string location)
{
  BACNET_APPLICATION_DATA_VALUE myLocation =
    m_instance->createDataString(location);

  // write the Device Object Location
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_DEVICE,
                                m_targetDeviceObjectID, PROP_LOCATION,
                                &myLocation))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

