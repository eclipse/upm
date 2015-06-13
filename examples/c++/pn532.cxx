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
#include <signal.h>
#include <iostream>
#include "pn532.h"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate an PN532 on I2C bus 0 (default) using gpio 3 for the
  // IRQ, and gpio 2 for the reset pin.

  upm::PN532 *nfc = new upm::PN532(3, 2);

  if (!nfc->init())
    cerr << "init() failed" << endl;

  uint32_t vers = nfc->getFirmwareVersion();

  if (vers)
    printf("Got firmware version: 0x%08x\n", vers);
  else
    {
      printf("Could not identify PN532\n");
      return 1;
    }

  // Now scan and identify any cards that come in range (1 for now)

  // Retry forever
  nfc->setPassiveActivationRetries(0xff);

  nfc->SAMConfig();

  uint8_t uidSize;
  uint8_t uid[7];

  while (shouldRun)
    {
      memset(uid, 0, 7);
      if (nfc->readPassiveTargetID(nfc->BAUD_MIFARE_ISO14443A,
                                   uid, &uidSize, 2000))
        {
          // found a card
          printf("Found a card: UID len %d\n", uidSize);
          printf("UID: ");
          for (int i = 0; i < uidSize; i++)
            printf("%02x ", uid[i]);
          printf("\n");
          printf("SAK: 0x%02x\n", nfc->getSAK());
          printf("ATQA: 0x%04x\n\n", nfc->getATQA());
          sleep(1);
        }
      else
        {
          printf("Waiting for a card...\n");
        }
    }


//! [Interesting]

  delete nfc;
  return 0;
}
