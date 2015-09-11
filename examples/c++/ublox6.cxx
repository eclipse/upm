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
#include "ublox6.h"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

const size_t bufferLength = 256;

int main (int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate a Ublox6 GPS device on uart 0.
  upm::Ublox6* nmea = new upm::Ublox6(0);

  // make sure port is initialized properly.  9600 baud is the default.
  if (!nmea->setupTty(B9600))
    {
      cerr << "Failed to setup tty port parameters" << endl;
      return 1;
    }

  // Collect and output NMEA data.  There are various libraries out on
  // the Internet, such as tinyGPS or tinyGPS++ that can handle
  // decoding NMEA data and presenting it in a more easily accessible
  // format.  This example will just check for, and read raw NMEA data
  // from the device and output it on stdout.

  // This device also supports numerous configuration options, which
  // you can set with writeData().  Please refer to the Ublox-6 data
  // sheet for further information on the formats of the data sent and
  // received, and the various operating modes available.

  char nmeaBuffer[bufferLength];
  while (shouldRun)
    {
      // we don't want the read to block in this example, so always
      // check to see if data is available first.
      if (nmea->dataAvailable())
        {
          int rv = nmea->readData(nmeaBuffer, bufferLength);

          if (rv > 0)
            write(1, nmeaBuffer, rv);

          if (rv < 0) // some sort of read error occured
            {
              cerr << "Port read error." << endl;
              break;
            }

          continue;
        }

      usleep(100000); // 100ms
    }
//! [Interesting]

  cout << "Exiting..." << endl;

  delete nmea;
  return 0;
}
