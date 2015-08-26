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
#include "maxsonarez.h"

using namespace std;

bool shouldRun = true;

#define MAXSONAREZ_AREF   5.0

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  // Instantiate a MaxSonarEZ on analog pin A1, with an analog
  // reference voltage of MAXSONAREZ_AREF
  upm::MAXSONAREZ *sonar = new upm::MAXSONAREZ(1, MAXSONAREZ_AREF);
  
  // Every second, sample the sonar and output the distance to an
  // object in inches.

  // With the EZ3 version, the minimum and maximum ranges seemed to be
  // between 6 and 33 inches

  while (shouldRun)
    {
      cout << "AREF: " << MAXSONAREZ_AREF 
           << ", distance in inches: " << sonar->inches() << endl;
      
      sleep(1);
    }

//! [Interesting]

  cout << "Exiting" << endl;

  delete sonar;
  return 0;
}
