/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <atomic>
#include <iostream>
#include <map>
#include <list>
#include <mutex>
#include <queue>
#include <cstdlib>
#include <string>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

#include "nmea_gps.h"
#include <interfaces/iGps.hpp>

namespace upm {
    /**
     * @brief Generic NMEA GPS Serial Device Library
     * @defgroup nmea_gps libupm-nmea_gps
     * @ingroup uart gpio gps
     */

    /**
     * @file nmea_gps.hpp
     * @library nmea_gps
     * @sensor nmea_gps
     * @comname Generic Serial Interface for GPS NMEA Devices
     * @type gps
     * @man dfrobot seeed
     * @con uart gpio
     * @altname VK2828u7 ublox LEA-6H
     *
     * @brief API for the NMEA GPS Module
     *
     * This driver was tested with a number of GPS devices that emit
     * NMEA data via a serial interface of some sort (typically a UART).
     *
     * The I2C capablity was tested with a UBLOX LEA-6H based GPS shield
     * from DFRobot.  Currently, the I2C capability is only supported
     * for UBLOX devices (or compatibles) that conform to the
     * specifications outlined in the u-blox6 Receiver Description
     * Protocol Specification, Chapter 4, DDC Port.
     *
     * An example using the UART.
     * @snippet nmea_gps.cxx Interesting
     * An example using I2C.
     * @snippet nmea_gps-i2c.cxx Interesting
     */
    class NMEAGPS;

    /** Coordinates for lat/long as decimal degrees (DD) */
    struct coord_DD {
        /** Latitude in decimal degrees */
        double latitude = 0.0;
        /** Longitude in decimal degrees */
        double longitude = 0.0;
        /**
         * Provide a string representation of this structure.
         * @return String representing coordinates
         */
        std::string __str__();
    };

    /** Satellite structure definition */
    struct satellite {
        /** PRN pseudo-random-noise value which identifies a satellite */
        std::string prn;
        /** Satellite elevation angle in degrees */
        int elevation_deg;
        /** Satellite azimuth angle in degrees */
        int azimuth_deg;
        /** Satellite signal-to-noise ratio */
        int snr;
        /** Default constructor */
        satellite():satellite("", 0, 0, 0){}
        /**
         * Create a satellite from arguments constructor
         * @param sprn Target PRN string
         * @param elevation Target elevation angle in degrees
         * @param azimuth Target azimuth angle in degrees
         * @param snr Target signal to noise ratio (usually in dB,
         * unfortunately non-standard)
         */
        satellite(const std::string& sprn, int elevation, int azimuth, int snr):
            prn(sprn), elevation_deg(elevation), azimuth_deg(azimuth), snr(snr) {}
        /**
         * Provide a string representation of this structure.
         * @return String representing a satellite
         */
        std::string __str__();
    };

    /** Text structure definition */
    struct nmeatxt {
        /** Message severity */
        int severity;
        /** Message text */
        std::string message;
        /** Default constructor */
        nmeatxt():nmeatxt(0, "") {}
        /**
         * Create a nmeatxt from arguments constructor
         * @param severity Message severity
         * @param message Target message
         */
        nmeatxt(int severity, const std::string& message):
                severity(severity), message(message){}
        /**
         * Provide a string representation of this structure.
         * @return String representing a nmeatxt
         */
        std::string __str__();
    };

    /** GPS fix quality values */
    enum class gps_fix_quality {
        /** No fix available or invalid */
        no_fix = 0,
        /** Fix - single point */
        fix_sp,
        /** Fix - differential point */
        fix_dp,
        /** Fix - pulse per second */
        fix_pps,
        /** Fix - real time kinematic */
        fix_rtk,
        /** Fix - float real time kinematic */
        fix_frtk,
        /** Fix - dead reckoning */
        fix_dr,
        /** Fix - manual input */
        fix_manual,
        /** Fix - simulation mode */
        fix_simulation
    };

    /** GPS fix definition.  A GPS fix structure should only be used if
     * valid == true
     */
    struct gps_fix {
        /** Fix coordinates */
        coord_DD coordinates;
        /** UTC time string as HHMMSS.mS */
        std::string time_utc = std::string("");
        /** GPS fix signal quality */
        gps_fix_quality quality = gps_fix_quality::no_fix;
        /** Number of satellites in use */
        uint8_t satellites = 0;
        /** Horizontal dilution of precision, unitless, lower is better */
        float hdop = 0.0;
        /** Altitude above mean sea level in meters */
        float altitude_meters = 0.0;
        /** Difference between the WGS-84 earth ellipsoid and mean-sea-level */
        float geoid_height_meters = 0.0;
        /** Time in seconds since last differential GPS fix */
        float age_seconds = 0.0;
        /** Differential GPS station ID */
        std::string station_id = std::string("");
        /** True if this gps_fix structure is valid to use */
        bool valid = false;
        /** True if the checksum matched, valid is set to false on mismatch */
        bool chksum_match = false;
        /**
         * Provide a string representation of this structure.
         * @return String representing a GPS Fix
         */
        std::string __str__();
    };

    class NMEAGPS : virtual public iGps {
        public:

            /**
             * NMEAGPS object constructor for a UART
             *
             * @param uart Specify which mraa uart index to use.
             * @param baudrate Specify the baudrate to use.  The device defaults
             * to 9600 baud.
             * @param enable_pin Specify the GPIO pin to use for the enable pin,
             * -1 to not use an enable pin.
             */
            NMEAGPS(unsigned int uart, unsigned int baudrate,
                    int enable_pin);

            /**
             * NMEAGPS object constructor for a UART
             *
             * @param uart Specify which uart to use (fs device path)
             * @param baudrate Specify the baudrate to use.  The device defaults
             * to 9600 baud.
             */
            NMEAGPS(const std::string& uart, unsigned int baudrate);

            /**
             * NMEAGPS object constructor for a UBLOX I2C interface
             *
             * @param bus Specify which the I2C bus to use.
             * @param addr Specify the I2C address to use.  For UBLOX devices,
             * this typically defaults to 0x42.
             */
            NMEAGPS(unsigned int bus, uint8_t addr);

            /**
             * NMEAGPS object destructor
             */
            ~NMEAGPS();

            /**
             * Read character data from the device.
             *
             * @param size The maximum number of characters to read.
             * @return string containing the data read.
             */
            std::string readStr(size_t size);

            /**
             * Write character data to the device.  This is only valid for a
             * UART device.
             *
             * @param buffer The string containing the data to write.
             * @return The number of bytes written.
             */
            int writeStr(const std::string& buffer);

            /**
             * Enable or disable the device.  When disabled, the device enters a
             * low power mode and does not emit NMEA data.  It will still
             * maintain location data however.
             *
             * @param enable true to enable the device, false otherwise.
             */
            virtual void enable(bool enable);

            /**
             * Set the baudrate of the device.  By default, the constructor
             * will set the baudrate to 9600.  This is only valid for UART
             * devices.
             *
             * @param baudrate The baud rate to set for the device.
             */
            virtual void setBaudrate(unsigned int baudrate);

            /**
             * Determine whether there is data available to be read.  In the
             * case of a UART, this function will wait up to "millis"
             * milliseconds for data to become available.  In the case of an I2C
             * device, the millis argument is ignored and the function will
             * return immediately, indicating whether data is available.
             *
             * @param millis The number of milliseconds to wait for data to
             * become available.
             * @return true if data is available to be read, false otherwise.
             */
            bool dataAvailable(unsigned int millis);

            /**
             * Return the current maximum queue depth.
             * @return Maximum queue depth
             */
            size_t getMaxQueueDepth();

            /**
             * Set the current maximum queue depth.
             * @param depth New target queue depth
             *      1 <= depth <= 1000
             * @return Actual maximum queue depth
             */
            size_t setMaxQueueDepth(size_t depth);

            /**
             * Start a NMEA parsing thread for reading/parsing NMEA sentences.  The
             * thread calls the readStr method, parsing NMEA sentences as they are
             * encountered.  Each sentence type is pushed into a corresponding queue
             * of size
             */
            void parseStart();

            /**
             * Stop a running NMEA parsing thread
             */
            void parseStop();

            /**
             * Is the parsing thread currently running?
             * @return True if parsing
             */
            bool isRunning() {return _running;}

            /**
             * Pop and return a GPS fix structure from the GPS fix queue.
             * A GPS fix should only be used if valid is true.
             * @return GPS fix structure
             */
            gps_fix getFix();

            /**
             * Pop and return a raw NMEA sentence from the NMEA sentence queue.
             * If the queue contains no elements, an empty string is returned
             * @return NMEA raw sentence
             */
            std::string getRawSentence();

            /**
             * Pop and return a message from the message queue (GPTXT sentences)
             * If the queue contains no elements, an empty string is returned
             * @return NMEA text message
             */
            nmeatxt getTxtMessage();

            /**
             * Get the number of elements in the GPS fix queue.
             * @return Number of fixes in the GPS fix queue
             */
            size_t fixQueueSize();

            /**
             * Get the number of elements in the NMEA raw sentence queue.
             * @return Number of sentences in the raw NMEA sentence queue
             */
            size_t rawSentenceQueueSize();

            /**
             * Get the number of messages in the NMEA message queue.
             * @return Number of messages in the message queue
             */
            size_t txtMessageQueueSize();

            /**
             * Parse NMEA sentences.
             * Raw sentence is placed into sentence queue.  Additional structures are
             * parsed depending on sentence type
             * @param sentence NMEA raw sentence ($...\r\n) inclusive
             */
            void parseNMEASentence(const std::string& sentence);

            /**
             * Return a vector of the current satellites
             * @return Current satellites
             */
            std::vector<satellite> satellites();

            /**
             * Get the number of sentences parsed per second
             * @return Sentences parsed per second
             */
            double sentencesPerSecond();

            /**
             * Get the throughput of the device.  This number shows a rough
             * data-rate as well as provides a bit of debug since it will show
             * bps even if sentences are not getting parsed correctly.

             * @return Total bytes read from the device/second (bps)
             */
            double bytesPerSecond();

            /**
             * Provide a string representation of this class
             * @return String representing this instance
             */
            std::string __str__();
        protected:
            /** nmeaGPS device context */
            nmea_gps_context m_nmea_gps;

        private:
            /** Disable implicit copy and assignment operators */
            NMEAGPS(const NMEAGPS&) = delete;
            NMEAGPS &operator=(const NMEAGPS&) = delete;

            /** Handle reading/parsing NMEA data */
            std::thread _parser;

            /** Method runs in a spawned thread for parsing NMEA sentences */
            void _parse_thread();

            /** Helper for thread syncronization */
            std::atomic<bool> _running;

            /** Parse GPGGA sentences, place in GPS fix queue */
            void _parse_gpgga(const std::string& string);
            /** Parse GPGSV sentences, place in satellite collection */
            void _parse_gpgsv(const std::string& string);
            /** Parse GPGLL sentences, place in satellite collection */
            void _parse_gpgll(const std::string& string);
            /** Parse GPTXT sentences, place in text collection */
            void _parse_gptxt(const std::string& string);

            /** Provide function pointer typedef for handling NMEA chunks */
            using fp = void (NMEAGPS::*)(const std::string &);
            /** Map of NMEA type to parser method */
            const std::map<std::string, fp> nmea_2_parser =
            {
                {"GPGGA", &NMEAGPS::_parse_gpgga},
                {"GPGSV", &NMEAGPS::_parse_gpgsv},
                {"GPGLL", &NMEAGPS::_parse_gpgll},
                {"GPTXT", &NMEAGPS::_parse_gptxt},
            };

            /** Raw NMEA sentence fix queue */
            std::queue<std::string> _queue_nmea_sentence;
            std::mutex _mtx_nmea_sentence;

            /** GPS fix queue */
            std::queue<gps_fix> _queue_fix;
            std::mutex _mtx_fix;

            /** Message queue */
            std::queue<nmeatxt> _queue_txt;
            std::mutex _mtx_txt;

            /** Specify a queue size for parsed objects */
            std::atomic<size_t> _maxQueueDepth;

            /** Count # of parsed sentences each time thread is started */
            std::atomic<size_t> _sentences_since_start;

            /** Count # oharacters read each time thread is started */
            std::atomic<size_t> _bytes_since_start;

            /** Count # of parsed sentences each time thread is started */
            std::atomic<double> _seconds_since_start;

            /** Set of current satellites */
            std::list<satellite> _satlist;
            std::mutex _mtx_satlist;
    };
}
