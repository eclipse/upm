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
#include <stdio.h>
#include "grovegprs.h"

using namespace std;
using namespace upm;

void printUsage(char *progname)
{
  cout << "Usage: " << progname << " [AT command]" << endl;
  cout << endl;

  cout << "If an argument is supplied on the command line, that argument is"
       << endl;
  cout << "sent to the module and the response is printed out." << endl;
  cout << endl;
  cout << "If no argument is used, then the manufacturer and the current" 
       << endl;
  cout << "saved profiles are queried and the results printed out." << endl;
  cout << endl;
  cout << endl;
}

// simple helper function to send a command and wait for a response
void sendCommand(upm::GroveGPRS* sensor, string cmd)
{
  // commands need to be terminated with a carriage return
  cmd += "\r";

  sensor->writeDataStr(cmd);
  
  // wait up to 1 second
  if (sensor->dataAvailable(1000))
    {
      cout << "Returned: " << sensor->readDataStr(1024) << endl;
    }
  else
    {
      cerr << "Timed out waiting for response" << endl;
    }
}


int main(int argc, char **argv)
{
//! [Interesting]

  // Instantiate a GroveGPRS Module on UART 0
  upm::GroveGPRS* sensor = new upm::GroveGPRS(0);

  // Set the baud rate, 19200 baud is the default.
  if (sensor->setBaudRate(19200) != mraa::SUCCESS)
    {
      cerr << "Failed to set tty baud rate" << endl;
      return 1;
    }

  printUsage(argv[0]);

  if (argc > 1)
    {
      cout << "Sending command line argument (" << argv[1] << ")..." << endl;
      sendCommand(sensor, argv[1]);
    }
  else
    {
      // query the module manufacturer
      cout << "Querying module manufacturer (AT+CGMI)..." << endl;
      sendCommand(sensor, "AT+CGMI");

      sleep(1);

      // query the saved profiles
      cout << "Querying the saved profiles (AT&V)..." << endl;
      sendCommand(sensor, "AT&V");

      // A comprehensive list is available from the datasheet at:
      // http://www.seeedstudio.com/wiki/images/7/72/AT_Commands_v1.11.pdf
    }

//! [Interesting]

  delete sensor;
  return 0;
}
