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
#pragma once

#include <string>
#include <map>

#include "Manager.h"
#include "Notification.h"
#include "Options.h"
#include "Driver.h"
#include "Node.h"
#include "Group.h"
#include "platform/Log.h"

namespace upm {
  
  /**
   * @brief UPM OpenZWave library
   * @defgroup ozw libupm-ozw
   * @ingroup uart wifi
   */

  /**
   * @library ozw
   * @sensor ozw
   * @comname UPM wrapper for the OpenZWave library
   * @type wifi
   * @man other
   * @con uart
   * @web http://www.openzwave.com/
   *
   * @brief UPM API for the OpenZWave library
   *
   * This module implements a singleton wrapper around the OpenZWave
   * library.  OpenZWave must be compiled and installed on your
   * machine in order to use this library.
   *
   * This module was developed with OpenZWave 1.3/1.4, and an Aeon
   * Z-Stick Gen5 configured as a Primary Controller.  It provides the
   * ability to query and set various values that can be used to
   * control ZWave devices.  It does not concern itself with
   * configuration of devices.  It is assumed that you have already
   * setup your ZWave network using a tool like the OpenZWave control
   * panel, and have already configured your devices as appropriate.
   *
   * To avoid exposing some of the internals of OpenZWave, devices
   * (nodes) and their values, are accessed via a nodeId and a value
   * index number.  The ozwdump example will run dumpNodes() which
   * will list the currently connected devices and the values that are
   * available to them, along with an index number for that value.  It
   * is through these values (nodeId and index) that you can query and
   * set device values at a low level.
   *
   * In addition to querying values from a device (such as state
   * (on/off), or temperature, etc), methods are provided to allow you
   * to control these devices to the extent they allow, for example,
   * using a ZWave connected switch to turn on a lamp.
   *
   * Access to this class by OZW drivers is handled by the
   * ozwInterface class.  It is that class that drivers use for access
   * to ozw, and therefore the Z-Wave network.
   *
   * This class is not intended to be used directly by end users.
   * When writing an OZW driver, the ozwInterface class should be used
   * (inherited) by your driver, and your driver should wrap and
   * expose only those methods needed by the user.  Take a look at
   * some of the drivers (like aeotecss6) to see how this works.
   */

  // forward declaration of private zwNode data
  class zwNode;

  class OZW {
  public:

    typedef std::map<uint8_t, zwNode *> zwNodeMap_t;

    /**
     * Get our singleton instance, initializing it if neccessary.  All
     * requests to this class should be done through this instance
     * accessor.
     *
     * @return static pointer to our class instance
     */
    static OZW* instance();

    /**
     * Start configuration with basic options.  This must be called
     * prior to init(), after the OZW() contructor is called.
     *
     * @param configPath Set the location of the OpenZWave config
     * directory, default is /etc/openzwave
     * @param userConfigDir Set the path to the user configuration
     * directory.  This is the location of the zwcfg*.xml and
     * option.xml files for the user (probably created by the
     * OpenZWave Control Panel example application).  The default is
     * the current directory ("").
     * @param cmdLine Specify command line formatted options to
     * OpenZWave.  The default is "".
     */
    void optionsCreate(std::string configPath="/etc/openzwave", 
                       std::string userConfigDir="",
                       std::string cmdLine="");
    
    /**
     * Add an integer Option.  See the OpenZWave documentation for
     * valid values.
     *
     * @param name The name of the configuration option
     * @param val The value to set it to
     */
    void optionAddInt(std::string name, int val);

    /**
     * Add a boolean Option.  See the OpenZWave documentation for
     * valid values.
     *
     * @param name The name of the configuration option
     * @param val The value to set it to
     */
    void optionAddBool(std::string name, bool val);

    /**
     * Add a string Option.  See the OpenZWave documentation for valid
     * values.
     *
     * @param name The name of the configuration option
     * @param val The value to set it to
     * @append true to append to the option, false to override
     */
    void optionAddString(std::string name, std::string val, bool append);

    /**
     * Lock the Options.  This must be called after all options have
     * been set, and before init() is called.  If init() is called
     * without locking the Options, init() will lock them itself.
     * After the options have been locked, no further options can be
     * specified.
     */
    void optionsLock();

    /**
     * Initialize the ZWave network.  This method will start a probe
     * of all defined devices on the ZWave network and query essential
     * information about them.  This function will not return until
     * either initialization has failed, or has succeeded far enough
     * along for the following methods to work.  Depending on the size
     * an complexity of the ZWave network, this may take anywhere from
     * seconds to several minutes to complete.
     *
     * All Options (via option*()) must have been specified before
     * this function is called.  If the Options have not been locked
     * via optionsLock() prior to calling init(), this method will
     * lock them for you before proceeding.
     *
     * @param devicePath The device path for the ZWave controller,
     * typically something like /dev/ttyACM0, or similiar
     * @param isHID true if this is a HID device, false otherwise (ie:
     * a serial port like /dev/ttyACM0, /dev/ttyUSB0, etc).  Default
     * is false.
     * @return true if init succeeded, false otherwise
     */
    bool init(std::string devicePath, bool isHID=false);

    /**
     * Dump information about all configured nodes (devices) and their
     * available values to stdout.  This is useful to determine what
     * nodes are available, and the index (used for querying and
     * seting values for them).  In addition, it includes information
     * about each value (type, current value, etc).
     *
     * @param all set to true to dump information about all values
     * available for each node.  If false, only information about
     * 'user' values (ignoring 'system' and 'configuration') are
     * output.  The default is false ('user' values only).
     */
    void dumpNodes(bool all=false);

    /**
     * Return a string (which may be empty) indicating the Units of
     * measure for a given value.  For example, querying a temperature
     * value may return "F" to indicate Fahrenheit.
     *
     * @param nodeId The node ID to query
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A string containing the Unit of measure for the value
     */
    std::string getValueUnits(int nodeId, int index);

    /**
     * Set the text for the Units of measure for a value.
     *
     * @param nodeId The node ID to query
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param text The text to set
     */
    void setValueUnits(int nodeId, int index, std::string text);

    /**
     * Return a string (which may be empty) containing the
     * user-freindly Label for a value.
     *
     * @param nodeId The node ID to query
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A string containing the Value's label
     */
    std::string getValueLabel(int nodeId, int index);

    /**
     * Set the text for a Value's label.
     *
     * @param nodeId The node ID to query
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param text The text to set
     */
    void setValueLabel(int nodeId, int index, std::string text);

    /**
     * Return a string (which may be empty) indicating the Help text
     * of a value, if available.
     *
     * @param nodeId The node ID to query
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A string containing the Help text, if available
     */
    std::string getValueHelp(int nodeId, int index);

    /**
     * Set the text for a Value's help text.
     *
     * @param nodeId The node ID to query
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param text The text to set
     */
    void setValueHelp(int nodeId, int index, std::string text);

    /**
     * Set the contents of a Value to a string.  This should always
     * succeed if the supplied content makes sense for a given value,
     * regardless of the value's actual type.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param val the content to assign to the value referenced by
     * nodeId, and index.
     */
    void setValueAsString(int nodeId, int index, std::string val);

    /**
     * Set the contents of a Value, to a bool.  This will fail, and an
     * error message printed if the value type is not a boolean value.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param val the boolean content to assign to the value referenced
     * by nodeId, and index.
     */
    void setValueAsBool(int nodeId, int index, bool val);

    /**
     * Set the contents of a Value, to a byte.  This will fail, and an
     * error message printed if the value type is not a byte value.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param val the byte content to assign to the value referenced
     * by nodeId, and index.
     */
    void setValueAsByte(int nodeId, int index, uint8_t val);

    /**
     * Set the contents of a Value, to a float.  This will fail, and an
     * error message printed if the value type is not a float value.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param val the float content to assign to the value referenced
     * by nodeId, and index.
     */
    void setValueAsFloat(int nodeId, int index, float val);

    /**
     * Set the contents of a Value, to a 32 bit integer (int32).  This
     * will fail, and an error message printed if the value type is
     * not an int32.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param val the int32 content to assign to the value referenced
     * by nodeId, and index.
     */
    void setValueAsInt32(int nodeId, int index, int32_t val);

    /**
     * Set the contents of a Value, to a 16 bit integer (int16).  This
     * will fail, and an error message printed if the value type is
     * not an int16.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param val the int16 content to assign to the value referenced
     * by nodeId, and index.
     */
    void setValueAsInt16(int nodeId, int index, int16_t val);

    /**
     * Set the contents of a Value, to an array of bytes.  This will
     * fail, and an error message printed if the value type is not
     * settable as an array of bytes.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param val the byte array content to assign to the value referenced
     * by nodeId, and index.
     * @param len The length of the byte array
     */
    void setValueAsBytes(int nodeId, int index, uint8_t *val, uint8_t len);

    /**
     * Get the minimum allowed value for a node's Value.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return The minumum allowed value
     */
    int getValueMin(int nodeId, int index);

    /**
     * Get the maximum allowed value for a node's Value.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return The maximum allowed value
     */
    int getValueMax(int nodeId, int index);

    /**
     * Test whether a value is read-only.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return true if the value is read-only, false otherwise
     */
    bool isValueReadOnly(int nodeId, int index);

    /**
     * Test whether a value is write only.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return true if the value is write-only, false otherwise
     */
    bool isValueWriteOnly(int nodeId, int index);

    /**
     * Test whether a value is really set on a node, and not a default
     * value chosen by the OpenZWave library.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return true if the value is really set, false if a default value is
     * being reported
     */
    bool isValueSet(int nodeId, int index);

    /**
     * Test whether a value is being manually polled by the OpenZWave
     * library.  Most modern devices are never polled, rather they are
     * configured to report changing values to the controller on their
     * own at device specific intervals or when appropriate events
     * (depending the device) have occurred.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return true if the value is being maually polled, false otherwise
     * being reported
     */
    bool isValuePolled(int nodeId, int index);

    /**
     * Return the content of a value as a string.  This should always
     * succeed, regardless of the actual value type.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A string representing the current contents of a value.
     */
    std::string getValueAsString(int nodeId, int index);

    /**
     * Return the content of a value as a booleang.  This will fail,
     * and an error message printed if the value type is not boolean.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A boolean representing the current contents of a value.
     */
    bool getValueAsBool(int nodeId, int index);

    /**
     * Return the content of a value as a byte.  This will fail, and
     * an error message printed if the value type is not a byte.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A byte representing the current contents of a value.
     */
    uint8_t getValueAsByte(int nodeId, int index);

    /**
     * Return the content of a value as a float.  This will fail, and
     * an error message printed if the value type is not a floating
     * point value.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A float representing the current contents of a value.
     */
    float getValueAsFloat(int nodeId, int index);

    /**
     * Return the content of a value as an int32.  This will fail, and
     * an error message printed if the value type is not an int32.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return An int32 representing the current contents of a value.
     */
    int getValueAsInt32(int nodeId, int index);

    /**
     * Return the content of a value as an int16.  This will fail, and
     * an error message printed if the value type is not an int16.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return An int16 representing the current contents of a value.
     */
    int getValueAsInt16(int nodeId, int index);

    /**
     * Issue a refresh request for a value on a node.  OpenZWave will
     * query the value and update it's internal state when the device
     * responds.  Note, this happens asynchronously - it may take some
     * time before the current value is reported to OpenZWave by the
     * node.  If the node is asleep, you may not get a current value
     * for some time (or at all, depending on the device).  This
     * method will return immediately after the request has been
     * queued.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     */
    void refreshValue(int nodeId, int index);

    /**
     * Enable or disable some debugging output.  Note, this will not
     * affect OpenZWave's own debugging, which is usually set in the
     * option.xml file.
     *
     * @param enable true to enable debugging, false otherwise
     */
    void setDebug(bool enable);

    /**
     * Determine if a node is a listening device -- in other words, the
     * node never sleeps.
     *
     * @param nodeId The node ID
     * @return true if the node never sleeps, false otherwise
     */
    bool isNodeListeningDevice(int nodeId);

    /**
     * Determine if a node is a frequent listening device -- in other
     * words, if the node is asleep, can it be woken by a beam.
     *
     * @param nodeId The node ID
     * @return true if the node is a frequent listening device, false
     * otherwise
     */
    bool isNodeFrequentListeningDevice(int nodeId);

    /**
     * Determine if a node is awake.
     *
     * @param nodeId The node ID
     * @return true if the node is awake, false otherwise
     */
    bool isNodeAwake(int nodeId);

    /**
     * Determine whether a Node's information has been received.  For
     * sleeping nodes, this may take a while (until the node wakes).
     *
     * @param nodeId The node ID
     * @return true if the node information is known, false otherwise
     */
    bool isNodeInfoReceived(int nodeId);

    /**
     * Determine if the Z-Wave network has been initialized yet.
     *
     * @return true if the network is initialized, false otherwise
     */
    bool isInitialized()
    {
      return m_initialized;
    }

  protected:
    /**
     * OZW constructor
     */
    OZW();

    /**
     * OZW Destructor
     */
    ~OZW();

    /**
     * Based on a nodeId and a value index, lookup the corresponding
     * OpenZWave ValueID.
     *
     * @param nodeId The node ID
     * @param index The value index (see dumpNodes()) of the value to query.
     * @param A pointer to a ValueID that will be returned if successful
     * @return true of the nodeId and index was found, false otherwise
     */
    bool getValueID(int nodeId, int index, OpenZWave::ValueID *vid);

    /**
     * Return the Home ID of the network.
     *
     * @return The Home ID.
     */
    uint32_t getHomeID()
    {
      return m_homeId;
    }

    /**
     * Lock the m_zwNodeMap mutex to protect against changes made to
     * the the the map by the OpenZWave notification handler.  Always
     * lock this mutex when acessing anything in the zwNodeMap map.
     */
    void lockNodes() { pthread_mutex_lock(&m_nodeLock); };

    /**
     * Unlock the m_zwNodeMap mutex after lockNodes() has been called.
     */
    void unlockNodes() { pthread_mutex_unlock(&m_nodeLock); };

  private:
    // prevent copying and assignment
    OZW(OZW const &) = delete;
    OZW& operator=(OZW const&) = delete;

    // our class instance
    static OZW* m_instance;

    uint32_t m_homeId;
    bool m_mgrCreated;
    bool m_driverFailed;
    bool m_debugging;
    bool m_initialized;

    bool m_driverIsHID;
    std::string m_devicePath;

    // our notification handler, called by OpenZWave for events on the
    // network.
    static void notificationHandler(OpenZWave::Notification 
                                    const* notification, 
                                    void *ctx);

    // a map of added nodes
    zwNodeMap_t m_zwNodeMap;

    // for coordinating access to the node list
    pthread_mutex_t m_nodeLock;

    // We use these to determine init failure or success (if OpenZWave
    // has successfully queried essential data about the network).
    pthread_mutex_t m_initLock;
    pthread_cond_t m_initCond;
  };
}


