# Author: Brendan Le Foll <brendan.le.foll@intel.com>
# Contributions: Sarah Knepper <sarah.knepper@intel.com>
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

import time
import pyupm_grove as grove

# Create the temperature sensor object using AIO pin 0
temp = grove.GroveTemp(0)
print temp.name()

# Read the temperature ten times, printing both the Celsius and
# equivalent Fahrenheit temperature, waiting one second between readings
for i in range(0, 10):
    celsius = temp.value()
    fahrenheit = celsius * 9.0/5.0 + 32.0;
    print "%d degrees Celsius, or %d degrees Fahrenheit" \
        % (celsius, fahrenheit)
    time.sleep(1)

# Delete the temperature sensor object
del temp
