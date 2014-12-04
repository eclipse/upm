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
#include "ds1307.h"

using namespace std;

void printTime(upm::DS1307 *rtc)
{
  cout << "The time is: " << 
    rtc->month << "/" << rtc->dayOfMonth << "/" << rtc->year << " " 
       << rtc->hours << ":" << rtc->minutes << ":" << rtc->seconds;

  if (rtc->amPmMode)
    cout << (rtc->pm) ? " PM " : " AM ";

  cout << endl;

  cout << "Clock is in " << ((rtc->amPmMode) ? "AM/PM mode" : "24hr mode")
       << endl;
}

int
main(int argc, char **argv)
{
//! [Interesting]
  // Instantiate a DS1037 on I2C bus 0
  upm::DS1307 *rtc = new upm::DS1307(0);
  
  // always do this first
  cout << "Loading the current time... " << endl;
  if (!rtc->loadTime())
    {
      cerr << "rtc->loadTime() failed." << endl;
      return 0;
    }
  
  printTime(rtc);

  // set the year as an example
  cout << "setting the year to 50" << endl;
  rtc->year = 50;

  rtc->setTime();

  // reload the time and print it
  rtc->loadTime();
  printTime(rtc);

  //! [Interesting]
  
  delete rtc;
  return 0;
}
