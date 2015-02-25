/*
* Author: Zion Orent <zorent@ics.com>
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
#include <iostream>
#include <time.h>
#include <signal.h>
#include "groveelectromagnet.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

float get_time()
{
	return ((float)(clock()))/CLOCKS_PER_SEC;
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // The was tested with the Grove Electromagnetic Module
  // Instantiate a Grove Electromagnet on digital pin D2
  upm::GroveElectromagnet* magnet = new upm::GroveElectromagnet(2);
  cout << "Starting up magnet...." << endl;
  magnet->off();

  bool magnetState = false;
  float time_passed = get_time();

  // Turn magnet on and off every 5 seconds
  while (shouldRun)
  {
	if ((get_time() - time_passed) > 5.0)
	{
		magnetState = !magnetState;
		if (magnetState)
			magnet->on();
		else
			magnet->off();
		cout << "Turning magnet " << ((magnetState) ? "on" : "off") << endl;
		time_passed = get_time();
	}
  }

//! [Interesting]
  magnet->off();
  cout << "Exiting" << endl;

  delete magnet;
  return 0;
}
