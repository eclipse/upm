/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016l Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <map>
#include <list>

#include "Manager.h"

namespace upm {

  /**
   * @library ozw
   *
   * @brief Node management for ozw
   *
   * This class is used by ozw to manage valueids for a node (device).
   * No user-serviceable parts inside.  It is not exposed to the end
   * user.
   */

  class zwNode {
  public:
    // valueid map
    typedef std::map<int, OpenZWave::ValueID> valueMap_t;
    // valueid list, used for sorting
    typedef std::list<OpenZWave::ValueID> valueList_t;

    /**
     * zwNode constructor.
     *
     * @param homeId the homeId os the network controller
     * @param nodeId the node index
     */
    zwNode(uint32_t homeId, uint8_t nodeId);

    ~zwNode();

    /**
     * get the nodeId
     *
     * @return The nodeId for this node
     */
    uint8_t nodeId();

    /**
     * get the homeId
     *
     * @return The homeId for this node
     */
    uint32_t homeId();

    /**
     * Add an OpenZWave ValueID and index to the value map,
     * incrementing m_vindex.
     *
     * @param vid The OpenZWave ValueID
     */
    void addValueID(OpenZWave::ValueID vid);

    /**
     * Remove an OpenZWave ValueID from the value map.
     *
     * @param vid The OpenZWave ValueID
     */
    void removeValueID(OpenZWave::ValueID vid);

    /**
     * Lookup and return a ValueID corresponding to an index.
     *
     * @param index the index to look up
     * @param vid The pointer to the returned ValueID, if it exists
     * @return true if the index was found, false otherwise
     */
    bool indexToValueID(int index, OpenZWave::ValueID *vid);

    /**
     * Dump various information about the ValueIDs stored in this
     * node.
     *
     * @param all true to dump all values, false to limit dumping only
     * 'user' values.
     */
    void dumpNode(bool all=false);

    /**
     * Clear the VID map, sort the list containing registered VID's,
     * and re-create the VID map.  The goal is to ensure that the Map
     * is always sorted in acsending order by VID.
     */
    void updateVIDMap();

    /**
     * When enabled, updateVIDMap() will be called every time a new
     * VID is inserted or removed.  This is disabled by default for
     * performance reasons during init() time.  Once the driver is
     * initialized, then this option is, by default  enabled so that
     * the VID map is always kept in a sorted order.
     *
     * @param enable true to enable, false to disable.
     */
    void setAutoUpdate(bool enable)
    {
      m_autoUpdate = enable;
    }

  protected:

  private:
    bool m_autoUpdate;
    uint32_t m_homeId;
    uint8_t m_nodeId;

    valueMap_t m_values;
    valueList_t m_list;

    // we increment this index for every ValueID we add into the map
    unsigned int m_vindex;
  };

}
