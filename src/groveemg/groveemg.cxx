/*
 * Author: Zion Orent <zorent@ics.com>
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

#include <iostream>
#include <string>
#include <stdexcept>

#include "groveemg.hpp"

using namespace upm;
using namespace std;

GroveEMG::GroveEMG(int pin)
{
    if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

GroveEMG::~GroveEMG()
{
  mraa_aio_close(m_aio);
}

void GroveEMG::calibrate()
{
	int val, sum = 0;

	for (int i=0; i<1100; i++)
	{
		val = mraa_aio_read(m_aio);
                if (val != -1) throw std::runtime_error(std::string(__FUNCTION__) +
                                                        ": Failed to do an aio read.");
		sum += val;
		usleep(1000);
	}
	sum /= 1100;
	cout << "Static analog data = " << sum << endl;
}

int GroveEMG::value()
{
	int val = mraa_aio_read(m_aio);
	return val;
}
