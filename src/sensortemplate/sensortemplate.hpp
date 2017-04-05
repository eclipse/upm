/*
 * The MIT License (MIT)
 *
 * Author: Your Full Name <your@email.address>
 * Copyright (c) <year> <copyright holder>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#pragma once

#include <string>

#include "mraa/i2c.hpp"

namespace upm
{
/**
 * @brief Library block provides a short library description
 * @defgroup sensortemplate libupm-sensortemplate
 * @ingroup <manufacturer: adafruit,aeon,amphenol,avago,bosch,catnip,comet,dfrobot,emax,epict,freescale,generic,honeywell,kionix,maxbotix,maxbotix,maxim,meas,mouser,numatolabs,omega,openelectrons,sainsmart,seeed,semtech,silabs,sparkfun,stmicro,ti,trane,veris> <connection: analog,gpio,i2c,pwm,spi,uart> <category: accelerometer,ainput,button,color,compass,digipot,display,electric,flexfor,gaseous,gps,led,light,liquid,medical,motor,other,pressure,relay,rfid,serial,servos,sound,temp,time,touch,video,wifi> (<kit>: eak,gsk,hak,robok,tsk)
 */

/**
 * @library sensortemplate
 * @sensor Usually the chip number used by the sensor. When this is not
 *         available or relevant, use a unique descriptor that makes sense.
 *         Must match class name. *Mandatory*
 * @comname Short, Title-Case Description from Datasheet. *Manditory*
 * @altname Alternative names that your sensor driver might have. This may
 *          include manufacturer's name. *Optional*
 * @altid Alternative chip-ids that your sensor driver supports. *Optional*
 * @type One or more of: accelerometer,ainput,button,color,compass,digipot,
 *       display,electric,flexfor,gaseous,gps,led,light,liquid,medical,motor,other,pressure,relay,rfid,serial,servos,sound,temp,time,touch,video,wifi *Manditory*
 * @man One or more of: Sensor manufacturer. Can be 'generic'. *Mandatory*
 * @web Links to vendors or data-sheets. *Optional*
 * @con One or more of: analog,gpio,i2c,pwm,spi,uart *Manditory*
 * @kit One of: gsk, hak, eak, tsk, robok *Optional*
 *
 * @brief Short class/sensor description
 *
 * Then add a much more detailed description here.  Include items such as
 * board-specifics, testing, etc...
 *
 * @image html sensortemplate.png
 * @snippet sensortemplate.cxx Interesting
 */

class SensorTemplate
{
    public:
        /**
         * @brief Default constructor for SensorTemplate
         *
         * This creates an instance of SensorTemplate given an I2C bus
         * number.
         *
         * @param i2c_bus Target I2C bus
         * @return sensor context pointer
         * @throws std::runtime_error on failure to initialize I2C
         */
        SensorTemplate(int i2c_bus);

        /**
         * SensorTemplate destructor
         */
        virtual ~SensorTemplate() {};

        /**
         * @brief Hello world method
         *
         * Provides an example method which returns 'Hello world'
         */
        std::string helloWorld();
    private:
        /* Private I2c instance */
        mraa::I2c _i2c;
};
}
