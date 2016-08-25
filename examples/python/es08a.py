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
import pyupm_servo as servo 

# Create the servo object using D5
gServo = servo.ES08A(5)

for i in range(0,10): 
    # Set the servo arm to 0 degrees
    gServo.setAngle(0)
    print 'Set angle to 0'
    time.sleep(1)

    # Set the servo arm to 90 degrees
    gServo.setAngle(90)
    print 'Set angle to 90'
    time.sleep(1)

    # Set the servo arm to 180 degrees
    gServo.setAngle(180)
    print 'Set angle to 180'
    time.sleep(1)

# Delete the servo object
del gServo 
