/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Contributions: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <string>
#include <mraa/aio.h>
#include <mraa/gpio.h>

namespace upm {

/**
 * @brief library for basic grove sensors
 * @defgroup grove libupm-grove
 */
class Grove {
    public:
        virtual ~Grove() {}
        std::string name()
        {
            return m_name;
        }
    protected:
        std::string m_name;
};

/**
 * @brief C++ API for Grove LED
 *
 * UPM module for Grove LED (or other similar light-emitting diode).
 * An LED is a small lightbulb that will emit light (turn on) in 
 * response to a small curent.  The longer wire of an LED connects
 * to the positive seat (anode); the shorter wire connects to the
 * negative seat (cathode).  The flat side of the bulb corresponds
 * to the cathode while the rounded side corresponds to the anode.
 *
 * @ingroup grove gpio
 * @snippet groveled.cxx Interesting
 */
class GroveLed: public Grove {
    public:
        /**
         * Grove LED constructor
         *
         * @param gpio pin to use
         */
        GroveLed(int pin);
        /**
         * Grove LED destructor
         */
        ~GroveLed();
        /**
         * Turn the LED on or off, depending on the value.
         * If the value is positive (greater than or equal
         * to 1), the LED is turned on.  Otherwise, for 0
         * or negative values, the LED is turned off.
         *
         * @param value tells the LED to turn on (for value >=1)
         * or off (for value <1)
         *
         * @return 0 on success; non-zero otherwise
         */
        mraa_result_t write(int value);
        /**
         * Turn the LED off
         *
         * @return 0 on success; non-zero otherwise
         */
        mraa_result_t off();
        /**
         * Turn the LED on
         *
         * @return 0 on success; non-zero otherwise
         */
        mraa_result_t on();
    private:
        mraa_gpio_context m_gpio;
};

/**
 * @brief C++ API for Grove Relay
 *
 * UPM module for Grove relay switch.  The Grove relay is a
 * digital normally-open switch that uses low voltage or current to 
 * control a higher voltage and/or higher current.  When closed, 
 * the indicator LED will light up and current is allowed to flow.
 *
 * @ingroup grove gpio
 * @snippet groverelay.cxx Interesting
 */
class GroveRelay: public Grove {
    public:
        /**
         * Grove relay constructor
         *
         * @param gpio pin to use
         */
        GroveRelay(unsigned int pin);
        /**
         * Grove relay destructor
         */
        ~GroveRelay();
        /**
         * Set the relay switch to on (close).  This allows current
         * to flow and lights up the indicator LED.
         *
         * @return 0 on success; non-zero otherwise
         */
        mraa_result_t on();
        /**
         * Set the relay switch to off (open).  This stops current
         * from flowing and the indicator LED will not be lit.
         *
         * @return 0 on success; non-zero otherwise
         */
        mraa_result_t off();
        /**
         * Returns whether or not the relay switch is closed.
         *
         * @return true if the switch is on (closed); false otherwise
         */
        bool isOn();
        /**
         * Returns whether or not the relay switch is open.
         *
         * @return true if the switch is off (open); false otherwise
         */
        bool isOff();
    private:
        mraa_gpio_context m_gpio;
};

/**
 * @brief C++ API for Grove temperature sensor
 *
 * Very basic UPM module for Grove temperature sensor on analog
 *
 * @ingroup grove analog
 * @snippet grovetemp.cxx Interesting
 */
class GroveTemp: public Grove {
    public:
        /**
         * Grove analog temperature sensor constructor
         *
         * @param pin analog pin to use
         */
        GroveTemp(unsigned int pin);
        /**
         * GroveTemp destructor
         */
        ~GroveTemp();
        /**
         * Get raw value from AIO pin
         *
         * @return the raw value from the ADC
         */
        float raw_value();
        /**
         * Get the temperature in Celsius from the sensor
         *
         * @return the normalized temperature in Celsius
         */
        int value();
    private:
        mraa_aio_context m_aio;
};

/**
 * @brief C++ API for Grove light sensor
 *
 * The Grove light sensor detects the intensity of the ambient light.
 * As the light intensity of the environment increases, the resistance
 * of the sensor decreases.  This means that the raw value from the
 * analog pin will be larger in bright light and smaller in the dark.
 * An approximate lux value can also be returned.
 *
 * @ingroup grove analog
 * @snippet grovelight.cxx Interesting
 */
class GroveLight: public Grove {
    public:
        /**
         * Grove analog light sensor constructor
         *
         * @param pin analog pin to use
         */
        GroveLight(unsigned int pin);
        /**
         * GroveLight Destructor
         */
        ~GroveLight();
        /**
         * Get raw value from AIO pin
         *
         * @return the raw value from the ADC
         */
        float raw_value();
        /**
         * Get the approximate light value, in lux, from the sensor
         *
         * @return the normalized light reading in lux
         */
        int value();
    private:
        mraa_aio_context m_aio;
};

/**
 * @brief C++ API for Grove Rotary Angle Sensor (Knob)
 *
 * Very basic UPM module for Grove rotary angle sensor (knob) on analog. Provides
 * a set of functions to read the absolute pin value, degrees or radians and another
 * to do the same relative to the center of the knob's range.
 *
 * @ingroup grove analog
 * @snippet groverotary.cxx Interesting
 * @image html groverotary.jpeg
 */
class GroveRotary: public Grove {
    public:
        /**
         * Grove rotary angle sensor constructor
         *
         * @param pin number of analog pin to use
         */
        GroveRotary(unsigned int pin);
        /**
         * GroveRotary Destructor
         */
        ~GroveRotary();
        /**
         * Get absolute raw value from AIO pin
         *
         * @return the unsigned value from the ADC
         */
        float abs_value();
        /**
         * Get absolute raw degrees from AIO pin
         *
         * @return the unsigned degrees from the ADC
         */
        float abs_deg();
        /**
         * Get absolute raw radians from AIO pin
         *
         * @return the unsigned radians from the ADC
         */
        float abs_rad();
        /**
         * Get the relative value from the pin
         *
         * @return the signed value from the ADC
         */
        float rel_value();
        /**
         * Get relative degrees from AIO pin
         *
         * @return the signed degrees from the ADC
         */
        float rel_deg();
        /**
         * Get relative radians from AIO pin
         *
         * @return the signed radians from the ADC
         */
        float rel_rad();
    private:
        mraa_aio_context m_aio;
        static const int m_max_angle = 300;
};

/**
 * @brief C++ API for Grove Slide Potentiometer
 *
 * Very basic UPM module for Grove slide potentiometer on analog,
 * returns either raw value or scaled voltage value.
 *
 * @ingroup grove analog
 * @snippet groveslide.cxx Interesting
 * @image html groveslide.jpeg
 */
class GroveSlide: public Grove {
    public:
        /**
         * Grove analog slide potentiometer constructor
         *
         * @param pin number of analog pin to use
         *
         * @param ref_voltage the reference voltage the board is set to as float, e.g. 3.3 or 5.0 (default)
         */
        GroveSlide(unsigned int pin, float ref_voltage = 5.0);
        /**
         * GroveSlide Destructor
         */
        ~GroveSlide();
        /**
         * Get raw value from AIO pin
         *
         * @return the raw value from the ADC
         */
        float raw_value();
        /**
         * Get the voltage value from the pin
         *
         * @return the voltage reading based on the reference voltage
         */
        float voltage_value();
        /**
         * Get the board's reference voltage passed on object initialization
         *
         * @return the reference voltage the class was set for
         */
        float ref_voltage();
    private:
        mraa_aio_context m_aio;
        float m_ref_voltage;
};

/**
 * @brief C++ API for Grove button
 *
 * Very basic UPM module for Grove button
 *
 * @ingroup grove gpio
 * @snippet grovebutton.cxx Interesting
 */
class GroveButton: public Grove {
    public:
        /**
         * Grove button constructor
         *
         * @param gpio pin to use
         */
        GroveButton(unsigned int pin);
        /**
         * Grove button destructor
         */
        ~GroveButton();
        /**
         * Get name of sensor
         *
         * @return the name of this sensor
         */
        std::string name();
        /**
         * Get value from GPIO pin
         *
         * @return the value from the GPIO pin
         */
        int value();
    private:
        std::string m_name;
        mraa_gpio_context m_gpio;
};

}
