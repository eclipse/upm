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

#include "ht9170.h"

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

  // Instantiate a DTMF decoder
  upm::HT9170* dtmf = new upm::HT9170(12, 11, 10, 9, 8);
  
  // Now we just spin in a loop, sleeping every 100ms, checking to see
  // if a digit is available.  If so, we decode and print the digit,
  // and continue looping.

  while(shouldRun)
    {
      if (dtmf->digitReady())
        {
          cout << "Got DTMF code: " << dtmf->decodeDigit() << endl;
          // now spin until digitReady() goes false again
          while (dtmf->digitReady())
            ;
        }

      usleep(100000);
    }
 
 //! [Interesting]

  cout << "Exiting..." << endl;

  delete dtmf;
  return 0;
}
