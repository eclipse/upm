/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "dfrph.h"
#include <mraa/initio.hpp>
#include <interfaces/iPH.hpp>

namespace upm {
  /**
   * @brief DFRobot pH Sensors
   * @defgroup dfrph libupm-dfrph
   * @ingroup dfrobot liquid analog
   */

  /**
   * @library dfrph
   * @sensor dfrph
   * @comname Analog pH Sensor
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
   * @image html dfrph.jpg
   * @snippet dfrph.cxx Interesting
   */

  class DFRPH : virtual public iPH {
  public:

    /**
     * DFRPH constructor
     *
     * @param pin Analog pin to use
     * @param vref Analog reference voltage; default is 5.0 V
     */
    DFRPH(int pin, float vref = 5.0);

    /**
     * Instantiates DFRPH object based on a given string.
     *
     * @param initStr string containing specific information for DFRPH initialization.
     */
    DFRPH(std::string initStr);
    
    /**
     * DFRPH destructor
     */
    ~DFRPH();

    /**
     * Specifies the offset determined from calibration.  The default
     * is 0.0.
     *
     * @param offset The offset value to use
     */
    void setOffset(float offset);

    /**
     * Specifies the scale determined from calibration.  The default
     * is 1.0.
     *
     * @param scale The scale value to use
     */
    void setScale(float scale);

    float volts();

    /**
     * Take a number of samples and return the detected pH value.  The
     * default number of samples is 15.
     *
     * @param samples The number of samples to average over, default 15
     * @return The pH value detected
     */
    virtual float pH(unsigned int samples = 15);

    /**
     * Returns the detected pH value.
     *
     * @return The pH value detected
     */
    virtual float getPH();

  private:
    /**
     * Don't allow copies of this class
     */
    DFRPH(const DFRPH&) {}
    DFRPH &operator=(const DFRPH &) {return *this;}

    dfrph_context _dev;
    mraa::MraaIo mraaIo;
  };
}
