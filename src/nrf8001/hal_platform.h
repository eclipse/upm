/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file
 * @brief HAL specific macros
 * @ingroup nrf8001
 */
 
#ifndef PLATFORM_H__
#define PLATFORM_H__

//Board dependent defines
#if defined (__AVR__)
    //For Arduino this AVR specific library has to be used for reading from Flash memory
    #include <avr/pgmspace.h>
    #include "Arduino.h"
    #ifdef PROGMEM
        #undef PROGMEM
        #define PROGMEM __attribute__(( section(".progmem.data") ))
        #endif
#elif defined(__PIC32MX__)
    //For Chipkit add the following libraries.
    #include <stdint.h>
    #include <stdbool.h>
    #include <string.h>
    #include <wiring.h>
    #include <WProgram.h>

    //For making the Serial.Print compatible between Arduino and Chipkit
    #define F(X) (X)

    //For ChipKit neither PROGMEM or PSTR are needed for PIC32
    #define PROGMEM
    #define PSTR(s) (s)

    #define pgm_read_byte(x)            (*((char *)x))
    #define pgm_read_byte_near(x)   (*((char *)x))
    #define pgm_read_byte_far(x)        (*((char *)x))
    #define pgm_read_word(x)            (*((short *)x))
    #define pgm_read_word_near(x)   (*((short *)x))
    #define pgm_read_workd_far(x)   (*((short *)x))

    #define prog_void       const void
    #define prog_char       const char
    #define prog_uchar      const unsigned char
    #define prog_int8_t     const int8_t
    #define prog_uint8_t    const uint8_t
    #define prog_int16_t    const int16_t
    #define prog_uint16_t   const uint16_t
    #define prog_int32_t    const int32_t
    #define prog_uint32_t   const uint32_t
    #define prog_int64_t    const int64_t
    #define prog_uint64_t   const uint64_t

    //Redefine the function for reading from flash in ChipKit
    #define memcpy_P        memcpy
#endif

#endif /* PLATFORM_H__ */
