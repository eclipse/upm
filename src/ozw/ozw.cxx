/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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
#include <pthread.h>

#include "platform/Log.h"

#include "zwNode.hpp"

#include "ozw.hpp"

using namespace upm;
using namespace std;
using namespace OpenZWave;

// our singleton instance
OZW* OZW::m_instance = 0;

OZW::OZW()
{
  m_initialized = false;
  m_mgrCreated = false;
  m_driverFailed = false;
  m_homeId = 0;

  pthread_mutexattr_t mutexAttrib;
  pthread_mutexattr_init(&mutexAttrib);
  pthread_mutexattr_settype(&mutexAttrib, PTHREAD_MUTEX_RECURSIVE);

  if (pthread_mutex_init(&m_nodeLock, &mutexAttrib))
    {
      pthread_mutexattr_destroy(&mutexAttrib);
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": pthread_mutex_init(nodeLock) failed");
    }

  pthread_mutexattr_destroy(&mutexAttrib);

  if (pthread_mutex_init(&m_initLock, NULL))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": pthread_mutex_init(initLock) failed");
    }

  // initialize our init conditional
  if (pthread_cond_init(&m_initCond, NULL))
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": pthread_cond_init() failed");
    }

  setDebug(false);
}

OZW::~OZW()
{
  if (m_mgrCreated)
    {
      // remove the driver
      if (m_driverIsHID)
        Manager::Get()->RemoveDriver("HID");
      else
        Manager::Get()->RemoveDriver(m_devicePath);
        
      // remove the notification handler
      Manager::Get()->RemoveWatcher(notificationHandler, this);

      // now destroy Manager and Options.  Options must be destroyed
      // after the Manager is destroyed.
      Manager::Destroy();
      Options::Destroy();
    }

  pthread_mutex_destroy(&m_nodeLock);
  pthread_mutex_destroy(&m_initLock);
  pthread_cond_destroy(&m_initCond);

  // delete any nodes.  This should be safe after deleting the node
  // mutex since the handler is no longer registered.
  for (auto it = m_zwNodeMap.cbegin();
       it != m_zwNodeMap.cend(); ++it)
    {
      // delete the zwNode pointer
      delete (*it).second;
    }
  // empty the map
  m_zwNodeMap.clear();
}

OZW* OZW::instance()
{
  if (!m_instance)
    m_instance = new OZW;

  return m_instance;
}

void OZW::optionsCreate(std::string configPath, 
                        std::string userConfigDir,
                        std::string cmdLine)
{
  Options::Create(configPath, userConfigDir, cmdLine);
}

void OZW::optionAddInt(std::string name, int val)
{
  if (!Options::Get()->AreLocked())
    Options::Get()->AddOptionInt(name, val);
}

void OZW::optionAddBool(std::string name, bool val)
{
  if (!Options::Get()->AreLocked())
    Options::Get()->AddOptionBool(name, val);
}

void OZW::optionAddString(std::string name, std::string val, bool append)
{
  if (!Options::Get()->AreLocked())
    Options::Get()->AddOptionString(name, val, append);
}

void OZW::optionsLock()
{
  // lock the options if not already locked
  if (!Options::Get()->AreLocked())
    Options::Get()->Lock();
}

bool OZW::init(string devicePath, bool isHID)
{
  if (m_initialized)
    {
      // we are already initialized...
      if (m_debugging)
        cerr << __FUNCTION__ << ": Already initialized, continuing..."
             << endl;

      return true;
    }

  // make sure options are locked
  optionsLock();

  pthread_mutex_lock(&m_initLock);

  // the fun begins
  Manager::Create();

  // add our event handler
  Manager::Get()->AddWatcher(notificationHandler, this);

  // now add the driver
  m_devicePath = devicePath;
  if (isHID)
    {
      m_driverIsHID = true;
      Manager::Get()->AddDriver("HID",
                                Driver::ControllerInterface_Hid);
    }
  else
    Manager::Get()->AddDriver(devicePath);

  m_mgrCreated = true;

  // now we block here waiting for the driver to get far enough along
  // (or fail) to proceed further
  pthread_cond_wait(&m_initCond, &m_initLock);

  if (m_driverFailed)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": driver initialization failed");
      return false;
    }

  // if we succeeded, update (sort) all of the VIDs in each zwNode, and
  // enable autoupdating from here on out.

  lockNodes();
  for (auto it = m_zwNodeMap.cbegin();
       it != m_zwNodeMap.cend(); ++it)
    {
      (*it).second->updateVIDMap();
      (*it).second->setAutoUpdate(true);
    }
  unlockNodes();

  m_initialized = true;
  return true;
}

void OZW::notificationHandler(Notification const* notification, void *ctx)
{
  upm::OZW *This = (upm::OZW *)ctx;
  
  This->lockNodes();

  if (This->m_debugging)
    fprintf(stderr, "### %s: homeId %08x, nodeId %d, type %x\n",
            __FUNCTION__,
            notification->GetHomeId(),
            notification->GetNodeId(),
            notification->GetType());
  
  const uint32_t homeId = notification->GetHomeId();
  const uint8_t nodeId = notification->GetNodeId();

  switch (notification->GetType()) 
    {

    case Notification::Type_NodeAdded:
    case Notification::Type_NodeNew:
      {
        if (This->m_debugging)
          cerr << "### ### ADDING NODE: " << int(nodeId) << endl;
        zwNode *node = new zwNode(homeId, nodeId);
        This->m_zwNodeMap.insert(std::pair<uint8_t, zwNode *>(nodeId, node));

        break;
      }

    case Notification::Type_NodeRemoved:
      { 
        if (This->m_debugging)
          cerr << "### ### REMOVING NODE: " << int(nodeId) << endl;
        if (This->m_zwNodeMap.count(nodeId) != 0)
          {
            delete This->m_zwNodeMap[nodeId];
            This->m_zwNodeMap.erase(nodeId);
          }

        break;
      }

    case Notification::Type_ValueAdded:
      {
        if (This->m_debugging)
          cerr << "### ### VALUE ADDED " << endl;
        This->m_zwNodeMap[nodeId]->addValueID(notification->GetValueID());

        break;
      }
      
    case Notification::Type_ValueRemoved:
      {
        if (This->m_debugging)
          cerr << "### ### VALUE DELETED " << endl;
        This->m_zwNodeMap[nodeId]->removeValueID(notification->GetValueID());

        break;
      }

    case Notification::Type_ValueChanged:
      {
        // might be able to do something with this someday...
        break;
      }

    case Notification::Type_DriverReset:
      {
        // all nodes deleted.  According to OZW docs, this happens
        // when a driver is reset, instead of sending potentially
        // hundreds of ValueRemoved/NodeRemoved events.
        for (auto it = This->m_zwNodeMap.begin();
             it != This->m_zwNodeMap.end(); ++it)
          {
            // delete the zwNode pointer
            delete (*it).second;
          }
        // empty the map
        This->m_zwNodeMap.clear();

        break;
      }

    case Notification::Type_DriverReady:
      {
        if (This->m_debugging)
          fprintf(stderr, "### DriverReady, homeID = %08x\n", This->m_homeId);
        This->m_homeId = notification->GetHomeId();
        break;
      }

    case Notification::Type_DriverFailed:
      {
        if (This->m_debugging)
          cerr << "### Driver FAILED" << endl;
        This->m_driverFailed = true;
        // wake up init()
        pthread_cond_broadcast(&(This->m_initCond));
        break;
      }

    case Notification::Type_AwakeNodesQueried:
    case Notification::Type_AllNodesQueried:
    case Notification::Type_AllNodesQueriedSomeDead:
      {
        if (This->m_debugging)
          cerr << "### Awake/All/SomeDead complete" << endl;
        // wake up init()
        pthread_cond_broadcast(&(This->m_initCond));
        break;
      }

      // might be able to do something with these someday too
    case Notification::Type_Notification:
    case Notification::Type_NodeNaming:
    case Notification::Type_NodeProtocolInfo:
    case Notification::Type_NodeQueriesComplete:
    case Notification::Type_PollingEnabled:
    case Notification::Type_PollingDisabled:
    case Notification::Type_NodeEvent:
    case Notification::Type_Group:
    default:
      {
        break;
      }
    }

  This->unlockNodes();
}

void OZW::dumpNodes(bool all)
{
  // iterate through all the nodes and dump various info on them

  lockNodes();

  for (auto it = m_zwNodeMap.cbegin();
       it != m_zwNodeMap.cend(); ++it)
    {
      uint8_t nodeId = (*it).first;

      cerr << "Node " << int(nodeId)
           << ": "
           << Manager::Get()->GetNodeProductName(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Type: "
           << Manager::Get()->GetNodeType(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Product Type: "
           << Manager::Get()->GetNodeProductType(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Manufacturer ID: "
           << Manager::Get()->GetNodeManufacturerId(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Product ID: "
           << Manager::Get()->GetNodeProductId(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Generic Type: "
           << (int)Manager::Get()->GetNodeGeneric(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Device Type: "
           << (int)Manager::Get()->GetNodeDeviceType(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Node Basic: "
           << (int)Manager::Get()->GetNodeBasic(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Node Query Stage: "
           << Manager::Get()->GetNodeQueryStage(m_homeId, nodeId)
           << endl;
      cerr << "\t"
           << "Is Node Info Rcvd: "
           << Manager::Get()->IsNodeInfoReceived(m_homeId, nodeId)
           << endl;

      (*it).second->dumpNode(all);
    }

  unlockNodes();
}

bool OZW::getValueID(int nodeId, int index, ValueID *vid)
{
  // nodeId's are uint8_t's in OpenZWave, but we want to use int's to
  // avoid hassles when dealing with SWIG, so here we just throw away
  // everything except the first byte.
  nodeId &= 0xff;

  lockNodes();

  zwNodeMap_t::iterator it;

  it = m_zwNodeMap.find(nodeId);

  if (it == m_zwNodeMap.end())
    {
      cerr << __FUNCTION__ << ": Node " << nodeId 
           << " does not exist" << endl;
      unlockNodes();
      return false;
    }

  // now get the ValueID
  if (!(*it).second->indexToValueID(index, vid))
    {
      cerr << __FUNCTION__ << ": Index " << index 
           << " for node " << nodeId 
           << " does not exist" << endl;
      unlockNodes();
      return false;
    }

  unlockNodes();
  return true;
}

string OZW::getValueAsString(int nodeId, int index)
{
  // we have to play this game since there is no default ctor for ValueID
  ValueID vid(m_homeId, (uint64)0);

  string rv;

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    Manager::Get()->GetValueAsString(vid, &rv);

  unlockNodes();

  return rv;
}

string OZW::getValueUnits(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  string rv;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->GetValueUnits(vid);

  unlockNodes();

  return rv;
}

void OZW::setValueUnits(int nodeId, int index, string text)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    Manager::Get()->SetValueUnits(vid, text);

  unlockNodes();
}

string OZW::getValueLabel(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  string rv;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->GetValueLabel(vid);

  unlockNodes();

  return rv;
}

void OZW::setValueLabel(int nodeId, int index, string text)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    Manager::Get()->SetValueLabel(vid, text);

  unlockNodes();
}

string OZW::getValueHelp(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  string rv;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->GetValueHelp(vid);

  unlockNodes();

  return rv;
}

void OZW::setValueHelp(int nodeId, int index, string text)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    Manager::Get()->SetValueHelp(vid, text);

  unlockNodes();
}

void OZW::setValueAsBool(int nodeId, int index, bool val)
{
  if (isValueReadOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is ReadOnly" << endl;
      return;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->SetValue(vid, val))
        {
          cerr << __FUNCTION__ << ": Value is not a bool type" << endl;
        }
    }

  unlockNodes();
}

void OZW::setValueAsByte(int nodeId, int index, uint8_t val)
{
  if (isValueReadOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is ReadOnly" << endl;
      return;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->SetValue(vid, val))
        {
          cerr << __FUNCTION__ << ": Value is not a byte type" << endl;
        }
    }

  unlockNodes();
}

void OZW::setValueAsFloat(int nodeId, int index, float val)
{
  if (isValueReadOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is ReadOnly" << endl;
      return;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->SetValue(vid, val))
        {
          cerr << __FUNCTION__ << ": Value is not a float type" << endl;
        }
    }

  unlockNodes();
}

void OZW::setValueAsInt32(int nodeId, int index, int32_t val)
{
  if (isValueReadOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is ReadOnly" << endl;
      return;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->SetValue(vid, val))
        {
          cerr << __FUNCTION__ << ": Value is not a int32 type" << endl;
        }
    }

  unlockNodes();
}

void OZW::setValueAsInt16(int nodeId, int index, int16_t val)
{
  if (isValueReadOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is ReadOnly" << endl;
      return;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->SetValue(vid, val))
        {
          cerr << __FUNCTION__ << ": Value is not a int16 type" << endl;
        }
    }

  unlockNodes();
}

void OZW::setValueAsBytes(int nodeId, int index, uint8_t *val, uint8_t len)
{
  if (isValueReadOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is ReadOnly" << endl;
      return;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->SetValue(vid, val, len))
        {
          cerr << __FUNCTION__ << ": Value is not a bytes type" << endl;
        }
    }

  unlockNodes();
}

void OZW::setValueAsString(int nodeId, int index, string val)
{
  if (isValueReadOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is ReadOnly" << endl;
      return;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->SetValue(vid, val))
        {
          // this should always succeed, but for consistancy...
          cerr << __FUNCTION__ << ": Value is not a string type" << endl;
        }
    }

  unlockNodes();
}

void OZW::refreshValue(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  if (getValueID(nodeId, index, &vid))
    Manager::Get()->RefreshValue(vid);

  unlockNodes();
}

int OZW::getValueMin(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  int rv = 0;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->GetValueMin(vid);

  unlockNodes();
  return rv;
}

int OZW::getValueMax(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  int rv = 0;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->GetValueMax(vid);

  unlockNodes();
  return rv;
}

bool OZW::isValueReadOnly(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  bool rv = false;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->IsValueReadOnly(vid);

  unlockNodes();
  return rv;
}

bool OZW::isValueWriteOnly(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  bool rv = false;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->IsValueWriteOnly(vid);

  unlockNodes();
  return rv;
}

bool OZW::isValueSet(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  bool rv = false;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->IsValueSet(vid);

  unlockNodes();
  return rv;
}

bool OZW::isValuePolled(int nodeId, int index)
{
  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  bool rv = false;
  if (getValueID(nodeId, index, &vid))
    rv = Manager::Get()->IsValuePolled(vid);

  unlockNodes();
  return rv;
}

bool OZW::getValueAsBool(int nodeId, int index)
{
  if (isValueWriteOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is WriteOnly" << endl;
      return false;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  bool rv = false;
  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->GetValueAsBool(vid, &rv))
        {
          cerr << __FUNCTION__ << ": Value is not a bool type, returning " 
               << rv << endl;
        }
    }

  unlockNodes();
  return rv;
}

uint8_t OZW::getValueAsByte(int nodeId, int index)
{
  if (isValueWriteOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is WriteOnly" << endl;
      return 0;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  uint8_t rv = false;
  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->GetValueAsByte(vid, &rv))
        {
          cerr << __FUNCTION__ << ": Value is not a byte type, returning " 
               << int(rv) << endl;
        }
    }

  unlockNodes();
  return rv;
}

float OZW::getValueAsFloat(int nodeId, int index)
{
  if (isValueWriteOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is WriteOnly" << endl;
      return 0.0;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  float rv = 0.0;
  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->GetValueAsFloat(vid, &rv))
        {
          cerr << __FUNCTION__ << ": Value is not a float type, returning " 
               << rv << endl;
        }
    }

  unlockNodes();
  return rv;
}

int OZW::getValueAsInt32(int nodeId, int index)
{
  if (isValueWriteOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is WriteOnly" << endl;
      return 0;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  int32_t rv = 0;
  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->GetValueAsInt(vid, &rv))
        {
          cerr << __FUNCTION__ << ": Value is not an int32 type, returning " 
               << rv << endl;
        }
    }

  unlockNodes();
  return int(rv);
}

int OZW::getValueAsInt16(int nodeId, int index)
{
  if (isValueWriteOnly(nodeId, index))
    {
      cerr << __FUNCTION__ << ": Node " << nodeId << " index " << index
           << " is WriteOnly" << endl;
      return 0;
    }

  ValueID vid(m_homeId, (uint64)0);

  lockNodes();

  int16_t rv = 0;
  if (getValueID(nodeId, index, &vid))
    {
      if (!Manager::Get()->GetValueAsShort(vid, &rv))
        {
          cerr << __FUNCTION__ << ": Value is not an int16 type, returning " 
               << rv << endl;
        }
    }

  unlockNodes();
  return int(rv);
}

bool OZW::isNodeListeningDevice(int nodeId)
{
  nodeId &= 0xff;

  lockNodes();

  bool rv = Manager::Get()->IsNodeListeningDevice(m_homeId, nodeId);

  unlockNodes();
  return rv;
}

bool OZW::isNodeFrequentListeningDevice(int nodeId)
{
  nodeId &= 0xff;

  lockNodes();

  bool rv = Manager::Get()->IsNodeFrequentListeningDevice(m_homeId, nodeId);

  unlockNodes();
  return rv;
}

bool OZW::isNodeAwake(int nodeId)
{
  nodeId &= 0xff;

  lockNodes();

  bool rv = Manager::Get()->IsNodeAwake(m_homeId, nodeId);

  unlockNodes();
  return rv;
}

bool OZW::isNodeInfoReceived(int nodeId)
{
  nodeId &= 0xff;

  lockNodes();

  bool rv = Manager::Get()->IsNodeInfoReceived(m_homeId, nodeId);

  unlockNodes();
  return rv;
}

void OZW::setDebug(bool enable)
{ 
  m_debugging = enable; 

  // To bad the following does not seem to affect anything.  The only
  // way I've found to control it is via the options.xml file.

  //  Log::SetLoggingState(enable);
}
