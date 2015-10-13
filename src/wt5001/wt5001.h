/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mraa/uart.h>

const int WT5001_DEFAULT_UART = 0;
const int WT5001_MAX_VOLUME = 31;

// protocol start and end codes
const uint8_t WT5001_START = 0x7e;
const uint8_t WT5001_END   = 0x7e;

namespace upm {
    /**
     * @brief WT5001 Serial MP3 module library
     * @defgroup wt5001 libupm-wt5001
     * @ingroup seeed uart sound
     */
    /**
     * @library wt5001
     * @sensor wt5001
     * @comname WT5001 Serial MP3 Module
     * @altname Grove Serial MP3 Player
     * @type sound
     * @man seeed
     * @web http://www.seeedstudio.com/wiki/Grove_%E2%80%93_Serial_MP3_Player
     * @con uart
     *
     * @brief API for the WT5001 Serial MP3 Module
     *
     *   UPM support for the WT5001 Serial MP3 module. This was tested
     *   specifically with the Grove Serial MP3 module.
     *
     * @image html wt5001.jpg
     * @snippet wt5001.cxx Interesting
     */
  class WT5001 {
  public:

    // WT5001 opcodes
    typedef enum { NONE             = 0x00,
                   PLAY_SD          = 0xa0,
                   PLAY_SPI         = 0xa1,
                   PLAY_UDISK       = 0xa2,
                   PAUSE            = 0xa3,
                   STOP             = 0xa4,
                   NEXT             = 0xa5,
                   PREVIOUS         = 0xa6,
                   SET_VOLUME       = 0xa7,
                   QUEUE            = 0xa8,
                   PLAY_MODE        = 0xa9,
                   COPY_SD2FLASH    = 0xaa, // not implemented
                   COPY_UDISK2FLASH = 0xab, // not implemented
                   INSERT_SONG      = 0xac,
                   SET_DATE         = 0xb1,
                   SET_TIME         = 0xb2,
                   SET_ALARM        = 0xb3,
                   SET_ALARM_DUR    = 0xb4, // not implemented
                   CLEAR_ALARM      = 0xb5,
                   CLEAR_ALARM_DUR  = 0xb6, // not implemented
                   READ_VOLUME      = 0xc1,
                   READ_PLAY_STATE  = 0xc2,
                   READ_SPI_NUMF    = 0xc3,
                   READ_SD_NUMF     = 0xc4,
                   READ_UDISK_NUMF  = 0xc5,
                   READ_CUR_FNAME   = 0xc6,
                   READ_CF_CHAR     = 0xc7, // not implemented
                   READ_DATE        = 0xd1,
                   READ_TIME        = 0xd2
    } WT5001_OPCODE_T;

    // play modes
    typedef enum { NORMAL           = 0x00,
                   SINGLE_REPEAT    = 0x01,
                   ALL_REPEAT       = 0x02,
                   RANDOM           = 0x03
    } WT5001_PLAYMODE_T;

    // music source
    typedef enum { SD,
                   SPI,
                   UDISK
    } WT5001_PLAYSOURCE_T;

    /**
     * WT5001 constructor
     *
     * @param uart Default UART to use (0 or 1)
     */
    WT5001(int uart);

    /**
     * WT5001 destructor
     */
    ~WT5001();

    /**
     * Checks to see if there is data available for reading
     *
     * @param millis Number of milliseconds to wait; 0 means no waiting
     * @return True if there is data available for reading
     */
    bool dataAvailable(unsigned int millis);

    /**
     * Reads any available data in a user-supplied buffer. Note: the
     * call blocks until data is available to be read. Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes read
     */
    int readData(char *buffer, int len);

    /**
     * Writes the data in the buffer to the device
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(char *buffer, int len);

    /**
     * Sets up proper tty I/O modes and the baud rate. The default
     * baud rate is 9,600 (B9600).
     *
     * @param baud Desired baud rate.
     * @return True if successful
     */
    bool setupTty(speed_t baud=B9600);

    /**
     * Gets a command response and returns its validity
     *
     * @param index Opcode to verify
     * @return True if successful
     */
    bool checkResponse(WT5001_OPCODE_T opcode);

    /**
     * Plays a file from a source
     *
     * @param psrc Play source (SD, UDISK, SPI)
     * @param index File number to play
     * @return True if successful
     */
    bool play(WT5001_PLAYSOURCE_T psrc, uint16_t index);

    /**
     * Stops playing
     *
     * @return True if successful
     */
    bool stop();

    /**
     * Pauses the playback or resumes it if already paused
     *
     * @return True if successful
     */
    bool pause();

    /**
     * Moves to the next track
     *
     * @return True if successful
     */
    bool next();

    /**
     * Moves to the previous track
     *
     * @return True if successful
     */
    bool previous();

    /**
     * Sets the volume. Valid range is 0-31. 0 means mute.
     *
     * @return True if successful
     */
    bool setVolume(uint8_t vol);

    /**
     * Queues a track to play next, when the current song is finished
     *
     * @param index File number to queue
     * @return True if successful
     */
    bool queue(uint16_t index);

    /**
     * Sets the playback mode
     *
     * @param pm Play mode to enable
     * @return True if successful
     */
    bool setPlayMode(WT5001_PLAYMODE_T pm);

    /**
     * Inserts a track to play immediately, interrupting the current
     * track. When the inserted track is finished, the
     * interrupted track resumes where it was interrupted.
     *
     * @param index File number to insert
     * @return True if successful
     */
    bool insert(uint16_t index);

    /**
     * Sets the date of the internal clock
     *
     * @param year 4-digit year
     * @param month Month
     * @param day Day
     * @return True if successful
     */
    bool setDate(uint16_t year, uint8_t month, uint8_t day);

    /**
     * Sets the time of the internal clock
     *
     * @param hour Hour
     * @param minute Minute
     * @param second Second
     * @return True if successful
     */
    bool setTime(uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * Sets the alarm
     *
     * @param hour Hour
     * @param minute Minute
     * @param second Second
     * @return True if successful
     */
    bool setAlarm(uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * Clears any alarm that has been set
     *
     * @return True if successful
     */
    bool clearAlarm();

    /**
     * Gets the current volume
     *
     * @param vol Volume
     * @return True if successful
     */
    bool getVolume(uint8_t *vol);

    /**
     * Gets the current play state: 1 = playing, 2 = stopped, 3 = paused
     *
     * @param ps Play state
     * @return True if successful
     */
    bool getPlayState(uint8_t *ps);

    /**
     * Gets the number of files present on the source device
     *
     * @param psrc Storage source
     * @param numf Number of files
     * @return True if successful
     */
    bool getNumFiles(WT5001_PLAYSOURCE_T psrc, uint16_t *numf);

    /**
     * Gets the index of the current file
     *
     * @param curf Index of the current file
     * @return True if successful
     */
    bool getCurrentFile(uint16_t *curf);

    /**
     * Gets the device date
     *
     * @param year 4-digit year
     * @param month Month
     * @param day Day
     * @return True if successful
     */
    bool getDate(uint16_t *year, uint8_t *month, uint8_t *day);

    /**
     * Gets the device time
     *
     * @param hour Hour
     * @param minute Minute
     * @param second Second
     * @return True if successful
     */
    bool getTime(uint8_t *hour, uint8_t *minute, uint8_t *second);


  protected:
    int ttyFd() { return m_ttyFd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
  };
}


