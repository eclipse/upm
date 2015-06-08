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
#include <stdio.h>
#include "grovescam.h"

using namespace std;
using namespace upm;

int main (int argc, char **argv)
{
//! [Interesting]

  // Instantiate a Grove Serial Camera on UART 0
  upm::GROVESCAM* camera = new upm::GROVESCAM(0);

  // make sure port is initialized properly.  115200 baud is the default.
  if (!camera->setupTty())
    {
      cerr << "Failed to setup tty port parameters" << endl;
      return 1;
    }

  if (camera->init())
    cout << "Initialized..." << endl;
  else
    cout << "init() failed" << endl;

  if (camera->preCapture())
    cout << "preCapture succeeded..." << endl;
  else
    cout << "preCapture failed." << endl;

  if (camera->doCapture())
    cout << "doCapture succeeded..." << endl;
  else
    cout << "doCapture failed." << endl;

  cout << "Image size is " << camera->getImageSize() << " bytes" << endl;

  if (camera->getImageSize() > 0)
    {
      cout << "Storing image.jpg..." << endl;
      if (camera->storeImage("image.jpg"))
        cout << "storeImage succeeded..." << endl;
      else
        cout << "storeImage failed." << endl;
    }
//! [Interesting]

  delete camera;
  return 0;
}
