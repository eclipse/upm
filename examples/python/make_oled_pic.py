#!/usr/bin/env python

# Author: Brendan Le Foll <brendan.le.foll@intel.com>
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
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

from __future__ import print_function

from PIL import Image
import sys

# Pixels are arranged in one byte for 8 vertical pixels and not addressed individually
# We convert the image to greysacle and end up making it monochrome where we
# consider that every pixel that is '40' is black.

width = 128
height = 64

if len(sys.argv) != 2:
  print('Please specify an image to use as the only argument')
  exit(1)

im = Image.open(sys.argv[1])
im = im.convert('L').resize((width, height))

data = list(im.getdata())

byteblock = [0 for i in range(width)]
widthblock = [list(byteblock) for i in range(int(height/8))]
numblock = 0
pixcount = 0
i = 0

# we split the list by width * 8, to create data chunks of 8rows
datachunks=[data[x:x+(width*8)] for x in range(0, len(data), (width*8))]

# grab every pixel of image (or datachunk)
while i < len(widthblock):
  pixcount = 0
  for y in datachunks[i]:
    xcoor = pixcount % width
    ycoor = int(pixcount/width)
    blknum = xcoor % len(widthblock)
    blkycoor = ycoor

    # 40 is what we consider 'black'
    if y > 40:
      widthblock[i][xcoor] |= (1 << blkycoor)

    pixcount += 1
  i += 1

flatlist = [y for x in widthblock for y in x]

carray = 'static uint8_t image[] = {\n' + ', '.join(str(x) for x in flatlist)
print(carray + '\n};')
