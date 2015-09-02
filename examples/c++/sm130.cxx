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

#include <unistd.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <stdio.h>
#include "sm130.h"

using namespace std;
using namespace upm;

int main (int argc, char **argv)
{
//! [Interesting]

  // Instantiate a UART based SM130 RFID Module using defaults
  upm::SM130* sensor = new upm::SM130();

  // set the baud rate.  19200 baud is the default.
  if (sensor->setBaudRate(19200))
    {
      cerr << "Failed to set baud rate" << endl;
      return 1;
    }

  cout << "Resetting..." << endl;
  sensor->reset();

  cout << "Firmware revision: " << sensor->getFirmwareVersion() << endl;

  cout << "Waiting up to 5 seconds for a tag..." << endl;

  if (sensor->waitForTag(5000))
    {
      cout << "Found tag, UID: " 
           << sensor->string2HexString(sensor->getUID()) << endl;
      cout << "Tag Type: " << sensor->tag2String(sensor->getTagType()) 
           << endl;
    }
  else
    {
      // error
      cout << "waitForTag failed: " << sensor->getLastErrorString() << endl;
    }

//! [Interesting]

  cout << "Exiting" << endl;
  delete sensor;
  return 0;
}
