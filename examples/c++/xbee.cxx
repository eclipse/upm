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
#include "xbee.h"

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
  cout << "If no argument is used, then the firmware revision, serial number" 
       << endl;
  cout << "and the current IP address (if set) are queried." << endl;
  cout << endl;
  cout << endl;
}

//! [Interesting]

// simple helper function to send a command and wait for a response
void sendCommand(upm::XBee* sensor, string cmd)
{
  // commands need to be terminated with a carriage return
  cmd += "\r";

  sensor->writeDataStr(cmd);
  
  string resp;
  while (sensor->dataAvailable(2000))
    {
      resp += sensor->readDataStr(1024);
    }

  if (resp.empty())
    {
      cerr << "Timed out waiting for response" << endl;
      return;
    }

  resp = sensor->stringCR2LF(resp);
  cout << "Returned (" << resp.size() << " bytes): " << endl;
  cout << resp << endl;
}

int main(int argc, char **argv)
{

  // Instantiate a XBee Module on UART 0
  upm::XBee* sensor = new upm::XBee(0);

  // Set the baud rate, 9600 baud is the default.
  if (sensor->setBaudRate(9600))
    {
      cerr << "Failed to set tty baud rate" << endl;
      return 1;
    }

  printUsage(argv[0]);

  if (argc > 1)
    {
      // enable command mode
      sensor->commandMode();
      cout << "Sending command line argument (" << argv[1] << ")..." << endl;
      sendCommand(sensor, argv[1]);
    }
  else
    {
      // enable command mode
      sensor->commandMode();
      // query the verbose firmware revision
      cout << "Querying verbose firmware revision (ATVL)..." << endl;
      sendCommand(sensor, "ATVL");
      // query the number
      cout << "Querying Serial Number High (ATSH)..." << endl;
      sendCommand(sensor, "ATSH");
      cout << "Querying Serial Number Low (ATSL)..." << endl;
      sendCommand(sensor, "ATSL");

      cout << "Querying address, if set (ATMY)..." << endl;
      sendCommand(sensor, "ATMY");

      // For the XBee WiFi S6B
      // A comprehensive list of commands and command modes is
      // available from the datasheet at:
      // ftp1.digi.com/support/documentation/90002180_L.pdf

      // For the XBee S1
      // A comprehensive list of commands and command modes is
      // available from the datasheet at:
      // http://www.sparkfun.com/datasheets/Wireless/Zigbee/XBee-Datasheet.pdf

      // For the XBee WiFi module:
      // An example using AT commands to connect to an AP, with a
      // private Key using WPA2:

      // Connect to AP with SSID 'mySSID':
      //  ATIDmySSID

      // Provide the private key 'secret':
      //  ATPKsecret

      // Use WPA2 encryption
      //  ATEE2

    }


  delete sensor;
  return 0;
}

//! [Interesting]
