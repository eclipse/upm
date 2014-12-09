/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <signal.h>
#include <iostream>
#include "mpr121.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

void printButtons(upm::MPR121 *touch)
{
  bool buttonPressed = false;

  cout << "Buttons Pressed: ";
  for (int i=0; i<12; i++)
    {
      if (touch->m_buttonStates & (1 << i))
        {
          cout << i << " ";
          buttonPressed = true;
        }
    }

  if (!buttonPressed)
    cout << "None";

  if (touch->m_overCurrentFault)
    cout << "Over Current Fault detected!" << endl;

  cout << endl;
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate an MPR121 on I2C bus 0

  upm::MPR121 *touch = new upm::MPR121(MPR121_I2C_BUS, MPR121_DEFAULT_I2C_ADDR);

  // init according to AN3944 defaults
  touch->configAN3944();

  while (shouldRun)
    {
      touch->readButtons();
      printButtons(touch);
      sleep(1);
    }

//! [Interesting]

  cout << "Exiting..." << endl;

  delete touch;
  return 0;
}
