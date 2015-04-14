# Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
# Copyright (c) 2014 Intel Corporation.
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

import time, signal
import pyupm_tm1637 as tm1637

# Register exit handler for normal Ctrl+C exit
def SIGINTHandler(signum, frame):
    raise SystemExit
signal.signal(signal.SIGINT, SIGINTHandler)

# Create a display object on pins 0 CLK and 1 DIO
display = tm1637.TM1637(0, 1)
dots = True

# Get local time
myTime = time.localtime(time.time())
print time.strftime("System time: %H:%M", myTime)
print ("You can adjust your time zone by setting the TZ environment variable.")

# Draw a box for 3 seconds using 7-segment encoding
display.write(0x39, 0x09, 0x09, 0x0f)
time.sleep(3)

# Loop indefinitely
while True:
    # Update and display time
    timeString = time.strftime("%H%M", time.localtime(time.time()))
    display.write(timeString)
    # Toggle colon
    display.setColon(dots)
    dots = not dots

    # Sleep for 1 s
    time.sleep(1)
