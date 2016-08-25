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
#pragma once

#include <string>

#include "ozw.hpp"

namespace upm {

  /**
   * @library ozw
   * @comname UPM Interface API for OpenZWave (OZW)
   * @con uart
   *
   * @brief UPM Interface API for OpenZWave (OZW)
   *
   * This class implements some common access functions that are
   * useful to any driver making use of the ozw driver.
   *
   * It is intended to be inherited by device-specific ozw drivers,
   * and provides some basic public methods all ozw drivers need to
   * have, as well as protected access to other OZW facilities that
   * are needed by a driver, but should not be exposed to the
   * end-user.
   */

  class ozwInterface {
  public:

    /**
     * ozwInterface constructor
     *
     * @param nodeID The ZWave node number of the device we are
     * interested in.
     */
    ozwInterface(int nodeID);

    /**
     * ozwInterface Destructor
     */
    virtual ~ozwInterface();

    /**
     * Enable some debugging output in this module as well as the
     * OZW module.  Debugging is disabled by default.
     *
     * @param enable true to enable, false to disable.
     */
    virtual void setDebug(bool enable);

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
    virtual void optionsCreate(std::string configPath="/etc/openzwave",
                               std::string userConfigDir="",
                               std::string cmdLine="")
    {
      m_instance->optionsCreate(configPath, userConfigDir, cmdLine);
    }

    /**
     * Add an integer Option.  See the OpenZWave documentation for
     * valid values.
     *
     * @param name The name of the configuration option
     * @param val The value to set it to
     */
    virtual void optionAddInt(std::string name, int val)
    {
      m_instance->optionAddInt(name, val);
    }

    /**
     * Add a boolean Option.  See the OpenZWave documentation for
     * valid values.
     *
     * @param name The name of the configuration option
     * @param val The value to set it to
     */
    virtual void optionAddBool(std::string name, bool val)
    {
      m_instance->optionAddBool(name, val);
    }

    /**
     * Add a string Option.  See the OpenZWave documentation for valid
     * values.
     *
     * @param name The name of the configuration option
     * @param val The value to set it to
     * @append true to append to the option, false to override
     */
    virtual void optionAddString(std::string name, std::string val,
                                 bool append)
    {
      m_instance->optionAddString(name, val, append);
    }

    /**
     * Lock the Options.  This must be called after all options have
     * been set, and before init() is called.  If init() is called
     * without locking the Options, init() will lock them itself.
     * After the options have been locked, no further options can be
     * specified.
     */
    virtual void optionsLock()
    {
      m_instance->optionsLock();
    }

    /**
     * This function initializes the underlying OZW Master singleton
     * and the openzwave library in the event it has not already been
     * initialized.  If the OZW Master singleton has already been
     * initialized, then this call will be ignored.
     *
     * @param devicePath The path to the device, typically something
     * like /dev/ttyACM0.
     * @param isHID true if this is an HID device, false if it's a
     * serial device.  The default is false.
     */
    virtual void init(std::string devicePath, bool isHID=false);

    /**
     * Dump information about all configured nodes and their valueids
     * to stderr.  This is useful to determine what nodes are
     * available, and the index (used for querying and seting values
     * for them).  In addition, it includes information about each
     * value (type, current value, etc).
     *
     * @param all set to true to dump information about all values
     * available for each node.  If false, only information about
     * 'user' values (ignoring 'system' and 'configuration') are
     * output.  The default is false ('user' values only).
     */
    virtual void dumpNodes(bool all=false)
    {
      m_instance->dumpNodes(all);
    }

    /**
     * Return a string (which may be empty) indicating the Units of
     * measure for a given value.  For example, querying a temperature
     * value may return "F" to indicate Fahrenheit.
     *
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A string containing the Unit of measure for the value
     */
    virtual std::string getValueUnits(int index)
    {
      return m_instance->getValueUnits(m_nodeID, index);
    }

    /**
     * Return a string (which may be empty) indicating the Label set
     * for a given value.  For example, querying a temperature value
     * may return "Temperature" to indicate what it is.
     *
     * @param index The value index (see dumpNodes()) of the value to query.
     * @return A string containing the Label for the value
     */
    virtual std::string getValueLabel(int index)
    {
      return m_instance->getValueLabel(m_nodeID, index);
    }

  protected:
    // also enable debugging in ozw
    bool m_debugging;

    // our target Device Node ID
    uint32_t m_nodeID;

    // a copy of the OZW singleton instance pointer.  This is used by
    // drivers to access underlying OZW facilities.
    OZW* m_instance;

    // are we initialized?
    bool m_initialized;

  private:
  };
}
