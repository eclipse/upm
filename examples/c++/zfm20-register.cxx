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

  // This example demonstrates registering a fingerprint on the zfm20
  // module.  The procedure is as follows:
  //
  // 1. get an image, store it in characteristics buffer 1
  // 2. get another image, store it in characteristics buffer 2
  // 3. store the image, assuming the two fingerprints match

  // first, we need to register our address and password
  
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
  
  cout << endl;

  uint8_t rv;
  // get the first image

  cout << "Place a finger on the sensor." << endl;
  while (fp->generateImage() != ZFM20::ERR_OK)
    ;

  // in theory, we have an image
  cout << "Image captured, converting..." << endl;

  if ((rv = fp->image2Tz(1)) != ZFM20::ERR_OK)
    {
      cerr << "Image conversion failed with error code " << int(rv) <<endl;
      return 1;
    }

  cout << "Image conversion succeeded, remove finger." << endl;
  sleep(1);

  while (fp->generateImage() != ZFM20::ERR_NO_FINGER)
    ;
  
  cout << endl;
  cout << "Now place the same finger on the sensor." << endl;

  while (fp->generateImage() != ZFM20::ERR_OK)
    ;

  cout << "Image captured, converting..." << endl;

  // save this one in slot 2
  if ((rv = fp->image2Tz(2)) != ZFM20::ERR_OK)
    {
      cerr << "Image conversion failed with error code " << int(rv) <<endl;
      return 1;
    }

  cout << "Image conversion succeeded, remove finger." << endl;
  cout << endl;

  cout << "Storing fingerprint at id 1" << endl;
  
  // create the model
  if ((rv = fp->createModel()) != ZFM20::ERR_OK)
    {
      if (rv == ZFM20::ERR_FP_ENROLLMISMATCH)
        cerr << "Fingerprints did not match." << endl;
      else
        cerr << "createModel failed with error code " << int(rv) <<endl;
 
      return 1;
    }
  
  // now store it, we hard code the id (second arg) to 1 here
  if ((rv = fp->storeModel(1, 1)) != ZFM20::ERR_OK)
    {
      cerr << "storeModel failed with error code " << int(rv) <<endl;
      return 1;
    }

  cout << endl;
  cout << "Fingerprint stored at id 1." << endl;
  
//! [Interesting]

  delete fp;
  return 0;
}
