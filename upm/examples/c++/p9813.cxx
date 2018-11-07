/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
 * Based on the p9813 driver writen by Yannick Adam <yannick.adam@gmail.com>
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

#include "p9813.hpp"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a chain of 2 LEDs using pin 2 for clock and pin 3 for data
    upm::P9813 ledChain(2, 2, 3);

    // Set all LEDs to Red
    ledChain.setAllLeds(255, 0, 0);

    // Set a single LED to green
    ledChain.setLed(1, 0, 255, 0);

    //! [Interesting]
    return 0;
}
