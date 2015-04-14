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
#include <iostream>
#include <signal.h>
#include "mq303a.h"

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
  // Instantiate an mq303a sensor on analog pin A0

  // This device uses a heater powered from an analog I/O pin. 
  // If using A0 as the data pin, then you need to use A1, as the heater
  // pin (if using a grove mq303a).  For A1, we can use the D15 gpio, 
  // setup as an output, and drive it low to power the heater.

  upm::MQ303A *mq303a = new upm::MQ303A(0, 15);
  
  cout << "Enabling heater and waiting 2 minutes for warmup." << endl;
  mq303a->heaterEnable(true);
  sleep(120);

  cout << "This sensor may need to warm until the value drops below about 450." 
       << endl;

  // Print the detected alcohol value every second
  while (shouldRun)
    {
      int val = mq303a->value();

      cout << "Alcohol detected (higher means stronger alcohol): " 
           << val << endl;

      sleep(1);
    }
//! [Interesting]

  cout << "Exiting" << endl;
  mq303a->heaterEnable(false);

  delete mq303a;
  return 0;
}
