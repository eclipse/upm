# The MIT License (MIT)
#
# Author: Assam Boudjelthia
# Copyright (c) 2018 Rohm Semiconductor.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import time, datetime
import sys
import signal
import atexit
from upm import pyupm_bh1792

def main():
	bh1792_sensor = pyupm_bh1792.BH1792()
	bh1792_sensor.SoftReset()
	bh1792_sensor.EnableSyncMode(256, 32)
	bh1792_sensor.StartMeasurement()

	# Prevent stack printing on CTRL^C
	def SIGINTHandler(signum, frame):
		raise SystemExit

	def exitHandler():
		print("Exiting")
		sys.exit(0)

	atexit.register(exitHandler)
	signal.signal(signal.SIGINT, SIGINTHandler)

	sampleCounter = 10
	waitTime = bh1792_sensor.GetMeasurementTimeMS()
	
	print("Heart beat sensor readings: ")
	while sampleCounter > 0:
		[off, on] = bh1792_sensor.GetGreenData()
		print ("off: %d, on: %d, timestamp: %d msec" % (off, on, time.time() * 1000))
		time.sleep(waitTime / 1000.0)
		sampleCounter -= 1


if __name__ == '__main__':
    main()
