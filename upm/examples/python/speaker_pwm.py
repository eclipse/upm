#!/usr/bin/env python
# Author: Jon Trulson <jtrulson@ics.com>
# Copyright (c) 2017 Intel Corporation.
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

import time, sys, signal, atexit
from upm import pyupm_speaker as upmspeaker

def main():
    # Instantiate a Speaker (PWM) pin D3 in PWM mode
    speaker = upmspeaker.Speaker(3, True)

    # emit a series of frequencies for 500ms each
    speaker.emit(50, 500);
    speaker.emit(75, 500);
    speaker.emit(125, 500);
    speaker.emit(250, 500);
    speaker.emit(500, 500);
    speaker.emit(1000, 500);
    speaker.emit(2000, 500);
    speaker.emit(3000, 500);
    speaker.emit(5000, 500);
    speaker.emit(10000, 500);

if __name__ == '__main__':
    main()
