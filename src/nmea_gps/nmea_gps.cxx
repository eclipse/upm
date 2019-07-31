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

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>

#include "upm_utilities.h"
#include "nmea_gps.hpp"

using namespace upm;
using namespace std;

NMEAGPS::NMEAGPS(unsigned int uart, unsigned int baudrate,
                 int enable_pin) :
  m_nmea_gps(nmea_gps_init(uart, baudrate, enable_pin)),
    _running(false),
    _maxQueueDepth(10),
    _sentences_since_start(0),
    _bytes_since_start(0),
    _seconds_since_start(0.0)
{
  if (!m_nmea_gps)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_init() failed");
}

NMEAGPS::NMEAGPS(const std::string& uart, unsigned int baudrate) :
  m_nmea_gps(nmea_gps_init_raw(uart.c_str(), baudrate)),
    _running(false),
    _maxQueueDepth(10),
    _sentences_since_start(0),
    _bytes_since_start(0),
    _seconds_since_start(0.0)
{
  if (!m_nmea_gps)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_init() failed");
}

NMEAGPS::NMEAGPS(unsigned int bus, uint8_t addr) :
  m_nmea_gps(nmea_gps_init_ublox_i2c(bus, addr)),
    _running(false),
    _maxQueueDepth(10),
    _sentences_since_start(0),
    _bytes_since_start(0),
    _seconds_since_start(0.0)
{
  if (!m_nmea_gps)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_init() failed");
}

NMEAGPS::~NMEAGPS()
{
  _running = false;
  if (_parser.joinable())
    _parser.join();
  nmea_gps_close(m_nmea_gps);
}

std::string NMEAGPS::readStr(size_t size)
{
  char buffer[size];

  int rv;

  if ((rv = nmea_gps_read(m_nmea_gps, buffer, size)) < 0)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_read() failed");

  /* Keep track of bytes read */
  _bytes_since_start += rv;
  return buffer;
}

int NMEAGPS::writeStr(const std::string& buffer)
{
  int rv;

  /* The write takes a char*.  This should be OK since it's known that the mraa
   * write call does not change buffer */
  if ((rv = nmea_gps_write(m_nmea_gps, const_cast<char*>(buffer.c_str()),
                           buffer.size())) < 0)
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_write() failed");

  return rv;
}

void NMEAGPS::enable(bool enable)
{
  if (nmea_gps_enable(m_nmea_gps, enable))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_enable() failed");
}

void NMEAGPS::setBaudrate(unsigned int baudrate)
{
  if (nmea_gps_set_baudrate(m_nmea_gps, baudrate))
    throw std::runtime_error(string(__FUNCTION__)
                             + ": nmea_gps_baudrate() failed");
}

bool NMEAGPS::dataAvailable(unsigned int millis)
{
  return nmea_gps_data_available(m_nmea_gps, millis);
}

size_t NMEAGPS::getMaxQueueDepth()
{
    return _maxQueueDepth;
}

size_t NMEAGPS::setMaxQueueDepth(size_t depth)
{
    /* 1 <= depth <= 1000 */
    if (depth > 1000) depth = 1000;
    if (depth == 0) depth = 1;

    _maxQueueDepth = depth;
    return _maxQueueDepth;
}

/* Given a NMEA sentence, return a checksum which is calculated on
 * all characters between the '$' and the '*' */
static uint8_t checksum(const std::string& sentence)
{
    uint8_t chksum = 0;
    std::string::const_iterator it = sentence.begin();
    /* Skip the '$' */
    if (*it == '$') ++it;

    /* Calculate the checksum on all characters */
    while ((*it != '*') && (it != sentence.end()))
        chksum ^= *it++;
    return chksum;
}

/* Regex for matching NMEA GGA coordinates
 * Unfortunately these sentences appear-non standard between the devices tested
 * so it can be expected that these would need updating to match additional
 * devices.
 * GPGGA,164800.00,4532.52680,N,12257.59972,W,1,10,0.93,73.3,M,-21.3,M,,*5E
 */
static std::regex rex_gga(R"(^\$GPGGA,(\d+\.\d+),(\d+)(\d{2}\.\d+),([NS]),(\d+)(\d{2}.\d+),([WE]),(\d+),(\d+),(\d+\.\d+),(\d+\.\d+),M,([+-]?\d+\.\d+),M,([+-]?\d+\.\d+)?,?(\S+)?[*]([A-Z0-9]{2}))");
void NMEAGPS::_parse_gpgga(const std::string& sentence)
{
    /* Parse the GGA message */
    std::smatch m;
    if (!std::regex_search(sentence, m, rex_gga))
        return;

    gps_fix fix;
    fix.valid = true;
    fix.time_utc = m[1];
    int deg = std::stoi(m[2]);
    fix.coordinates.latitude = (deg + std::stof(m[3])/60.0) *
        (m[4] == "N" ? 1.0 : m[4] == "S" ? -1.0 : fix.valid = false );
    deg = std::stoi(m[5]);
    fix.coordinates.longitude = (deg + std::stof(m[6])/60.0) *
        (m[7] == "E" ? 1.0 : m[7] == "W" ? -1.0 : fix.valid = false );
    fix.quality = static_cast<gps_fix_quality>(std::stoi(m[8]));
    fix.satellites = std::stoi(m[9]);
    fix.hdop = std::stof(m[10]);
    fix.altitude_meters = std::stof(m[11]);
    fix.geoid_height_meters = std::stof(m[12]);
    fix.age_seconds = m[13].str().empty() ? 0.0 : std::stof(m[13]);
    fix.station_id = m[14];
    fix.chksum_match = std::stoi(m[15], nullptr, 16) == checksum(sentence);
    fix.valid &= fix.chksum_match;

    /* Throw away oldest if full, push to queue */
    _mtx_fix.lock();
    if (_queue_fix.size() == _maxQueueDepth)
        _queue_fix.pop();
    _queue_fix.push(fix);
    _mtx_fix.unlock();
}

/* Regex for matching NMEA GSV satellite sentences
 * Unfortunately these sentences appear-non standard between the devices tested
 * so it can be expected that these would need updating to match additional
 * devices.
 *
 * Example sentence:
 *
 * $GPGSV,3,3,12,28,75,028,20,30,55,116,28,48,37,194,41,51,35,159,32*7A
 */
static std::regex rex_gsv_hdr(R"(^\$GPGSV,(\d+),(\d+),(\d\d),)");
static std::regex rex_gsv_sat(R"((\d{2}),(\d{2}),(\d{3}),(\d+)?,?)");
static std::regex rex_gsv_ftr("[*]([A-Z0-9]{2})$");
void NMEAGPS::_parse_gpgsv(const std::string& sentence)
{
    /* Parse the GSV header message */
    std::smatch mhdr;
    std::smatch mftr;
    /* No further parsing if this message doesn't match the header
     * or footer or the checksum is bad */
    if (!std::regex_search(sentence, mhdr, rex_gsv_hdr) ||
            !std::regex_search(sentence, mftr, rex_gsv_ftr) ||
            (std::stoi(mftr[1], nullptr, 16) != checksum(sentence)))
        return;

    size_t total_svs = std::stoi(mhdr[3]);

    /* Match each satellite */
    std::sregex_iterator next(sentence.begin(), sentence.end(), rex_gsv_sat);
    std::sregex_iterator end;
    while (next != end)
    {
        std::smatch satmatches = *next++;

        /* Add these satellites.  Only keep a max total_svs satellites at any
         * one time.  The latest are the most current */
        satellite sat = {
            satmatches[1].str(),
            std::stoi(satmatches[2].str()),
            std::stoi(satmatches[3].str()),
            satmatches[4].str().empty() ? 0 :
            std::stoi(satmatches[4].str())
        };

        /* Add to the back of satmap, remove any matching prn */
        _mtx_satlist.lock();
        auto sit = _satlist.begin();
        while(sit != _satlist.end())
        {
            /* Remove */
            if ((*sit).prn == sat.prn)
            {
                sit = _satlist.erase(sit);
                break;
            }
            ++sit;
        }
        /* Add satellite to the end */
        _satlist.push_back(sat);

        /* If more sats exist than current sat count, remove them */
        while (_satlist.size() > total_svs) _satlist.pop_front();
        _mtx_satlist.unlock();
    }
}

/*
 * Regex for matching NMEA GLL coordinates
 * Unfortunately these sentences appear-non standard between the devices tested
 * so it can be expected that these would need updating to match additional
 * devices.
 *
 * For example, the HJ GPS compass returned GLL sentences
 * with a duplicate ,A,A at the end :(
 *      "$GPGLL,4532.55107,N,12257.68422,W,170004.20,A,A*74"
 */
static std::regex rex_gll(R"(^\$GPGLL,(\d+)(\d{2}\.\d+),([NS]),(\d+)(\d{2}.\d+),([WE]),(\d+\.\d+)(,A)?,A[*]([A-Z0-9]{2}))");
void NMEAGPS::_parse_gpgll(const std::string& sentence)
{
    /* Parse the GLL message */
    std::smatch m;
    if (!std::regex_search(sentence, m, rex_gll))
        return;

    gps_fix fix;
    fix.valid = true;
    fix.time_utc = m[7];
    int deg = std::stoi(m[1]);
    fix.coordinates.latitude = (deg + std::stof(m[2])/60.0) *
        (m[3] == "N" ? 1.0 : m[3] == "S" ? -1.0 : fix.valid = false );
    deg = std::stoi(m[4]);
    fix.coordinates.longitude = (deg + std::stof(m[5])/60.0) *
        (m[6] == "E" ? 1.0 : m[6] == "W" ? -1.0 : fix.valid = false );
    fix.chksum_match = std::stoi(m[9], nullptr, 16) == checksum(sentence);
    fix.valid &= fix.chksum_match;

    /* Throw away oldest if full, push to queue */
    _mtx_fix.lock();
    if (_queue_fix.size() == _maxQueueDepth) _queue_fix.pop();
    _queue_fix.push(fix);
    _mtx_fix.unlock();
}

/*
 * Regex for matching NMEA TXT messages
 * Grab-bag of messages coming from a GPS device.  Can basically be any
 * additional information that the manufacture wants to send out.
 *
 * For example, the ublox GPS compass writes out data about itself:
 *      $GPTXT,01,01,02,u-blox ag - www.u-blox.com*50
 *      $GPTXT,01,01,02,HW  UBX-G60xx  00040007 *52
 *      $GPTXT,01,01,02,EXT CORE 7.03 (45970) Mar 17 2011 16:26:24*44
 *      $GPTXT,01,01,02,ROM BASE 6.02 (36023) Oct 15 2009 16:52:08*58
 *      $GPTXT,01,01,02,MOD LEA-6H-0*2D
 *      $GPTXT,01,01,02,ANTSUPERV=AC SD PDoS SR*20
 *      $GPTXT,01,01,02,ANTSTATUS=OK*3B
 */
static std::regex rex_txt(R"(^\$GPTXT,(\d{2}),(\d{2}),(\d{2}),(.*)[*]([A-Z0-9]{2}))");
void NMEAGPS::_parse_gptxt(const std::string& sentence)
{
    /* Parse the GLL message */
    std::smatch m;
    if (!std::regex_search(sentence, m, rex_txt) ||
            (std::stoi(m[5], nullptr, 16) != checksum(sentence)) )
        return;

    /* Throw away oldest if full, push to queue */
    _mtx_txt.lock();
    if (_queue_txt.size() == _maxQueueDepth) _queue_txt.pop();
    _queue_txt.push({std::stoi(m[3]), m[4]});
    _mtx_txt.unlock();
}

void NMEAGPS::parseNMEASentence(const std::string& sentence)
{
    /* Needs to start with $GP... and be (at least 6 characters
     * long to call a parser.  Otherwise skip parsing and put into
     * raw sentence queue for debug */
    size_t msgsz = sentence.size();
    if ((sentence.find("$GP") == 0) &&
            (msgsz >= 5) && (msgsz <=100))
    {
        auto cit = nmea_2_parser.find(sentence.substr(1, 5));
        if (cit != nmea_2_parser.end())
        {
            fp parser = cit->second;
            /* Call the corresponding parser */
            (this->*parser)(sentence);
        }

        /* Keep track of total number of sentences */
        _sentences_since_start++;
    }

    /* Throw away oldest if full, push to raw sentence queue */
    _mtx_nmea_sentence.lock();
    if (_queue_nmea_sentence.size() == _maxQueueDepth)
        _queue_nmea_sentence.pop();
    _queue_nmea_sentence.push(sentence);
    _mtx_nmea_sentence.unlock();
}

void NMEAGPS::_parse_thread()
{
    /* NMEA 0183 max sentence length is 82 characters.  There seems to be
     * varying specs out there.  Using 94 characters between the $GP and
     * the checksum as a max length for a basic max length sanity check.
     *   $GP(94 chars max)*XX length = 100 characters total
     */
    std::regex rex(R"((\$GP.{5,94}\*[a-fA-F0-9][a-fA-F0-9])\r\n)");
    while (_running)
    {
        /* While data is available, read from the GPS.  A 5s
         * timeout appears long, but UARTS can be slow with minimal
         * data getting returned, possible slow UART speeds, and it's
         * better to maximize the UART buffer.  This currently
         * assumes whole sentences are returned each read.
         * TODO: Handle leftover uart data
         */
        if (dataAvailable(5000))
        {
            /* Read a block */
            std::string buf = readStr(4095);

            std::sregex_iterator next(buf.begin(), buf.end(), rex);
            std::sregex_iterator end;
            while (next != end)
            {
                std::smatch matches = *next++;
                parseNMEASentence(matches[1].str());
            }

            /* Let this thread do other stuff */
            upm_delay_us(100);
        }
    }
}

static double getTimeSinceEpoch_s()
{
    auto now = std::chrono::system_clock::now();
    auto now_s = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto epoch = now_s.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    return value.count()/1000.0;
}

void NMEAGPS::parseStart()
{
    /* Don't create multiple running threads */
    if (_running) return;

    /* Set running flag */
    _running = true;

    /* Reset total number of bytes/sentences parsed */
    _sentences_since_start = 0;
    _bytes_since_start = 0;

    /* Reset the total number of seconds */
    _seconds_since_start = getTimeSinceEpoch_s();

    /* Start the thread */
    _parser = std::thread(&NMEAGPS::_parse_thread, this);
}

void NMEAGPS::parseStop()
{
    /* Only stop if running */
    if (!_running) return;

    _running = false;
    if (_parser.joinable())
        _parser.join();

    /* Zero number of bytes/sentences parsed */
    _sentences_since_start = 0;
}

gps_fix NMEAGPS::getFix()
{
    gps_fix x;
    _mtx_fix.lock();
    if (!_queue_fix.empty())
    {
        /* Get a copy of the structure, pop an element */
        x = _queue_fix.front();
        _queue_fix.pop();
    }
    _mtx_fix.unlock();
    return x;
}

std::string NMEAGPS::getRawSentence()
{
    std::string ret;
    _mtx_nmea_sentence.lock();
    if (!_queue_nmea_sentence.empty())
    {
        /* Get a copy of the sentence, pop an element */
        ret = _queue_nmea_sentence.front();
        _queue_nmea_sentence.pop();
    }
    _mtx_nmea_sentence.unlock();
    return ret;
}

nmeatxt NMEAGPS::getTxtMessage()
{
    nmeatxt ret;
    _mtx_txt.lock();
    if (!_queue_txt.empty())
    {
        /* Get a copy of the sentence, pop an element */
        ret = _queue_txt.front();
        _queue_txt.pop();
    }
    _mtx_txt.unlock();
    return ret;
}

size_t NMEAGPS::fixQueueSize()
{
    _mtx_fix.lock();
    size_t x =_queue_fix.size();
    _mtx_fix.unlock();
    return x;
}

size_t NMEAGPS::rawSentenceQueueSize()
{
    _mtx_nmea_sentence.lock();
    size_t x =_queue_nmea_sentence.size();
    _mtx_nmea_sentence.unlock();
    return x;
}

size_t NMEAGPS::txtMessageQueueSize()
{
    _mtx_txt.lock();
    size_t x =_queue_txt.size();
    _mtx_txt.unlock();
    return x;
}

std::string gps_fix::__str__()
{
    std::ostringstream oss;
    oss << "valid:" << (valid ? "T" : "F") << ", ";
    if (time_utc.size() < 6) oss << "UNKNOWN UTC, ";
    else
        oss << time_utc.substr(0, 2) << ":" << time_utc.substr(2,2) << ":"
            << time_utc.substr(4,2) << time_utc.substr(6) << " UTC, ";
    oss << coordinates.latitude << ", " << coordinates.longitude << ", "
        << "quality: " << static_cast<int>(quality) << ", "
        << "sats: " << static_cast<int>(satellites) << ", "
        << "hdop: " << hdop << ", "
        << "alt (m): " << altitude_meters << ", "
        << "geoid_ht (m): " << geoid_height_meters << ", "
        << "age (s): " << age_seconds << ", "
        << "dgps sid: " << station_id << ", "
        << "chksum match: " << (chksum_match ? "T" : "F");
    return oss.str();
}

std::vector<satellite> NMEAGPS::satellites()
{
    /* Create a new set for now */
    _mtx_satlist.lock();
    std::vector<satellite> sats(_satlist.begin(), _satlist.end());
    _mtx_satlist.unlock();

    return sats;
}

std::string satellite::__str__()
{
    std::ostringstream oss;
    oss << "id:" << std::setw(3) << prn << ", "
        << "elevation (d):" << std::setw(3) << elevation_deg
        << ", " << "azimuth (d):" << std::setw(3) << azimuth_deg
        << ", " << "snr:" << std::setw(3) << snr;
    return oss.str();
}

std::string nmeatxt::__str__()
{
    /* Return an emty string */
    if (!severity && message.empty()) return "";

    std::ostringstream oss;
    oss << "[";
    switch (severity)
    {
        case 0:
            oss << "ERROR";
            break;
        case 1:
            oss << "WARNING";
            break;
        case 2:
            oss << "NOTICE";
            break;
        case 7:
            oss << "USER";
            break;
        default:
            oss << "UNKNOWN";
            break;
    }
    oss << "] " << message;

    return oss.str();
}

double NMEAGPS::sentencesPerSecond()
{
    double now_s = getTimeSinceEpoch_s();
    return _sentences_since_start/(now_s - _seconds_since_start);
}

double NMEAGPS::bytesPerSecond()
{
    double now_s = getTimeSinceEpoch_s();
    return _bytes_since_start/(now_s - _seconds_since_start);
}

std::string NMEAGPS::__str__()
{
    std::ostringstream oss;
    std::vector<satellite> sats = satellites();
    size_t msgs = txtMessageQueueSize();
    size_t qsz = getMaxQueueDepth();
    oss << "NMEA GPS Instance" << std::endl
        << "  Parsing: " << (isRunning() ? "T" : "F") << std::endl
        << "  NMEA sentences/second: " << std::fixed << std::setprecision(2)
        << sentencesPerSecond()
        << "  (" << bytesPerSecond() << " bps)" << std::endl
        << "  Available satellites: " << sats.size() << std::endl;
    for(auto sat : sats)
        oss << "    " << sat.__str__() << std::endl;
    oss << "  Queues" << std::endl
        << "    Raw sentence Q: " << std::setw(4) << rawSentenceQueueSize() << "/" << qsz << std::endl
        << "    GPS fix      Q: " << std::setw(4) << fixQueueSize() << "/" << qsz << std::endl
        << "    Messages     Q: " << std::setw(4) << msgs << "/" << qsz;
    if (msgs > 0)
    {
        oss << std::endl << "  Messages" << std::endl;
        for (size_t i = 0; i < msgs; i++)
            oss << "    " << getTxtMessage().__str__() << std::endl;
    }

    return oss.str();
}

std::string coord_DD::__str__()
{
    std::ostringstream oss;
    oss << latitude << ", " << longitude;
    return oss.str();
}
