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
#include "m24lr64e.h"

using namespace std;
using namespace upm;

int main(int argc, char **argv)
{
//! [Interesting]
  // Instantiate an M24LR64E on I2C bus 0

  upm::M24LR64E *nfcTag = new upm::M24LR64E(M24LR64E_I2C_BUS);

  // This example accesses the device in the 'user' (default) mode,
  // reads the last byte of data in the EEPROM, inverts it, writes
  // it back, and then re-reads it.

  // Read the last byte of the EEPROM area

  int addr = M24LR64E::EEPROM_I2C_LENGTH - 1;
  printf("Address: %d\n", addr);
  uint8_t byte = nfcTag->readByte(addr);
  printf("Read byte: %02x\n", byte);

  // Now change it to it's opposite and write it
  byte = ~byte;
  nfcTag->writeByte(addr, byte);
  printf("Wrote inverted byte: %02x\n", byte);

  // Now read it back.
  byte = nfcTag->readByte(addr);
  printf("Read byte: %02x\n", byte);

//! [Interesting]

  cout << "Exiting..." << endl;

  delete nfcTag;
  return 0;
}
