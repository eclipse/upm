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
#pragma once

#include <map>

#include "Manager.h"

namespace upm {

  class zwNode {
  public:
    typedef std::map<int, OpenZWave::ValueID> valueMap_t;

    /**
     * zwNode contructor.
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

  protected:

  private:
    uint32_t m_homeId;
    uint8_t m_nodeId;

    valueMap_t m_values;

    // we increment this index for every ValueID we add
    unsigned int m_vindex;
  };

}
