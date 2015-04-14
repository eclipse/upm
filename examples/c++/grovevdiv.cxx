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
#include "grovevdiv.h"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main ()
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate a Grove Voltage Divider sensor on analog pin A0
  upm::GroveVDiv* vDiv = new upm::GroveVDiv(0);

  // collect data and output measured voltage according to the setting
  // of the scaling switch (3 or 10)
  while (shouldRun)
    {
      unsigned int val = vDiv->value(100);
      float gain3val = vDiv->computedValue(3, val);
      float gain10val = vDiv->computedValue(10, val);
      cout << "ADC value: " << val << " Gain 3: " << gain3val 
           << "v Gain 10: " << gain10val << "v" << endl;

      sleep(1);
    }
//! [Interesting]

  cout << "Exiting..." << endl;

  delete vDiv;
  return 0;
}
