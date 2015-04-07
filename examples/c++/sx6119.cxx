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
#include <signal.h>
#include "sx6119.h"

using namespace std;

int main (int argc, char **argv)
{
//! [Interesting]
  // Instantiate a SX6119 on digital pins 2 (power) and 3 (seek)
  // This example was tested on the Grove FM Receiver.

  upm::SX6119* radio = new upm::SX6119(2, 3);
  
  // if an argument was specified (any argument), seek to the next
  // station, else just toggle the power.


  cout << "Supply any argument to the command line to seek to the" << endl;
  cout << "next station." << endl;
  cout << "Running the example without an argument will toggle the" <<endl;
  cout << "power on or off." << endl;

  cout << endl;

  bool doSeek = false;

  if (argc > 1)
    doSeek = true;
  
  // depending on what was selected, do it

  if (doSeek)
    radio->seek();
  else
    radio->togglePower();
    
//! [Interesting]

  cout << "Exiting..." << endl;

  delete radio;
  return 0;
}
