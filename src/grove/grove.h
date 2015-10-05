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
#include <mraa/aio.hpp>
#include <mraa/gpio.hpp>

#ifdef JAVACALLBACK
#include "../IsrCallback.h"
#endif

namespace upm {

/**
 * @brief Generic library for basic Grove sensors
 * @defgroup grove libupm-grove
 * @ingroup seeed gpio pwm ainput button led light relay temp touch gsk eak hak
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
 * @brief API for the Grove LED
 *
 * UPM module for the Grove LED (or other similar light-emitting diodes).
 * An LED is a small lightbulb that emits light in 
 * response to a small current. The longer wire of an LED connects
 * to the positive seat (anode); the shorter wire connects to the
 * negative seat (cathode). The flat side of the bulb corresponds
 * to the cathode, while the rounded side corresponds to the anode.
 *
 * @ingroup grove gpio
 * @snippet groveled.cxx Interesting
 * @snippet groveled-multi.cxx Interesting
 * @image html groveled.jpg
 */
class GroveLed: public Grove {
    public:
        /**
         * Grove LED constructor
         *
         * @param gpio Pin to use
         */
        GroveLed(int pin);
        /**
         * Grove LED destructor
         */
        ~GroveLed();
        /**
         * Turns the LED on or off, depending on the value.
         * If the value is positive (greater than or equal
         * to 1), the LED is turned on.  Otherwise, for 0
         * or negative values, the LED is turned off.
         *
         * @param value Tells the LED to turn on (for values >=1)
         * or off (for values <1)
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t write(int value);
        /**
         * Turns the LED off
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t off();
        /**
         * Turns the LED on
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t on();
    private:
        mraa_gpio_context m_gpio;
};

/**
 * @brief API for the Grove Relay
 *
 * UPM module for the Grove relay switch. Grove relay is a
 * digital normally-open switch that uses low voltage or current to 
 * control a higher voltage and/or higher current.  When closed, 
 * the indicator LED lights up and current is allowed to flow.
 *
 * @ingroup grove gpio
 * @snippet groverelay.cxx Interesting
 * @image html groverelay.jpg
 */
class GroveRelay: public Grove {
    public:
        /**
         * Grove relay constructor
         *
         * @param gpio Pin to use
         */
        GroveRelay(unsigned int pin);
        /**
         * Grove relay destructor
         */
        ~GroveRelay();
        /**
         * Sets the relay switch to on (closed). This allows current
         * to flow and lights up the indicator LED.
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t on();
        /**
         * Sets the relay switch to off (open). This stops current
         * from flowing and the indicator LED is not lit.
         *
         * @return 0 if successful, non-zero otherwise
         */
        mraa_result_t off();
        /**
         * Defines whether the relay switch is closed.
         *
         * @return True if the switch is on (closed), false otherwise
         */
        bool isOn();
        /**
         * Defines whether the relay switch is open.
         *
         * @return True if the switch is off (open), false otherwise
         */
        bool isOff();
    private:
        mraa_gpio_context m_gpio;
};

/**
 * @brief API for the Grove Temperature Sensor
 *
 * Basic UPM module for the Grove temperature sensor on analog
 *
 * @ingroup grove analog
 * @snippet grovetemp.cxx Interesting
 * @image html grovetemp.jpg
 */
class GroveTemp: public Grove {
    public:
        /**
         * Grove analog temperature sensor constructor
         *
         * @param pin Analog pin to use
         */
        GroveTemp(unsigned int pin);
        /**
         * GroveTemp destructor
         */
        ~GroveTemp();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /**
         * Gets the temperature in Celsius from the sensor
         *
         * @return Normalized temperature in Celsius
         */
        int value();
    private:
        mraa_aio_context m_aio;
};

/**
 * @brief API for the Grove Light Sensor
 *
 * The Grove light sensor detects the intensity of the ambient light.
 * As the light intensity of the environment increases, the resistance
 * of the sensor decreases. This means the raw value from the
 * analog pin is greater in bright light and smaller in the dark.
 * An approximate lux value can also be returned.
 *
 * @ingroup grove analog
 * @snippet grovelight.cxx Interesting
 * @image html grovelight.jpg
 */
class GroveLight: public Grove {
    public:
        /**
         * Grove analog light sensor constructor
         *
         * @param pin Analog pin to use
         */
        GroveLight(unsigned int pin);
        /**
         * GroveLight destructor
         */
        ~GroveLight();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /**
         * Gets an approximate light value, in lux, from the sensor
         *
         * @return Normalized light reading in lux
         */
        int value();
    private:
        mraa_aio_context m_aio;
};

/**
 * @brief API for the Grove Rotary Angle Sensor (Knob)
 *
 * Basic UPM module for the Grove rotary angle sensor (knob) on analog. Provides
 * a set of functions to read the absolute pin value, degrees or radians, and another set
 * to do the same relative to the center of the knob range.
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
         * @param pin Number of the analog pin to use
         */
        GroveRotary(unsigned int pin);
        /**
         * GroveRotary destructor
         */
        ~GroveRotary();
        /**
         * Gets the absolute raw value from the AIO pin
         *
         * @return Unsigned value from the ADC
         */
        float abs_value();
        /**
         * Gets absolute raw degrees from the AIO pin
         *
         * @return Unsigned degrees from the ADC
         */
        float abs_deg();
        /**
         * Gets absolute raw radians from the AIO pin
         *
         * @return Unsigned radians from the ADC
         */
        float abs_rad();
        /**
         * Gets the relative value from the AIO pin
         *
         * @return Signed value from the ADC
         */
        float rel_value();
        /**
         * Gets relative degrees from the AIO pin
         *
         * @return Signed degrees from the ADC
         */
        float rel_deg();
        /**
         * Gets relative radians from the AIO pin
         *
         * @return Signed radians from the ADC
         */
        float rel_rad();
    private:
        mraa_aio_context m_aio;
        static const int m_max_angle = 300;
};

/**
 * @brief API for the Grove Slide Potentiometer
 *
 * Basic UPM module for the Grove slide potentiometer on analog that
 * returns either a raw value or a scaled voltage value.
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
         * @param pin Number of the analog pin to use
         *
         * @param ref_voltage Reference voltage the board is set to, as a floating-point value; default is 5.0V
         */
        GroveSlide(unsigned int pin, float ref_voltage = 5.0);
        /**
         * GroveSlide destructor
         */
        ~GroveSlide();
        /**
         * Gets the raw value from the AIO pin
         *
         * @return Raw value from the ADC
         */
        float raw_value();
        /**
         * Gets the voltage value from the pin
         *
         * @return Voltage reading based on the reference voltage
         */
        float voltage_value();
        /**
         * Gets the board's reference voltage passed on object initialization
         *
         * @return Reference voltage the class was set for
         */
        float ref_voltage();
    private:
        mraa_aio_context m_aio;
        float m_ref_voltage;
};

/**
 * @brief API for the Grove Button
 *
 * Basic UPM module for the Grove button
 *
 * @ingroup grove gpio
 * @snippet grovebutton.cxx Interesting
 * @image html grovebutton.jpg
 */
class GroveButton: public Grove {
    public:
        /**
         * Grove button constructor
         *
         * @param gpio Pin to use
         */
        GroveButton(unsigned int pin);
        /**
         * Grove button destructor
         */
        ~GroveButton();
        /**
         * Gets the name of the sensor
         *
         * @return Name of this sensor
         */
        std::string name();
        /**
         * Gets the value from the GPIO pin
         *
         * @return Value from the GPIO pin
         */
        int value();

        /**
         * Installs an interrupt service routine (ISR) to be called when
         * the button is activated or deactivated.
         *
         * @param fptr Pointer to a function to be called on interrupt
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        void installISR(mraa::Edge level, IsrCallback *cb);
#else
        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);
#endif
        /**
         * Uninstalls the previously installed ISR
         *
         */
        void uninstallISR();

    private:
#if defined(SWIGJAVA) || defined(JAVACALLBACK)
        void installISR(mraa::Edge level, void (*isr)(void *), void *arg);
#endif
        bool m_isrInstalled;
        std::string m_name;
        mraa_gpio_context m_gpio;
};

}
