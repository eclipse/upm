/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ozwinterface.hpp"

using namespace upm;
using namespace std;


ozwInterface::ozwInterface(int nodeID) :
  m_instance(0)
{
  // Save our device's ID
  m_nodeID = nodeID;

  // create the OZW instance here if it does not already exist, and
  // store the pointer in our class to save on some typing.
  m_instance = OZW::instance();

  // now see if it has been initialized yet for init()
  m_initialized = m_instance->isInitialized();

  setDebug(false);
}

ozwInterface::~ozwInterface()
{
}

void ozwInterface::init(string devicePath, bool isHID)
{
  // if we are already intitialized, don't try to do it again...

  if (!m_initialized)
    m_instance->init(devicePath, isHID);

  // either it threw, or we are initialized now...
  m_initialized = true;
}

void ozwInterface::setDebug(bool enable)
{
  m_debugging = enable;

  // we also enable/disable debugging in OZW
  m_instance->setDebug(enable);
}
