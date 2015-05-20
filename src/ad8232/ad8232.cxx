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

#include <iostream>

#include "ad8232.h"

using namespace upm;
using namespace std;

AD8232::AD8232(int loPlus, int loMinus, int output, float aref) :
  m_gpioLOPlus(loPlus), m_gpioLOMinus(loMinus), m_aioOUT(output)
{
  m_gpioLOPlus.dir(mraa::DIR_IN);
  m_gpioLOMinus.dir(mraa::DIR_IN);
  
  m_aref = aref;
  m_ares = (1 << m_aioOUT.getBit());
}

AD8232::~AD8232()
{
}

int AD8232::value()
{
  if (m_gpioLOPlus.read() || m_gpioLOMinus.read())
    return 0;
  else
    return m_aioOUT.read();
}
