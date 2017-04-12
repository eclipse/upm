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
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <syslog.h>

#include "bacnetmstp.hpp"
#include "handlers.h"
#include "client.h"
#include "txbuf.h"
#include "mstpdef.h"

using namespace upm;
using namespace std;

// our singleton instance
BACNETMSTP* BACNETMSTP::m_instance = 0;

BACNETMSTP::BACNETMSTP()
{
  // set defaults here
  m_maxInfoFrames = DEFAULT_MAX_INFO_FRAMES;
  m_maxMaster = DEFAULT_MAX_MASTER;
  m_baudRate = 38400;
  m_macAddr = DEFAULT_MAX_MASTER;

  m_initialized = false;

  // 60 sec, for MS/TP
  m_adpuTimeoutMS = 60000;

  m_deviceInstanceID = BACNET_MAX_INSTANCE;

  memset(m_rxBuffer, 0, MAX_MPDU);

  m_returnedValue.clear();
  m_targetAddress = {0};
  m_invokeID = 0;
  m_errorDetected = false;

  setDebug(false);
}

BACNETMSTP::~BACNETMSTP()
{
  if (m_initialized)
    datalink_cleanup();
}

void BACNETMSTP::setDebug(bool enable)
{
  m_debugging = enable;
}

void BACNETMSTP::clearErrors()
{
  m_errorType = BACERR_TYPE_NONE;

  // empty out all of our error/reject/abort info
  m_rejectReason = REJECT_REASON_OTHER;
  m_rejectString.clear();

  m_abortReason = ABORT_REASON_OTHER;
  m_abortString.clear();

  m_errorClass = ERROR_CLASS_DEVICE;
  m_errorCode = ERROR_CODE_OTHER;
  m_errorString.clear();

  m_upmErrorString.clear();
}

void BACNETMSTP::handlerError(BACNET_ADDRESS* src,
                              uint8_t invoke_id,
                              BACNET_ERROR_CLASS error_class,
                              BACNET_ERROR_CODE error_code)
{
  if (instance()->m_debugging)
    cerr << __FUNCTION__ << ": entered" << endl;

  if (address_match(&(instance()->m_targetAddress), src) &&
      (invoke_id == instance()->m_invokeID))
    {
      instance()->m_errorType = BACERR_TYPE_ERROR;
      instance()->m_errorClass = error_class;
      instance()->m_errorCode = error_code;
      instance()->m_errorString =
        bactext_error_class_name((int)error_class)
        + string(": ") + bactext_error_code_name((int)error_code);

      instance()->m_errorDetected = true;
  }
}

void BACNETMSTP::handlerAbort(BACNET_ADDRESS* src,
                              uint8_t invoke_id,
                              uint8_t abort_reason,
                              bool server)
{
  (void)server; // not used

  if (instance()->m_debugging)
    cerr << __FUNCTION__ << ": entered" << endl;

  if (address_match(&(instance()->m_targetAddress), src) &&
      (invoke_id == instance()->m_invokeID))
    {
      instance()->m_errorType = BACERR_TYPE_ABORT;
      instance()->m_abortReason = abort_reason;
      instance()->m_abortString =
        bactext_abort_reason_name((int)abort_reason);

      instance()->m_errorDetected = true;
    }
}

void BACNETMSTP::handlerReject(BACNET_ADDRESS* src,
                               uint8_t invoke_id,
                               uint8_t reject_reason)
{
  if (instance()->m_debugging)
    cerr << __FUNCTION__ << ": entered" << endl;

  if (address_match(&(instance()->m_targetAddress), src) &&
      (invoke_id == instance()->m_invokeID))
    {
      instance()->m_errorType = BACERR_TYPE_REJECT;
      instance()->m_rejectReason = reject_reason;
      instance()->m_rejectString =
        bactext_reject_reason_name((int)reject_reason);

      instance()->m_errorDetected = true;
    }
}

void BACNETMSTP::handlerReadPropertyAck(uint8_t* service_request,
                                        uint16_t service_len,
                                        BACNET_ADDRESS* src,
                                        BACNET_CONFIRMED_SERVICE_ACK_DATA* service_data)
{
  int len = 0;
  BACNET_READ_PROPERTY_DATA data;

  // clear our stored data
  instance()->m_returnedValue.clear();

  BACNET_APPLICATION_DATA_VALUE value;
  memset((void *)&value, 0, sizeof(value));

  uint8_t *application_data = 0;
  int application_data_len = 0;

  if (address_match(&(instance()->m_targetAddress), src) &&
      (service_data->invoke_id == instance()->m_invokeID))
    {
      if (instance()->m_debugging)
        cerr << __FUNCTION__ << ": got readProp ack" << endl;

      len = rp_ack_decode_service_request(service_request, service_len,
                                          &data);

      // store any delivered data elements
      if (len > 0)
        {
          application_data_len = data.application_data_len;
          application_data = data.application_data;

          while (true)
            {
              len = bacapp_decode_application_data(application_data,
                                                   application_data_len,
                                                   &value);
              if (len > 0)
                {
                  // store a copy
                  instance()->m_returnedValue.push_back(value);

                  if (len < application_data_len)
                    {
                      // there is more data
                      application_data += len;
                      application_data_len -= len;
                    }
                  else
                    {
                      // we are done
                      break;
                    }
                }
              else
                {
                  // shouldn't happen?
                  cerr << __FUNCTION__ << ": decode app data failed" << endl;
                  break;
                }
            }
        }
    }

  if (instance()->m_debugging)
    cerr << __FUNCTION__ << ": STORED "
         << instance()->m_returnedValue.size()
         << " data elements." << endl;
}

void BACNETMSTP::handlerWritePropertyAck(BACNET_ADDRESS* src,
                                         uint8_t invoke_id)
{
  if (address_match(&(instance()->m_targetAddress), src) &&
      (invoke_id == instance()->m_invokeID))
    {
      if (instance()->m_debugging)
        cerr << __FUNCTION__ << ": got writeProp ack" << endl;
    }
}

void BACNETMSTP::initServiceHandlers()
{
  // this is in device-client.c
  Device_Init(NULL);

  // These are service requests we must handle from other masters

  // we need to handle who-is to support dynamic device binding to us
  apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);

  // handle i-am to support binding to other devices
  apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, handler_i_am_bind);

  // set the handler for all the services we don't implement

  // It is required to send the proper reject message...
  apdu_set_unrecognized_service_handler_handler(handler_unrecognized_service);

  // we must implement read property (it's required)
  apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
                             handler_read_property);

  // These are related to requests we make

  // handle the data coming back from confirmed readProp requests
  apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,
                                 handlerReadPropertyAck);

  // handle the simple ack for confirmed writeProp requests
  apdu_set_confirmed_simple_ack_handler(SERVICE_CONFIRMED_WRITE_PROPERTY,
                                        handlerWritePropertyAck);

  // handle any errors coming back
  apdu_set_error_handler(SERVICE_CONFIRMED_READ_PROPERTY, handlerError);
  apdu_set_abort_handler(handlerAbort);
  apdu_set_reject_handler(handlerReject);
}

BACNETMSTP* BACNETMSTP::instance()
{
  if (!m_instance)
    m_instance = new BACNETMSTP;

  return m_instance;
}

void BACNETMSTP::initMaster(std::string port, int baudRate,
                            int deviceInstanceID, int macAddr, int maxMaster,
                            int maxInfoFrames)
{
  // first some checking

  // if we are already initialized, then it's too late to change things now
  if (m_initialized)
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": Instance is already initialized, ignored."
             << endl;
      return;
    }

  // baudrate
  // The standard allows (as of at least 2010) the following baud rates
  if ( !(baudRate == 9600 || baudRate == 19200 || baudRate == 38400
         || baudRate == 57600 || baudRate == 76800 || baudRate == 115200) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__)
                                  + ": baudRate must be 9600, 19200, 38400, "
                                  + " 57600, 76800, or 115200");
    }

  // maxMaster
  // maxMaster must be less than or equal to 127
  if (maxMaster < 0 || maxMaster > DEFAULT_MAX_MASTER)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": maxMaster must be between 0 and "
                              + std::to_string(DEFAULT_MAX_MASTER));
    }

  // As a master ourselves, we must have a MAC address also within the
  // constraints of maxMaster
  if (macAddr < 0 || macAddr > DEFAULT_MAX_MASTER)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": macAddr must be between 0 and "
                              + std::to_string(DEFAULT_MAX_MASTER));
    }

  // this should be unique on the network
  if (deviceInstanceID >= BACNET_MAX_INSTANCE)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": deviceInstanceID must be less than "
                              + std::to_string(BACNET_MAX_INSTANCE)
                              + ", and must be unique on the network");
    }

  m_port = port;
  m_baudRate = baudRate;
  m_maxInfoFrames = maxInfoFrames;
  m_macAddr = macAddr;
  m_maxMaster = maxMaster;
  m_deviceInstanceID = deviceInstanceID;

  // Let the fun begin...

  // setup our info
  Device_Set_Object_Instance_Number(m_deviceInstanceID);
  address_init();

  initServiceHandlers();

  dlmstp_set_max_info_frames(m_maxInfoFrames);
  dlmstp_set_max_master(m_maxMaster);
  dlmstp_set_baud_rate(m_baudRate);
  dlmstp_set_mac_address(m_macAddr);

  // FIXME - allow to change?
  apdu_timeout_set(m_adpuTimeoutMS);

  // Ordinarily, I'd like to check the return value of this function,
  // but even in the face of errors, it always returns true :( This
  // function starts the ball rolling, and initializes the Master FSM
  // thread.  Unfortunately, it doesn't appear this can be turned back
  // off without exiting the application.
  datalink_init((char *)port.c_str());

  m_initialized = true;
}

bool BACNETMSTP::dispatchRequest()
{
  uint16_t pdu_len = 0;
  unsigned timeout = 100;     // milliseconds
  unsigned max_apdu = 0;
  time_t elapsed_seconds = 0;
  time_t last_seconds = 0;
  time_t current_seconds = 0;
  time_t timeout_seconds = 0;
  bool found = false;

  // address where message came from
  BACNET_ADDRESS src = {0};

  clearErrors();
  m_errorDetected = false;

  uint32_t targetDeviceInstanceID = BACNET_MAX_INSTANCE;

  switch (m_command.cmd)
    {
    case BACCMD_READ_PROPERTY:
      targetDeviceInstanceID = m_command.readPropArgs.targetDeviceInstanceID;
      break;

    case BACCMD_WRITE_PROPERTY:
      targetDeviceInstanceID = m_command.writePropArgs.targetDeviceInstanceID;
      break;

    case BACCMD_NONE:
      {
        m_errorType = BACERR_TYPE_UPM;
        m_upmErrorString = string(__FUNCTION__) +
          ": called with BACCMD_NONE, ignoring";

        return true; // error
      }

      break;

    default:
      {
        // should this throw?
        m_errorType = BACERR_TYPE_UPM;
        m_upmErrorString = string(__FUNCTION__) +
          ": internal error, called with unknown command, ignoring";

        return true; // error
      }

      break;
    }

  // timeouts
  last_seconds = time(NULL);
  timeout_seconds = (apdu_timeout() / 1000) * apdu_retries();

  // we use 0 to indicate that request hasn't been made yet, so that
  // it will be made once the address is bound.
  m_invokeID = 0;

  // bind to the device first.
  found = address_bind_request(targetDeviceInstanceID, &max_apdu,
                               &(instance()->m_targetAddress));

  if (!found)
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": Address not found, Sending WhoIs..." << endl;

      Send_WhoIs(targetDeviceInstanceID, targetDeviceInstanceID);
    }
  else
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": Address was found" << endl;
    }

  // loop until either we get our data, an error occurs, or we timeout
  while (true)
    {
      current_seconds = time(NULL);

      // at least one second has passed
      if (current_seconds != last_seconds)
        tsm_timer_milliseconds((uint16_t) ((current_seconds -
                                            last_seconds) * 1000));
      if (m_errorDetected)
        break;

      // we have to wait until the address is bound before proceeding
      if (!found)
        {
          found =
            address_bind_request(targetDeviceInstanceID, &max_apdu,
                                 &(instance()->m_targetAddress));
        }

      if (found)
        {
          // address is bound, and we have not sent our request yet.  Make it so.
          if (m_invokeID == 0)
            {
              switch (m_command.cmd)
                {
                case BACCMD_READ_PROPERTY:
                  m_invokeID =
                    Send_Read_Property_Request(targetDeviceInstanceID,
                                               m_command.readPropArgs.objType,
                                               m_command.readPropArgs.objInstance,
                                               m_command.readPropArgs.objProperty,
                                               m_command.readPropArgs.arrayIndex);
                  if (m_debugging)
                    cerr << __FUNCTION__
                         << ": Called Send_Read_Property_Request(), m_invokeID = "
                         << (int)m_invokeID << endl;

                  break;

                case BACCMD_WRITE_PROPERTY:
                  m_invokeID =
                    Send_Write_Property_Request(targetDeviceInstanceID,
                                                m_command.writePropArgs.objType,
                                                m_command.writePropArgs.objInstance,
                                                m_command.writePropArgs.objProperty,
                                                m_command.writePropArgs.propValue,
                                                m_command.writePropArgs.propPriority,
                                                m_command.writePropArgs.arrayIndex);
                  if (m_debugging)
                    cerr << __FUNCTION__
                         << ": Called Send_Write_Property_Request(), m_invokeID = "
                         << (int)m_invokeID << endl;

                  break;
                default:
                  syslog(LOG_WARNING, "%s: switch case not defined",
                          std::string(__FUNCTION__).c_str());
                }
            }
          else if (tsm_invoke_id_free(m_invokeID))
            {
              // transaction completed successfully

              if (m_debugging)
                cerr << __FUNCTION__ << ": Success, m_invokeID = "
                     << (int)m_invokeID << endl;

              break;
            }
          else if (tsm_invoke_id_failed(m_invokeID))
            {
              // transaction state machine failed, most likely timeout
              tsm_free_invoke_id(m_invokeID);

              m_errorType = BACERR_TYPE_UPM;
              m_upmErrorString = string(__FUNCTION__) +
                ": TSM Timed Out.";

              if (m_debugging)
                cerr << m_upmErrorString << endl;

              m_errorDetected = true;

              break;
            }
        }
      else
        {
          // still waiting to bind.  timeout if we've waited too long.
          elapsed_seconds += (current_seconds - last_seconds);
          if (elapsed_seconds > timeout_seconds)
            {
              m_errorType = BACERR_TYPE_UPM;
              m_upmErrorString = string(__FUNCTION__) +
                ": Timed out waiting to bind address.";

              // We output this error unconditionally as this is an
              // error you will get if you supply a non-existant
              // Device Obeject Instance ID.

              cerr << m_upmErrorString << endl;
              cerr << __FUNCTION__
                   << ": Did you supply the correct Device Object Instance ID "
                   << "for your device?"
                   << endl;

              m_errorDetected = true;
              break;
            }
        }

      // returns 0 bytes on timeout
      pdu_len = datalink_receive(&src, m_rxBuffer, MAX_MPDU, timeout);

      // process the packet if valid.  This will call our handlers as needed.
      if (pdu_len)
        npdu_handler(&src, m_rxBuffer, pdu_len);

      // keep track of time for next check
      last_seconds = current_seconds;
    }

  return m_errorDetected;
}

bool BACNETMSTP::readProperty(uint32_t targetDeviceInstanceID,
                              BACNET_OBJECT_TYPE objType,
                              uint32_t objInstance,
                              BACNET_PROPERTY_ID objProperty,
                              uint32_t arrayIndex)
{
  // some sanity checking...
  if (objInstance >= BACNET_MAX_INSTANCE)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": objInstance must be less than "
                              + std::to_string(BACNET_MAX_INSTANCE));
    }

  // fill in the command structure and dispatch
  m_command.cmd = BACCMD_READ_PROPERTY;
  m_command.readPropArgs.targetDeviceInstanceID = targetDeviceInstanceID;
  m_command.readPropArgs.objType = objType;
  m_command.readPropArgs.objInstance = objInstance;
  m_command.readPropArgs.objProperty = objProperty;
  m_command.readPropArgs.arrayIndex = arrayIndex;

  if (m_debugging)
    cerr << __FUNCTION__  << ": calling dispatchRequest()..." << endl;

  // send it off
  bool error = dispatchRequest();

  // clear the command to avoid accidental re-calls
  m_command.cmd = BACCMD_NONE;

  return error;
}

bool BACNETMSTP::writeProperty(uint32_t targetDeviceInstanceID,
                               BACNET_OBJECT_TYPE objType,
                               uint32_t objInstance,
                               BACNET_PROPERTY_ID objProperty,
                               BACNET_APPLICATION_DATA_VALUE* propValue,
                               uint8_t propPriority,
                               int32_t arrayIndex)
{
  // some sanity checking...
  if (objInstance >= BACNET_MAX_INSTANCE)
    {
      throw std::out_of_range(std::string(__FUNCTION__)
                              + ": objInstance must be less than "
                              + std::to_string(BACNET_MAX_INSTANCE));
    }

  // fill in the command structure and dispatch
  m_command.cmd = BACCMD_WRITE_PROPERTY;
  m_command.writePropArgs.targetDeviceInstanceID = targetDeviceInstanceID;
  m_command.writePropArgs.objType = objType;
  m_command.writePropArgs.objInstance = objInstance;
  m_command.writePropArgs.objProperty = objProperty;
  m_command.writePropArgs.propValue = propValue;
  m_command.writePropArgs.propPriority = propPriority;
  m_command.writePropArgs.arrayIndex = arrayIndex;

  if (m_debugging)
    cerr << __FUNCTION__  << ": calling dispatchRequest()..." << endl;

  // send it off
  bool error = dispatchRequest();

  // clear the command to avoid accidental re-calls
  m_command.cmd = BACCMD_NONE;

  return error;
}

BACNET_APPLICATION_DATA_VALUE BACNETMSTP::getData(int index)
{
  return m_returnedValue.at(index);
}

int BACNETMSTP::getDataNumElements()
{
  return m_returnedValue.size();
}

uint8_t BACNETMSTP::getDataType(int index)
{
  return m_returnedValue.at(index).tag;
}

float BACNETMSTP::getDataTypeReal(int index)
{
  if (getDataType(index) == BACNET_APPLICATION_TAG_REAL)
    return m_returnedValue.at(index).type.Real;
  else
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": Not of Real type, trying to convert..." << endl;

      // try to convert or throw
      switch (getDataType(index))
        {
        case BACNET_APPLICATION_TAG_BOOLEAN:
          return (getDataTypeBoolean(index) ? 1.0 : 0.0);
        case BACNET_APPLICATION_TAG_UNSIGNED_INT:
          return float(getDataTypeUnsignedInt(index));
        case BACNET_APPLICATION_TAG_SIGNED_INT:
          return float(getDataTypeSignedInt(index));
        default:
          throw std::invalid_argument(std::string(__FUNCTION__)
                                      + ": data type ("
                                      + std::to_string(int(getDataType(index)))
                                      + ") is not convertible to Real");
        }
    }
}

bool BACNETMSTP::getDataTypeBoolean(int index)
{
  if (getDataType(index) == BACNET_APPLICATION_TAG_BOOLEAN)
    return ((m_returnedValue.at(index).type.Boolean) ? true : false);
  else
    throw std::invalid_argument(std::string(__FUNCTION__)
                                + ": data type ("
                                + std::to_string(int(getDataType(index)))
                                + ") is not convertible to Bool");
}

unsigned int BACNETMSTP::getDataTypeUnsignedInt(int index)
{
  if (getDataType(index) == BACNET_APPLICATION_TAG_UNSIGNED_INT)
    return m_returnedValue.at(index).type.Unsigned_Int;
  else
    throw std::invalid_argument(std::string(__FUNCTION__)
                                + ": data type ("
                                + std::to_string(int(getDataType(index)))
                                + ") is not convertible to UnsignedInt");
}

int BACNETMSTP::getDataTypeSignedInt(int index)
{
  if (getDataType(index) == BACNET_APPLICATION_TAG_SIGNED_INT)
    return m_returnedValue.at(index).type.Signed_Int;
  else
    throw std::invalid_argument(std::string(__FUNCTION__)
                                + ": data type ("
                                + std::to_string(int(getDataType(index)))
                                + ") is not convertible to SignedInt");
}

#if defined(BACAPP_DOUBLE)
double BACNETMSTP::getDataTypeDouble(int index)
{
  if (getDataType() == BACNET_APPLICATION_TAG_DOUBLE)
    return m_returnedValue.at(index).type.Double;
  else
    {
      if (m_debugging)
        cerr << __FUNCTION__ << ": Not of Double type, trying to convert..." << endl;

      // try to convert or throw
      switch (getDataType(index))
        {
        case BACNET_APPLICATION_TAG_REAL:
          return double(getDataTypeReal(index));
        case BACNET_APPLICATION_TAG_BOOLEAN:
          return (getDataTypeBoolean(index) ? 1.0 : 0.0);
        case BACNET_APPLICATION_TAG_UNSIGNED_INT:
          return double(getDataTypeUnsignedInt(index));
        case BACNET_APPLICATION_TAG_SIGNED_INT:
          return double(getDataTypeSignedInt(index));
        default:
          throw std::invalid_argument(std::string(__FUNCTION__)
                                      + ": data type ("
                                      + std::to_string(int(getDataType(index)))
                                      + ") is not convertible to Double");
        }
    }
}
#endif // BACAPP_DOUBLE

unsigned int BACNETMSTP::getDataTypeEnum(int index)
{
  if (getDataType(index) == BACNET_APPLICATION_TAG_ENUMERATED)
    return m_returnedValue.at(index).type.Enumerated;
  else
    throw std::invalid_argument(std::string(__FUNCTION__)
                                + ": data type ("
                                + std::to_string(int(getDataType(index)))
                                + ") is not convertible to Enum");
}

std::string BACNETMSTP::getDataTypeString(int index)
{
  string retval;

  // Here, we can try to accomodate all the types
  switch(getDataType(index))
    {
    case BACNET_APPLICATION_TAG_REAL:
      retval = std::to_string(getDataTypeReal(index));
      break;

#if defined(BACAPP_DOUBLE)
    case BACNET_APPLICATION_TAG_DOUBLE:
      retval = std::to_string(getDataTypeDouble(index));
      break;
#endif // BACAPP_DOUBLE

    case BACNET_APPLICATION_TAG_UNSIGNED_INT:
      retval = std::to_string(getDataTypeUnsignedInt(index));
      break;

    case BACNET_APPLICATION_TAG_SIGNED_INT:
      retval = std::to_string(getDataTypeSignedInt(index));
      break;

    case BACNET_APPLICATION_TAG_BOOLEAN:
      retval = (getDataTypeBoolean(index) ? string("true") : string("false"));
      break;

    case BACNET_APPLICATION_TAG_CHARACTER_STRING:
      retval = string(characterstring_value(&m_returnedValue.at(index).type.Character_String),

                      characterstring_length(&m_returnedValue.at(index).type.Character_String));
      break;

    case BACNET_APPLICATION_TAG_OCTET_STRING:
      {
        string tmpstr((char *)octetstring_value(&m_returnedValue.at(index).type.Octet_String),

                      octetstring_length(&m_returnedValue.at(index).type.Octet_String));
        retval = string2HexString(tmpstr);
      }

      break;

    case BACNET_APPLICATION_TAG_BIT_STRING:
      {
        int len = bitstring_bits_used(&m_returnedValue.at(index).type.Bit_String);

        for (int i=0; i<len; i++)
          {
            if (bitstring_bit(&m_returnedValue.at(index).type.Bit_String,
                              uint8_t(i)))
              retval += "1";
            else
              retval += "0";

            if (i != 0 && ((i % 8) == 0))
              retval += " ";
          }
      }

      break;

    default:
      throw std::invalid_argument(std::string(__FUNCTION__)
                                  + ": data type ("
                                  + std::to_string(int(getDataType(index)))
                                  + ") is not convertible to String");
      break;
    }

  return retval;
}

BACNET_APPLICATION_DATA_VALUE BACNETMSTP::createDataReal(float value)
{
  BACNET_APPLICATION_DATA_VALUE data;

  memset(&data, 0, sizeof(BACNET_APPLICATION_DATA_VALUE));

  data.tag = BACNET_APPLICATION_TAG_REAL;
  data.type.Real = value;

  return data;
}

BACNET_APPLICATION_DATA_VALUE BACNETMSTP::createDataBool(bool value)
{
  BACNET_APPLICATION_DATA_VALUE data;

  memset(&data, 0, sizeof(BACNET_APPLICATION_DATA_VALUE));

  data.tag = BACNET_APPLICATION_TAG_BOOLEAN;
  data.type.Boolean = value;

  return data;
}

BACNET_APPLICATION_DATA_VALUE BACNETMSTP::createDataSignedInt(int value)
{
  BACNET_APPLICATION_DATA_VALUE data;

  memset(&data, 0, sizeof(BACNET_APPLICATION_DATA_VALUE));

  data.tag = BACNET_APPLICATION_TAG_SIGNED_INT;
  data.type.Signed_Int = value;

  return data;
}

BACNET_APPLICATION_DATA_VALUE BACNETMSTP::createDataUnsignedInt(unsigned int value)
{
  BACNET_APPLICATION_DATA_VALUE data;

  memset(&data, 0, sizeof(BACNET_APPLICATION_DATA_VALUE));

  data.tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
  data.type.Unsigned_Int = value;

  return data;
}

BACNET_APPLICATION_DATA_VALUE BACNETMSTP::createDataString(string value)
{
  if (value.size() > (MAX_CHARACTER_STRING_BYTES - 1))
    {
      throw std::invalid_argument(std::string(__FUNCTION__)
                                  + ": value must be less than or equal to "
                                  + std::to_string(MAX_CHARACTER_STRING_BYTES - 1)
                                  + " characters long");
    }

  BACNET_APPLICATION_DATA_VALUE data;

  memset(&data, 0, sizeof(BACNET_APPLICATION_DATA_VALUE));

  data.tag = BACNET_APPLICATION_TAG_CHARACTER_STRING;

  characterstring_init_ansi(&data.type.Character_String, value.c_str());

  return data;
}

BACNET_APPLICATION_DATA_VALUE BACNETMSTP::createDataEnum(uint32_t value)
{
  BACNET_APPLICATION_DATA_VALUE data;

  memset(&data, 0, sizeof(BACNET_APPLICATION_DATA_VALUE));

  data.tag = BACNET_APPLICATION_TAG_ENUMERATED;
  data.type.Enumerated = value;

  return data;
}

string BACNETMSTP::string2HexString(string input)
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
