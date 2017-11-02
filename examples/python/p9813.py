#!/usr/bin/env python
# Author: Sergey Kiselev <sergey.kiselev@intel.com>
# Copyright (c) 2016 Sergey Kiselev
# Based on the apa102 driver writen by Yannick Adam <yannick.adam@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

from __future__ import print_function
import time, sys, signal, atexit
from upm import pyupm_p9813 as mylib

def main():
    # Instantiate a chain of 2 LEDs using pin 2 for clock and pin 3 for data
    ledChain = mylib.P9813(2, 2, 3)

    ## Exit handlers ##
    # This stops python from printing a stacktrace when you hit control-C
    def SIGINTHandler(signum, frame):
        raise SystemExit

    # Register exit handlers
    signal.signal(signal.SIGINT, SIGINTHandler)

    print("Setting all LEDs to Green")
    ledChain.setAllLeds(0, 255, 0)

    print("Setting LED 1 to Blue")
    ledChain.setLed(1, 0, 0, 255)

if __name__ == '__main__':
    main()
