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

#include <stdint.h>
#include <string>
#include "zwNode.hpp"

#include "Node.h"

using namespace upm;
using namespace std;
using namespace OpenZWave;

zwNode::zwNode(uint32_t homeId, uint8_t nodeId)
{
  m_homeId = homeId;
  m_nodeId = nodeId;

  m_vindex = 0;
}

zwNode::~zwNode()
{
}

uint8_t zwNode::nodeId()
{
  return m_nodeId;
}

uint32_t zwNode::homeId()
{
  return m_homeId;
}

void zwNode::addValueID(ValueID vid) 
{
  // We need to use insert since ValueID's default ctor is private
  m_values.insert(std::pair<int, ValueID>(m_vindex++, vid));
}

void zwNode::removeValueID(ValueID vid) 
{
  //we have to get a little complicated here since we need to delete
  //the value id, but the map is indexed by m_vindex
  for (valueMap_t::iterator it = m_values.begin();
       it != m_values.end(); ++it)
    {
      if ((*it).second == vid)
        {
          m_values.erase((*it).first);
          break;
        }
    }
}

bool zwNode::indexToValueID(int index, ValueID *vid)
{
  valueMap_t::iterator it;

  it = m_values.find(index);

  if (it == m_values.end())
    {
      // not found, return false
      return false;
    }
  else
    *vid = (*it).second;

  return true;
}

void zwNode::dumpNode(bool all)
{
  for (valueMap_t::iterator it = m_values.begin();
       it != m_values.end(); ++it)
    {
      int vindex = (*it).first;
      ValueID vid = (*it).second;
      string label = Manager::Get()->GetValueLabel(vid);
      string valueAsStr;
      Manager::Get()->GetValueAsString(vid, &valueAsStr);
      string valueUnits = Manager::Get()->GetValueUnits(vid);
      ValueID::ValueType vType = vid.GetType();
      string vTypeStr;

      switch (vType)
        {
        case ValueID::ValueType_Bool:
          vTypeStr = "bool";
          break;

        case ValueID::ValueType_Byte:
          vTypeStr = "byte";
          break;

        case ValueID::ValueType_Decimal:
          vTypeStr = "float";
          break;

        case ValueID::ValueType_Int:
          vTypeStr = "int32";
          break;

        case ValueID::ValueType_List:
          vTypeStr = "list";
          break;

        case ValueID::ValueType_Schedule:
          vTypeStr = "schedule";
          break;

        case ValueID::ValueType_Short:
          vTypeStr = "int16";
          break;

        case ValueID::ValueType_String:
          vTypeStr = "string";
          break;

        case ValueID::ValueType_Button:
          vTypeStr = "button";
          break;

        case ValueID::ValueType_Raw:
          vTypeStr = "raw";
          break;

        default:
          vTypeStr = "undefined";
          break;
        }

      // by default we only want user values, unless 'all' is true
      if (all || (vid.GetGenre() == ValueID::ValueGenre_User))
          fprintf(stderr, "\t Index: %2d, Type: %s, Label: %s, Value: %s %s\n", 
                  vindex,
                  vTypeStr.c_str(),
                  label.c_str(),
                  valueAsStr.c_str(),
                  valueUnits.c_str());
    }
}



