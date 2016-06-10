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

  // empty our msv info store
  m_msvInfo.clear();

  // empty our binary info stores
  m_bvInfo.clear();
  m_biInfo.clear();
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
                               + ": "
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
                               + ": "
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
                               + ": "
                               + getAllErrorString());
    }

  return m_instance->getDataTypeReal();
}

bool BACNETUTIL::getBinaryInput(uint32_t objInstance)
{
  // check the BV info, and update/cache the data if needed
  updateBinaryInputInfo(objInstance);

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
                               + ": "
                               + getAllErrorString());
    }

  BACNET_BINARY_PV bpv =
    static_cast<BACNET_BINARY_PV>(m_instance->getDataTypeEnum());

  return (bpv == BINARY_INACTIVE) ? false : true;
}

bool BACNETUTIL::getBinaryValue(uint32_t objInstance)
{
  // check the BV info, and update/cache the data if needed
  updateBinaryValueInfo(objInstance);

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
                               + ": "
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
                               + ": "
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

unsigned int BACNETUTIL::getMultiStateValue(uint32_t objInstance)
{
  // check the MSV info, and update/cache the data if needed
  updateMultiStateValueInfo(objInstance);

  // check reliability first, if enabled
  if (m_checkReliability)
    {
      if (m_instance->readProperty(m_targetDeviceObjectID,
                                   OBJECT_MULTI_STATE_VALUE,
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
  if (m_instance->readProperty(m_targetDeviceObjectID,
                               OBJECT_MULTI_STATE_VALUE,
                               objInstance, PROP_PRESENT_VALUE))
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": (value): " << getAllErrorString() << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": "
                               + getAllErrorString());
    }

  return m_instance->getDataTypeUnsignedInt();
}

void BACNETUTIL::updateMultiStateValueInfo(uint32_t objInstance)
{
  // bail if we already have information on this msv
  if (m_msvInfo.count(objInstance) != 0)
    return;

  // we need to fetch information on MSV's - number of states, and
  // possibly the state-text, if present

  // get the number of values possible (required)
  if (m_instance->readProperty(m_targetDeviceObjectID,
                               OBJECT_MULTI_STATE_VALUE,
                               objInstance, PROP_NUMBER_OF_STATES))
    {
      if (m_debugging)
        cerr << __FUNCTION__
             << ": (number of states): "
             << getAllErrorString()
             << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": "
                               + getAllErrorString());
    }

  // Add the entry...
  m_msvInfo[objInstance].numStates = m_instance->getDataTypeUnsignedInt();

  if (m_debugging)
    cerr << __FUNCTION__
         << ": number of states: "
         << m_msvInfo[objInstance].numStates
         << endl;

  // now get the state-text.  This is optional, so we will not throw
  // here.
  if (m_instance->readProperty(m_targetDeviceObjectID,
                               OBJECT_MULTI_STATE_VALUE,
                               objInstance, PROP_STATE_TEXT))
    {
      if (m_debugging)
        cerr << __FUNCTION__
             << ": (state-text): "
             << getAllErrorString()
             << endl;
    }

  // store them
  int numElements = m_instance->getDataNumElements();

  if (m_debugging)
    cerr << __FUNCTION__ << ": numElements: " << numElements << endl;

  if (numElements > 0)
    {
      for (int i=0; i<numElements; i++)
        {
          m_msvInfo[objInstance].stateList.push_back(m_instance->getDataTypeString(i));

          if (m_debugging)
            cerr << __FUNCTION__ << ": " << int(objInstance) << ", "
                 << i << ": "
                 << "added state text string: "
                 << m_msvInfo[objInstance].stateList.at(i)
                 << endl;
        }
    }

  return;
}

void BACNETUTIL::deleteMultiStateValueInfo(uint32_t objInstance)
{
  // if there is no data stored for this objInstance yet, then we do
  // not need to do anything.
  if (m_msvInfo.count(objInstance) == 0)
    return;

  // Now, we just erase the entry, and it will be updated the next
  // time the MSV is accessed.
  m_msvInfo.erase(objInstance);

  return;
}

string BACNETUTIL::lookupMultiStateValueText(uint32_t objInstance,
                                             unsigned int value)
{
  // verify that we have the relevant object data cached. If not, go
  // get it.
  updateMultiStateValueInfo(objInstance);

  // verify that value is valid for this object
  if (value == 0 || value > m_msvInfo[objInstance].numStates)
    throw std::out_of_range(std::string(__FUNCTION__)
                            + ": value supplied is invalid. Maximum "
                            + "allowed values are 1 to "
                            + std::to_string(m_msvInfo[objInstance].numStates)
                            + " for this object");


  // at this point either it failed or suceeded.  If it suceeded, then
  // we will see if any state text was retrieved.  If no text is
  // available (it is an optional property), then we will simply
  // return the value itself as a string.

  if (m_msvInfo[objInstance].stateList.size() > 0)
    {
      // we have state-text and a usable value.

      // value should never be 0 at this point, so compensate for
      // indexing into stateList which is 0-based.
      value--;

      return m_msvInfo[objInstance].stateList.at(value);
    }

  // no stateList text available, so just return value as a string
  return std::to_string(value);
}

unsigned int BACNETUTIL::getMultiStateValueMaxStates(uint32_t objInstance)
{
  // check the MSV info, and update/cache the data if needed
  updateMultiStateValueInfo(objInstance);

  return m_msvInfo[objInstance].numStates;
}

string BACNETUTIL::getMultiStateValueText(uint32_t objInstance)
{
  unsigned int value = getMultiStateValue(objInstance);

  return lookupMultiStateValueText(objInstance, value);
}

void BACNETUTIL::setMultiStateValue(uint32_t objInstance,
                                    unsigned int value)
{
  // check the MSV info, and update/cache the data if needed
  updateMultiStateValueInfo(objInstance);

  // Check value against the valid limits

  if (value == 0 || value > m_msvInfo[objInstance].numStates)
    throw std::invalid_argument(std::string(__FUNCTION__)
                                + ": value supplied is invalid. Maximum "
                                + "allowed values are 1 to "
                                + std::to_string(m_msvInfo[objInstance].numStates)
                                + " for this object");

  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataUnsignedInt(value);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID,
                                OBJECT_MULTI_STATE_VALUE,
                                objInstance, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      throw std::runtime_error(std::string(__FUNCTION__)
                               + ": "
                               + getAllErrorString());
    }
}

void BACNETUTIL::updateBinaryValueInfo(uint32_t objInstance)
{
  // bail if we already have information on this object
  if (m_bvInfo.count(objInstance) != 0)
    return;

  // fetch inactive/active text.  These are optional accordingto the
  // spec, so we will not throw if they do not exist.

  // get inactive text
  if (m_instance->readProperty(m_targetDeviceObjectID,
                               OBJECT_BINARY_VALUE,
                               objInstance, PROP_INACTIVE_TEXT))
    {
      if (m_debugging)
        cerr << __FUNCTION__
             << ": (inactive text): "
             << getAllErrorString()
             << endl;

      m_bvInfo[objInstance].inactiveText = "inactive";
    }
  else
    {
      m_bvInfo[objInstance].inactiveText = m_instance->getDataTypeString();
    }

  // get active text
  if (m_instance->readProperty(m_targetDeviceObjectID,
                               OBJECT_BINARY_VALUE,
                               objInstance, PROP_ACTIVE_TEXT))
    {
      if (m_debugging)
        cerr << __FUNCTION__
             << ": (active text): "
             << getAllErrorString()
             << endl;

      m_bvInfo[objInstance].activeText = "active";
    }
  else
    {
      m_bvInfo[objInstance].activeText = m_instance->getDataTypeString();
    }

  return;
}

void BACNETUTIL::deleteBinaryValueInfo(uint32_t objInstance)
{
  // if there is no data stored for this objInstance yet, then we do
  // not need to do anything.
  if (m_bvInfo.count(objInstance) == 0)
    return;

  // Now, we just erase the entry, and it will be updated the next
  // time it is accessed.
  m_bvInfo.erase(objInstance);

  return;
}

void BACNETUTIL::updateBinaryInputInfo(uint32_t objInstance)
{
  // bail if we already have information on this object
  if (m_biInfo.count(objInstance) != 0)
    return;

  // fetch inactive/active text.  These are optional accordingto the
  // spec, so we will not throw if they do not exist.

  // get inactive text
  if (m_instance->readProperty(m_targetDeviceObjectID,
                               OBJECT_BINARY_INPUT,
                               objInstance, PROP_INACTIVE_TEXT))
    {
      if (m_debugging)
        cerr << __FUNCTION__
             << ": (inactive text): "
             << getAllErrorString()
             << endl;

      m_biInfo[objInstance].inactiveText = "inactive";
    }
  else
    {
      m_biInfo[objInstance].inactiveText = m_instance->getDataTypeString();
    }

  // get active text
  if (m_instance->readProperty(m_targetDeviceObjectID,
                               OBJECT_BINARY_INPUT,
                               objInstance, PROP_ACTIVE_TEXT))
    {
      if (m_debugging)
        cerr << __FUNCTION__
             << ": (active text): "
             << getAllErrorString()
             << endl;

      m_biInfo[objInstance].activeText = "active";
    }
  else
    {
      m_biInfo[objInstance].activeText = m_instance->getDataTypeString();
    }

  return;
}

void BACNETUTIL::deleteBinaryInputInfo(uint32_t objInstance)
{
  // if there is no data stored for this objInstance yet, then we do
  // not need to do anything.
  if (m_biInfo.count(objInstance) == 0)
    return;

  // Now, we just erase the entry, and it will be updated the next
  // time it is accessed.
  m_biInfo.erase(objInstance);

  return;
}

string BACNETUTIL::lookupBinaryInputText(uint32_t objInstance, bool value)
{
  // cache relevant data if necessary
  updateBinaryInputInfo(objInstance);

 if (value)
    return m_biInfo[objInstance].activeText;
  else
    return m_biInfo[objInstance].inactiveText;
}

string BACNETUTIL::getBinaryInputText(uint32_t objInstance)
{
  bool value = getBinaryInput(objInstance);

  return lookupBinaryInputText(objInstance, value);
}

string BACNETUTIL::lookupBinaryValueText(uint32_t objInstance, bool value)
{
  // cache relevant data if necessary
  updateBinaryValueInfo(objInstance);

 if (value)
    return m_bvInfo[objInstance].activeText;
  else
    return m_bvInfo[objInstance].inactiveText;
}

string BACNETUTIL::getBinaryValueText(uint32_t objInstance)
{
  bool value = getBinaryValue(objInstance);

  return lookupBinaryValueText(objInstance, value);
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

string BACNETUTIL::getDeviceDescription()
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

string BACNETUTIL::getDeviceLocation()
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

bool BACNETUTIL::setDeviceLocation(string location)
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

string BACNETUTIL::getDeviceName()
{
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_DEVICE,
                               m_targetDeviceObjectID, PROP_OBJECT_NAME))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return "";
    }

  return m_instance->getDataTypeString();
}

bool BACNETUTIL::setDeviceName(string name)
{
  if (name.size() < 1)
    {
      throw std::invalid_argument(std::string(__FUNCTION__)
                                  + ": name must have at least one character");
    }

  BACNET_APPLICATION_DATA_VALUE myName =
    m_instance->createDataString(name);

  // write the Device Object Location
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_DEVICE,
                                m_targetDeviceObjectID, PROP_OBJECT_NAME,
                                &myName))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}
