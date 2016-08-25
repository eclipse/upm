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

from time import sleep
import pyupm_gas as TP401

# Give a qualitative meaning to the value from the sensor
def airQuality(value):
    if(value < 50): return "Fresh Air"
    if(value < 200): return "Normal Indoor Air"
    if(value < 400): return "Low Pollution"
    if(value < 600): return "High Pollution - Action Recommended"
    return "Very High Pollution - Take Action Immediately"

# New Grove Air Quality Sensor on AIO pin 0
airSensor = TP401.TP401(0)

# Wait for sensor to warm up
print "Sensor is warming up for 3 minutes..."
for i in range (1, 4):
    sleep(60)
    print i, "minute(s) passed."
print "Sensor is ready!"

# Loop indefinitely
while True:

    # Read values (consecutive reads might vary slightly)
    value = airSensor.getSample()
    ppm = airSensor.getPPM()

    print "raw: %4d" % value , " ppm: %5.2f   " % ppm , airQuality(value)

    # Sleep for 2.5 s
    sleep(2.5)
