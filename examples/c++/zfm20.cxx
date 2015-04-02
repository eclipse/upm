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
#include "zfm20.h"

using namespace std;
using namespace upm;

int main (int argc, char **argv)
{
//! [Interesting]
  // Instantiate a ZFM20 Fingerprint reader on UART 0

  upm::ZFM20* fp = new upm::ZFM20(0);

  // make sure port is initialized properly.  57600 baud is the default.
  if (!fp->setupTty(B57600))
    {
      cerr << "Failed to setup tty port parameters" << endl;
      return 1;
    }

  // first, set the default password and address
  fp->setPassword(ZFM20_DEFAULT_PASSWORD);
  fp->setAddress(ZFM20_DEFAULT_ADDRESS);
      
  // now verify the password.  If this fails, any other commands
  // will be ignored, so we just bail.
  if (fp->verifyPassword())
    {
      cout << "Password verified." << endl;
    }
  else
    {
      cerr << "Password verification failed." << endl;
      return 1;
    }

  // how many valid stored templates (fingerprints) do we have?
  cout << "Total stored templates: " << fp->getNumTemplates() << endl;
  cout << endl;

  // now spin waiting for a fingerprint to successfully image
  cout << "Waiting for finger print..." << endl;

  while (fp->generateImage() == ZFM20::ERR_NO_FINGER)
    ;

  // in theory, we have an image
  cout << "Image captured, converting..." << endl;

  uint8_t rv;
  if ((rv = fp->image2Tz(1)) != ZFM20::ERR_OK)
    {
      cerr << "Image conversion failed with error code " << int(rv) <<endl;
      return 1;
    }

  cout << "Image conversion succeeded." << endl;
  cout << "Searching database..." << endl;

  uint16_t id = 0;
  uint16_t score = 0;

  // we search for a print matching slot 1, where we shored our last
  // converted fingerprint
  if ((rv = fp->search(1, &id, &score)) != ZFM20::ERR_OK)
    {
      if (rv == ZFM20::ERR_FP_NOTFOUND)
        {
          cout << "Finger Print not found" << endl;
          return 0;
        }
      else
        {
          cerr << "Search failed with error code " << int(rv) <<endl;
          return 1;
        }
    }

  cout << "Fingerprint found!" << endl;
  cout << "ID: " << int(id) << ", Score: " << int(score) << endl;

//! [Interesting]

  delete fp;
  return 0;
}
