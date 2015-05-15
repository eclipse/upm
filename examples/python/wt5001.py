#!/usr/bin/python
# Author: Zion Orent <zorent@ics.com>
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

import time, signal, sys
import pyupm_wt5001 as upmWt5001

# Instantiate a WT5001 serial MP3 player on uart 0.
# This example was tested on the Grove Serial MP3 module.
myMP3Player = upmWt5001.WT5001(0)


def printUsage(progname):
	print ("Usage: python " + progname + " <command>\n"
	"Commands:\n"
	"0  - stop playing\n"
	"1  - start playing track 1\n"
	"2  - pause/un-pause playback\n"
	"3  - next track\n"
	"4  - previous track")


cmd = -1;
if (len(sys.argv) > 1):
	cmd = int(sys.argv[1])

if (not myMP3Player.setupTty(upmWt5001.cvar.int_B9600)):
	print "Failed to setup tty port parameters"
	sys.exit(0)

if cmd == 0:
	myMP3Player.stop()
elif cmd == 1:
	myMP3Player.play(upmWt5001.WT5001.SD, 1)
elif cmd == 2:
	myMP3Player.pause()
elif cmd == 3:
	myMP3Player.next()
elif cmd == 4:
	myMP3Player.previous()
else:
	# nothing, just output usage, and info below
	printUsage(sys.argv[0])


# print out some information
vol = upmWt5001.uint8Array(0)
myMP3Player.getVolume(vol)
print "The current volume is: " + str(vol.__getitem__(0))

ps = upmWt5001.uint8Array(0)
myMP3Player.getPlayState(ps)
print "The current play state is: " + str(ps.__getitem__(0))

numf = upmWt5001.uint16Array(0)
myMP3Player.getNumFiles(upmWt5001.WT5001.SD, numf)
print "The number of files on the SD card is: " + str(numf.__getitem__(0))

curf = upmWt5001.uint16Array(0)
myMP3Player.getCurrentFile(curf)
print "The current file is: " + str(curf.__getitem__(0))


# set the date
myMP3Player.setDate(2015, 3, 14)

# set the time
myMP3Player.setTime(9, 26, 53)


year = upmWt5001.uint16Array(0)
month = upmWt5001.uint8Array(0)
day = upmWt5001.uint8Array(0)

myMP3Player.getDate(year, month, day)
mp3date = str(month.__getitem__(0)) + "/"
mp3date += (str(day.__getitem__(0)) + "/")
mp3date += str(year.__getitem__(0))
print "The device date is: " + mp3date

hour = upmWt5001.uint8Array(0)
minute = upmWt5001.uint8Array(0)
second = upmWt5001.uint8Array(0)
myMP3Player.getTime(hour, minute, second)
mp3time = str(hour.__getitem__(0)) + ":"
mp3time += (str(minute.__getitem__(0)) + ":")
mp3time += str(second.__getitem__(0))
print "The device time is: " + mp3time
