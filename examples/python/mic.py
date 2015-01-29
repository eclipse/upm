# Author: John Van Drasek <john.r.van.drasek@intel.com>
# Copyright (c) 2015 Intel Corporation.
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

import time
import pyupm_mic as upmMicrophone 

# Attach microphone to analog port A0
myMic = upmMicrophone.Microphone(0)
threshContext = upmMicrophone.thresholdContext()
threshContext.averageReading = 0
threshContext.runningAverage = 0
threshContext.averagedOver = 2

# Infinite loop, ends when script is cancelled
# Repeatedly, take a sample every 2 microseconds;
# find the average of 128 samples; and
# print a running graph of dots as averages
while(1):
    buffer = upmMicrophone.uint16Array(128)
    len = myMic.getSampledWindow(2, 128, buffer);
    if len:
        thresh = myMic.findThreshold(threshContext, 30, buffer, len)
        myMic.printGraph(threshContext)
        if(thresh):
            print "Threshold is ", thresh

# Delete the upmMicrophone object
del myMic
