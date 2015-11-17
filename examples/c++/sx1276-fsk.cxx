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
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include "sx1276.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);
//! [Interesting]

  cout << "Specify an argument to go into receive mode.  Default is transmit"
       << endl;

  bool rx = false;
  if (argc > 1)
    rx = true;

  // Instantiate an SX1276 using default parameters
  upm::SX1276 *sensor = new upm::SX1276();

  // 915Mhz
  sensor->setChannel(915000000);

  // FSK configuration (rx and tx must be configured the same):
  // Tx output power = 14 dBm
  // FSK freq deviation = 25000 Hz
  // FSK bandwidth = 50000 bps
  // FSK AFC bandwidth = 83333 Hz
  // FSK datarate = 50000 bps
  // FSK preamble len = 5
  // FSK fixed length payload = false
  // FSK CRC check = true
  // FSK (rx) continuous Rx mode = false

  sensor->setTxConfig(sensor->MODEM_FSK, 14, 25000, 0,
                       50000, 0, 5, false, true, false, 0, false);
    
  sensor->setRxConfig(sensor->MODEM_FSK, 50000, 50000,
                      0, 83333, 5, 0, false, 0, true,
                      false, 0, false, true);

  int count = 0;
  int buflen = 64;
  char buffer[buflen];

  while (shouldRun)
    {
      if (!rx)
        {
          snprintf(buffer, buflen, "Ping %d", count++);
          cout << "Sending..." << std::string(buffer) << endl;
          sensor->sendStr(string(buffer), 3000);
          
          sensor->setSleep();
          sleep(1);
        }
      else
        {
          // receiving
          cout << "Attempting to receive..." << endl;
          int rv;
          if (rv = sensor->setRx(3000))
            {
              cout << "setRx returned " << rv << endl;
            }
          else
            {
              cout << "Received Buffer: " << sensor->getRxBufferStr() << endl;
            }

          // go back to sleep when done
          sensor->setSleep();
          usleep(250000);
        }
      

    }
//! [Interesting]

  cout << "Exiting..." << endl;
  
  delete sensor;
  
  return 0;
}
