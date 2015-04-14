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

#include "nunchuck.h"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate a nunchuck controller bus 0
  upm::NUNCHUCK *nunchuck = new upm::NUNCHUCK(0);
  
  // always do this first
  cout << "Initializing... " << endl;
  if (!nunchuck->init())
    {
      cerr << "nunchuck->init() failed." << endl;
      return 0;
    }
  
  while (shouldRun)
    {
      nunchuck->update();

      cout << "stickX: " << nunchuck->stickX 
           << ", stickY: " << nunchuck->stickY << endl;
      cout << "accelX: " << nunchuck->accelX 
           << ", accelY: " << nunchuck->accelY 
           << ", accelZ: " << nunchuck->accelZ << endl;
      
      cout << "button C: " 
           << ((nunchuck->buttonC) ? "pressed" : "not pressed") << endl;
      cout << "button Z: " 
           << ((nunchuck->buttonZ) ? "pressed" : "not pressed") << endl;
      cout << endl;

      usleep(100000);
    }
  //! [Interesting]
  
  delete nunchuck;
  return 0;
}
