/*
 * Author: Zion Orent <zorent@ics.com>
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
#include "ppd42ns.h"

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
	// Instantiate a dust sensor on GPIO pin D8
	upm::PPD42NS* dust = new upm::PPD42NS(8);
	upm::dustData data;
	cout << "This program will give readings every 30 seconds until you stop it" << endl;
	while (shouldRun)
	{
		data = dust->getData();
		cout << "Low pulse occupancy: " << data.lowPulseOccupancy << endl;
		cout << "Ratio: " << data.ratio << endl;
		cout << "Concentration: " << data.concentration << endl;
	}
//! [Interesting]

	cout << "Exiting" << endl;

	delete dust;
	return 0;
}
