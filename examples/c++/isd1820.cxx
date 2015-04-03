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
#include "isd1820.h"

using namespace std;

int main (int argc, char **argv)
{
//! [Interesting]
  // Instantiate a ISD1820 on digital pins 2 (play) and 3 (record)
  // This example was tested on the Grove Recorder.

  upm::ISD1820* recorder = new upm::ISD1820(2, 3);
  
  // if an argument was specified (any argument), go into record mode,
  // else playback a previously recorded sample

  cout << "Supply any argument to the command line to record." << endl;
  cout << "Running this example without arguments will play back any " << endl;
  cout << "previously recorded sound." << endl;
  cout << "There is approximately 10 seconds of recording time." << endl;

  cout << endl;

  bool doRecord = false;

  if (argc > 1)
    doRecord = true;
  
  // depending on what was selected, do it, and sleep for 15 seconds

  if (doRecord)
    recorder->record(true);
  else
    recorder->play(true);
    
  // There are about 10 seconds of recording/playback time, so we will
  // sleep for a little extra time.
  cout << "Sleeping for 15 seconds..." << endl;
  sleep(15);

  // now, turn off whatever we were doing.

  if (doRecord)
    recorder->record(false);
  else
    recorder->play(false);

//! [Interesting]

  cout << "Exiting..." << endl;

  delete recorder;
  return 0;
}
