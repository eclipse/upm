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

#include <iostream>

#include "rgbringcoder.h"

using namespace std;
using namespace upm;

RGBRingCoder::RGBRingCoder(int en, int latch, int clear, int clk, int dat, 
                           int sw, int encA, int encB, int red, 
                           int green, int blue) :
  m_gpioEn(en), m_gpioLatch(latch), m_gpioClear(clear), m_gpioClock(clk), 
  m_gpioData(dat), m_gpioSwitch(sw), m_gpioEncA(encA), m_gpioEncB(encB),
  m_pwmRed(red), m_pwmGreen(green), m_pwmBlue(blue)
{
  m_counter = 0;

  // enable, set LOW
  m_gpioEn.dir(mraa::DIR_OUT);
  m_gpioEn.write(0);

  // latch
  m_gpioLatch.dir(mraa::DIR_OUT);
  m_gpioLatch.write(0);
  
  // clear, HIGH
  m_gpioClear.dir(mraa::DIR_OUT);
  m_gpioLatch.write(1);
  
  // clock
  m_gpioClock.dir(mraa::DIR_OUT);
  m_gpioClock.write(0);
  
  // data
  m_gpioData.dir(mraa::DIR_OUT);
  m_gpioData.write(0);

  // switch
  m_gpioSwitch.dir(mraa::DIR_IN);
  m_gpioSwitch.mode(mraa::MODE_HIZ);  // no pullup
  m_gpioSwitch.write(0);
  
  // ecoder A interrupt
  m_gpioEncA.dir(mraa::DIR_IN);
  m_gpioEncA.mode(mraa::MODE_PULLUP);
  // EDGE_BOTH would be nice...
  m_gpioEncA.isr(mraa::EDGE_RISING, &interruptHandler, this);

  // ecoder B interrupt
  m_gpioEncB.dir(mraa::DIR_IN);
  m_gpioEncB.mode(mraa::MODE_PULLUP);
  // EDGE_BOTH would be nice...
  m_gpioEncB.isr(mraa::EDGE_RISING, &interruptHandler, this);

  // RGB LED pwms, set to off

  // Red led
  m_pwmRed.period_ms(1);
  m_pwmRed.write(0.99);
  m_pwmRed.enable(true);

  // Green led
  m_pwmGreen.period_ms(1);
  m_pwmGreen.write(0.99);
  m_pwmGreen.enable(true);

  // Blue led
  m_pwmBlue.period_ms(1);
  m_pwmBlue.write(0.99);
  m_pwmBlue.enable(true);

  // whew.
}

RGBRingCoder::~RGBRingCoder()
{
  m_gpioEncA.isrExit();
  m_gpioEncB.isrExit();

  // turn off the ring
  setRingLEDS(0x0000);

  // Turn of RGB LEDS
  setRGBLED(0.99, 0.99, 0.99);
  usleep(100000);

  // turn off PWM's
  m_pwmRed.enable(false);
  m_pwmGreen.enable(false);
  m_pwmBlue.enable(false);
}

void RGBRingCoder::interruptHandler(void *ctx)
{
   upm::RGBRingCoder *This = (upm::RGBRingCoder *)ctx;

   // From the Sparkfun guys:

   // enc_states[] is a fancy way to keep track of which direction
   // the encoder is turning. 2-bits of oldEncoderState are paired
   // with 2-bits of newEncoderState to create 16 possible values.
   // Each of the 16 values will produce either a CW turn (1),
   // CCW turn (-1) or no movement (0).

   static int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1,
                                 -1, 0, 0, 1, 0, 1, -1, 0};
   static uint8_t oldEncoderState = 0;
   static uint8_t newEncoderState = 0;

   // First, find the newEncoderState. This'll be a 2-bit value
   // the msb is the state of the B pin. The lsb is the state
   // of the A pin on the encoder.
   newEncoderState = (This->m_gpioEncB.read()<<1) | 
     (This->m_gpioEncA.read());

   // Now we pair oldEncoderState with new encoder state
   // First we need to shift oldEncoder state left two bits.
   // This'll put the last state in bits 2 and 3.

   oldEncoderState <<= 2;

   // Mask out everything in oldEncoderState except for the previous state
   oldEncoderState &= 0x0c;

   // Now add the newEncoderState. oldEncoderState will now be of
   // the form: 0b0000(old B)(old A)(new B)(new A)
   oldEncoderState |= newEncoderState;

   // update our counter
   This->m_counter += enc_states[oldEncoderState & 0x0f];
}

void RGBRingCoder::setRingLEDS(uint16_t bits)
{
  // First we need to set latch LOW
  m_gpioLatch.write(0);

  // Now shift out the bits, msb first
  for (int i=0; i<16; i++)
    {
      m_gpioData.write( ((bits & 0x8000) ? 1 : 0) );

      // pulse the clock pin
      m_gpioClock.write(1);
      m_gpioClock.write(0);

      bits <<= 1;
    }

  // latch it
  m_gpioLatch.write(1);
}

bool RGBRingCoder::getButtonState()
{
  return (m_gpioSwitch.read() ? true : false);
}

void RGBRingCoder::setRGBLED(float r, float g, float b)
{
  m_pwmRed.write(r);
  m_pwmGreen.write(g);
  m_pwmBlue.write(b);
}
