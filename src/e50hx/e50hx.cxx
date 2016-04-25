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

#include "e50hx.hpp"

using namespace upm;
using namespace std;


E50HX::E50HX(uint32_t targetDeviceObjectID) :
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

E50HX::~E50HX()
{
}

void E50HX::initMaster(std::string port, int baudRate,
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

void E50HX::setDebug(bool enable)
{
  m_debugging = enable;

  // we also enable/disable debugging in BACNETMSTP
  m_instance->setDebug(enable);
}

float E50HX::getAnalogValue(ANALOG_VALUES_T objInstance)
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

          return RETURN_ERROR;
        }

      BACNET_RELIABILITY reliable =
        static_cast<BACNET_RELIABILITY>(m_instance->getDataTypeEnum());

      if (reliable != RELIABILITY_NO_FAULT_DETECTED)
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": Reliability check failed" << endl;

          return RETURN_UNRELIABLE;
        }
    }

  // now get the value
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                               objInstance, PROP_PRESENT_VALUE))
    {
          if (m_debugging)
            cerr << __FUNCTION__ << ": (value): " << getAllErrorString()
                 << endl;

      return RETURN_ERROR;
    }

  return m_instance->getDataTypeReal();
}

string E50HX::getAnalogValueUnits(ANALOG_VALUES_T objInstance)
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

float E50HX::getAnalogInput(ANALOG_INPUTS_T objInstance)
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

          return RETURN_ERROR;
        }

      BACNET_RELIABILITY reliable =
        static_cast<BACNET_RELIABILITY>(m_instance->getDataTypeEnum());

      if (reliable != RELIABILITY_NO_FAULT_DETECTED)
        {
          if (m_debugging)
            cerr << __FUNCTION__ << ": Reliability check failed" << endl;

          return RETURN_UNRELIABLE;
        }
    }

  // now get the value
  if (m_instance->readProperty(m_targetDeviceObjectID, OBJECT_ANALOG_INPUT,
                               objInstance, PROP_PRESENT_VALUE))
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": (value): " << getAllErrorString() << endl;

      return RETURN_ERROR;
    }

  return m_instance->getDataTypeReal();
}

string E50HX::getAnalogInputUnits(ANALOG_INPUTS_T objInstance)
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

uint16_t E50HX::getAlarmBits()
{
  return uint16_t(getAnalogInput(AI_Alarm_Bitmap));
}

BACNETMSTP::BACERR_TYPE_T E50HX::getErrorType()
{
  return m_instance->getErrorType();
}

uint8_t E50HX::getRejectReason()
{
  return m_instance->getRejectReason();
}

std::string E50HX::getRejectString()
{
  return m_instance->getRejectString();
}

uint8_t E50HX::getAbortReason()
{
  return m_instance->getAbortReason();
}

std::string E50HX::getAbortString()
{
  return m_instance->getAbortString();
}

BACNET_ERROR_CLASS E50HX::getErrorClass()
{
  return m_instance->getErrorClass();
}

BACNET_ERROR_CODE E50HX::getErrorCode()
{
  return m_instance->getErrorCode();
}

std::string E50HX::getUPMErrorString()
{
  return m_instance->getUPMErrorString();
}

std::string E50HX::getErrorString()
{
  return m_instance->getErrorString();
};


string E50HX::getAllErrorString()
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

string E50HX::getDescription()
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

string E50HX::getLocation()
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

bool E50HX::setLocation(string location)
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

bool E50HX::writeConfig(CFG_VALUES_T config)
{
  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(float(config));

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_Config, PROP_PRESENT_VALUE, &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writeSystemType(SYSTEM_TYPES_T systype)
{
  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(float(systype));

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_System_Type, PROP_PRESENT_VALUE, &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writeCTRatioPrimary(float ctRatio)
{
  if (ctRatio < 5 || ctRatio > 32000)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": ctRatio must be between 5-32000");
    }

  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(ctRatio);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_CT_Ratio_Primary, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writeCTRatioSecondary(CT_SECONDARY_T ctRatio)
{
  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(float(ctRatio));

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_CT_Ratio_Secondary, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writePTRatio(float ptRatio)
{
  if (ptRatio < 0.01 || ptRatio > 320.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": ptRatio must be between 0.01-320.0");
    }

  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(ptRatio);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_PT_Ratio, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writeSystemVoltage(float sysVolts)
{
  if (sysVolts < 82.0 || sysVolts > 32000.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": sysVolts must be between 82.0-32000.0");
    }

  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(sysVolts);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_System_Voltage, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writeDisplayUnits(DISP_UNITS_T dispUnits)
{
  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(float(dispUnits));

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_Display_Units, PROP_PRESENT_VALUE,
                                &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writePhaseLossVT(float phaseLoss)
{
  if (phaseLoss < 1.0 || phaseLoss > 99.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": phaseLoss must be between 1.0-99.0");
    }

  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(phaseLoss);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_Phase_Loss_Voltage_Threshold,
                                PROP_PRESENT_VALUE, &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}

bool E50HX::writePhaseLossIT(float phaseLoss)
{
  if (phaseLoss < 1.0 || phaseLoss > 99.0)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": phaseLoss must be between 1.0-99.0");
    }

  // Write the value
  BACNET_APPLICATION_DATA_VALUE myData =
    m_instance->createDataReal(phaseLoss);

  // write it
  if (m_instance->writeProperty(m_targetDeviceObjectID, OBJECT_ANALOG_VALUE,
                                AV_Phase_Loss_Imbalance_Threshold,
                                PROP_PRESENT_VALUE, &myData))
    {
      // error occurred
      if (m_debugging)
        cerr << __FUNCTION__ << ": " << getAllErrorString() << endl;

      return false;
    }

  return true;
}
