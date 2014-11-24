# Author: Sarah Knepper <sarah.knepper@intel.com>
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
import array
import pyupm_ldt0028 as ldt0028

NUMBER_OF_SECONDS = 10
SAMPLES_PER_SECOND = 50
THRESHOLD = 100

# Create the LDT0-028 Piezo Vibration Sensor object using AIO pin 0
sensor = ldt0028.LDT0028(0)

# Read the signal every 20 milliseconds for 10 seconds
print 'For the next', NUMBER_OF_SECONDS, 'seconds,', \
      SAMPLES_PER_SECOND, 'samples will be taken every second.\n'
buffer = array.array('H')
for i in range(0, NUMBER_OF_SECONDS * SAMPLES_PER_SECOND):
    buffer.append(sensor.getSample())
    time.sleep(1.0/SAMPLES_PER_SECOND)

# Print the number of times the reading was greater than the threshold
count = 0
for i in range(0, NUMBER_OF_SECONDS * SAMPLES_PER_SECOND):
    if buffer[i] > THRESHOLD:
        count += 1
print sensor.name(), ' exceeded the threshold value of', \
        THRESHOLD, 'a total of', count, 'times,'
print 'out of a total of', NUMBER_OF_SECONDS*SAMPLES_PER_SECOND, \
        'reading.\n'

# Print a graphical representation of the average value sampled
# each second for the past 10 seconds, using a scale factor of 15
print 'Now printing a graphical representation of the average reading '
print 'each second for the last', NUMBER_OF_SECONDS, 'seconds.'
SCALE_FACTOR = 15
for i in range(0, NUMBER_OF_SECONDS):
    sum = 0
    for j in range(0, SAMPLES_PER_SECOND):
        sum += buffer[i*SAMPLES_PER_SECOND+j]
    average = sum / SAMPLES_PER_SECOND
    stars_to_print = int(round(average / SCALE_FACTOR))
    print '(' + repr(int(round(average))).rjust(4) + ') |', '*' * stars_to_print

# Delete the sensor object
del sensor
