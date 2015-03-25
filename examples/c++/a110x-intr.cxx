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
#include "a110x.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

// Our pulse counter
volatile unsigned int counter = 0;

// Our interrupt handler
void hallISR(void *arg)
{
  counter++;
}

int main ()
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate an A110X sensor on digital pin D2
  upm::A110X* hall = new upm::A110X(2);
  
  // This example uses a user-supplied interrupt handler to count
  // pulses that occur when a magnetic field of the correct polarity
  // is detected.  This could be used to measure the rotations per
  // minute (RPM) of a rotor for example.

  hall->installISR(hallISR, NULL);

  while (shouldRun)
    {
      cout << "Pulses detected: " << counter << endl;

      sleep(1);
    }
//! [Interesting]

  cout << "Exiting..." << endl;

  delete hall;
  return 0;
}
