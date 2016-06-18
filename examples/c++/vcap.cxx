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

#include <unistd.h>
#include <iostream>

#include "vcap.hpp"

using namespace std;

int main(int argc, char **argv)
{
//! [Interesting]

  string defaultDev = "/dev/video0";

  // if an argument was specified, use it as the device instead
  if (argc > 1)
    defaultDev = string(argv[1]);

  cout << "Using device " << defaultDev << endl;
  cout << "Initializing..." << endl;

  // Instantiate an VCAP instance, using the specified video device
  upm::VCAP *sensor = new upm::VCAP(defaultDev);

  // enable some debug/verbose output
  sensor->setDebug(true);

  // This is just a hint.  The kernel can change this to a lower
  // resolution that the hardware supports.  Use getWidth() and
  // getHeight() methods to see what the kernel actually chose if you
  // care.
  sensor->setResolution(1920, 1080);

  // capture an image
  sensor->captureImage();

  // convert and save it as a jpeg
  sensor->saveImage("video-img1.jpg");

  cout << "Exiting..." << endl;

  delete sensor;

//! [Interesting]

  return 0;
}
