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
#include "groveledbar.hpp"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main ()
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  // Instantiate a GroveLEDBar, we use D8 for the data, and D9 for the
  // clock.  This was tested with a Grove LED bar.
  upm::GroveLEDBar* bar = new upm::GroveLEDBar(8, 9);

  while (shouldRun)
    {
      // count up from green to red
      for (int i=0; i<=10; i++)
        {
          bar->setBarLevel(i, true);
          usleep(100000);
        }
      sleep(1);

      // count down from red to green
      for (int i=0; i<=10; i++)
        {
          bar->setBarLevel(i, false);
          usleep(100000);
        }
      sleep(1);
    }
//! [Interesting]

  cout << "Exiting..." << endl;
  // turn off the LED's
  bar->setBarLevel(0);

  delete bar;
  return 0;
}
