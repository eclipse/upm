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
#pragma once

#include <iostream>
#include <string>
#include <mraa/aio.hpp>

namespace upm {
  /**
   * @brief DFRobot pH sensors
   * @defgroup dfrph libupm-dfrph
   * @ingroup dfrobot liquid analog
   */

  /**
   * @library dfrph
   * @sensor dfrph
   * @comname DFRobot pH Sensors
   * @type liquid
   * @man dfrobot 
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=1110
   * @con analog
   *
   * @brief API for the DFRobot pH Sensors
   *
   * This sensor family returns an analog voltage proportional to the
   * acidity or alkalinity of a liquid -- it's pH value.
   *
   * This driver was developed using the DFRobot Analog pH meter and
   * the DFRobot Analog pH Meter Pro.
   *
   *
   * Calibration instructions, taken and slightly reworded from the
   *  DFRobot wiki at:
   *  http://dfrobot.com/wiki/index.php/PH_meter%28SKU:_SEN0161%29
   *
   *  1) Connect equipment: the pH electrode is connected to the BNC
   *  connector on the pH meter board, and then the pH meter board is
   *  connected to the analog port 0 of the controller. When the
   *  controller gets power, you will see the blue LED on board is on.
   *
   *  2) Put the pH electrode into the standard solution whose pH
   *  value is 7.00.  Run the dfrph example and note the pH output
   *  value.  Compare the value with 7.00, and calculate the
   *  difference.  This is the value you should supply to the
   *  setOffset() method.
   *
   *  3) Put the pH electrode into the pH standard solution whose
   *  value is 4.00. Then wait about one minute, and adjust the
   *  potentiometer on the interface board.  Let the value stabilise
   *  at around 4.00. At this time,the acidic calibration has been
   *  completed and you can measure the pH value of an acidic
   *  solution.
   *
   *  4) According to the linear characteristics of pH electrode
   *  itself, after the above calibration,you can directly measure the
   *  pH value of the alkaline solution. If you want to get better
   *  accuracy, you can recalibrate it. Alkaline calibration use the
   *  standard solution whose pH value is 9.18.  Also adjust the
   *  potentiometer and let the value stabilise at around 9.18. After
   *  this calibration, you can measure the pH value of an alkaline
   *  solution.
   *
   * @snippet dfrph.cxx Interesting
   */

  class DFRPH {
  public:

    /**
     * DFRPH constructor
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage; default is 5.0 V
     */
    DFRPH(int pin, float aref=5.0);

    /**
     * DFRPH destructor
     */
    ~DFRPH();

    /**
     * Returns the voltage detected on the analog pin
     *
     * @return The detected voltage
     */
    float volts();

    /**
     * Specifies the offset determined from calibration.  The default
     * is 0.0.
     *
     * @param offset The offset value to use
     */
    void setOffset(float offset);

    /**
     * Take a number of samples and return the detected pH value.  The
     * default number of samples is 15.
     *
     * @param samples The number of samples to average over, default 15
     * @return The pH value detected
     */
    float pH(unsigned int samples=15);

  protected:
    mraa::Aio m_aio;

  private:
    float m_aref;
    // ADC resolution
    int m_aRes;

    // voltage offset
    float m_offset;
  };
}


