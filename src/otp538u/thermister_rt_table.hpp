/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// This table was taken from the '538RT_table.pdf' datasheet. It maps
// resistance values to ambient temperatures starting at -20C and
// going up to 200C in increments of 1C

static const int otp538u_rt_table_max = 121;

static int otp538u_rt_table[otp538u_rt_table_max] = {
  919730, 869299, 821942, 777454, 735644, 696336, 659365, 624578, 591834,
  561002, 531958, 504588, 478788, 454457, 431504, 409843, 389394, 370082,
  351839, 334598, 318300, 302903, 288329, 274533, 261471, 249100, 237381,
  226276, 215750, 205768, 196300, 187316, 178788, 170691, 163002, 155700,
  148766, 142183, 135936, 130012, 124400, 119038, 113928, 109059, 104420,
  100000, 95788, 91775, 87950, 84305, 80830, 77517, 74357, 71342, 68466,
  65720, 63098, 60595, 58202, 55916, 53730, 51645, 49652, 47746, 45924,
  44180, 42511, 40912, 39380, 37910, 36500, 35155, 33866, 32631, 31446,
  30311, 29222, 28177, 27175, 26213, 25290, 24403, 23554, 22738, 21955,
  21202, 20479, 19783, 19115, 18472, 17854, 17260, 16688, 16138, 15608,
  15098, 14608, 14135, 13680, 13242, 12819, 12412, 12020, 11642, 11278,
  10926, 10587, 10260, 9945, 9641, 9347, 9063, 8789, 8525, 8270, 8023,
  7785, 7555, 7333, 7118, 6911 
};
 
