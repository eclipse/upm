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
#include "grovewfs.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main()
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate a Grove Water Flow Sensor on digital pin D2
  upm::GroveWFS* flow = new upm::GroveWFS(2);
  
  // set the flow counter to 0 and start counting
  flow->clearFlowCounter();
  flow->startFlowCounter();

  while (shouldRun)
    {
      // we grab these (,illis and flowCount) just for display
      // purposes in this example
      uint32_t millis = flow->getMillis();
      uint32_t flowCount = flow->flowCounter();

      float fr = flow->flowRate();

      // output milliseconds passed, flow count, and computed flow rate
      cout << "Millis: " << millis << " Flow Count: " << flowCount;
      cout << " Flow Rate: " << fr << " LPM" << endl;

      // best to gather data for at least one second for reasonable
      // results.
      sleep(2);
    }

  flow->stopFlowCounter();
//! [Interesting]

  cout << "Exiting..." << endl;

  delete flow;
  return 0;
}
