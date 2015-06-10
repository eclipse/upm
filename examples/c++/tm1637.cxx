/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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

#include "tm1637.h"
#include <signal.h>
#include <unistd.h>
#include <sstream>
#include <time.h>

using namespace std;
using namespace upm;

bool run = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    run = false;
}

int
main(int argc, char** argv)
{
	//! [Interesting]
	bool point = true;
	int timezone = -7; // Your UTC offset
	time_t rawtime;
	struct tm * gmt;
	char myTime[5];

    fprintf(stdout, "TM1637 Display Example\n");
    signal(SIGINT, sig_handler);

    TM1637 myDisplay = TM1637(0, 1); // TM1637 on pins 0 (clk) and 1 (dio)
    myDisplay.write(0x39, 0x09, 0x09); // Start a box using 7-segment encoding
    myDisplay.writeAt(3, ']'); // Finish box using writeAt function
    sleep(3); // Wait 3 seconds

    while(run)
    {
    	time(&rawtime); // Update raw time
        gmt = gmtime(&rawtime); // Get current time

        int hour = (gmt) ? gmt->tm_hour : 0;
        int min = (gmt) ? gmt->tm_min : 0;
        // Format and store the time in 24 hour format
        snprintf(myTime, 5, "%2d%02d", (hour + timezone + 24) % 24, min);

        myDisplay.write(myTime); // Write to display as string
    	myDisplay.setColon(point ^= true); // Toggle the dots on the display
    	sleep(1); // Only update once every second
    }

	//! [Interesting]
	return 0;
}


