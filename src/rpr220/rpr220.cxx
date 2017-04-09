/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
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

#include "rpr220.hpp"

using namespace upm;
using namespace std;

RPR220::RPR220(int pin) :
    m_rpr220(rpr220_init(pin))
{
    if (!m_rpr220)
        throw std::runtime_error(std::string(__FUNCTION__) +
                                 ": rpr220_init() failed");
}

RPR220::~RPR220()
{
    rpr220_close(m_rpr220);
}

bool RPR220::blackDetected()
{
    return rpr220_black_detected(m_rpr220);
}

#ifdef JAVACALLBACK
void RPR220::installISR(jobject runnable)
{
    installISR(mraa_java_isr_callback, runnable);
}
#endif

void RPR220::installISR(void (*isr)(void *), void *arg)
{
    rpr220_install_isr(m_rpr220, isr, arg);
}

void RPR220::uninstallISR()
{
    rpr220_uninstall_isr(m_rpr220);
}

