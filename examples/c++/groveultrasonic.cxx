/*
 * Author: Jun Kato <i@junkato.jp>
 * Copyright (c) 2015 Jun Kato.
 *
 * Thanks to Seeed Studio for a working arduino sketch
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
#include "groveultrasonic.hpp"
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

upm::GroveUltraSonic *sonar = NULL;
bool running = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
      running = false;
    }
}

int
main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);
//! [Interesting]
  // upm::GroveUltraSonic *sonar = NULL;
  sonar = new upm::GroveUltraSonic(2);
  while(running) {
    int width = sonar->getDistance();
    printf("Echo width = %d\n", width);
    printf("Distance inches = %f.2\n\n", width/148.0);
    sleep(3);
  }
//! [Interesting]
  printf("exiting application\n");
  delete sonar;
  return 0;
}
