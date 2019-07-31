/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2018 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "gtest/gtest.h"
#include "nmea_gps.hpp"
#include "mraa.hpp"

#include <algorithm>
#include <random>

/* NMEA GPS test fixture */
class nmea_gps_unit : public ::testing::Test
{
    protected:
        /* One-time setup logic if needed */
        nmea_gps_unit() = default;

        /* One-time tear-down logic if needed */
        ~nmea_gps_unit() override = default;

        /* Fail if not MOCK platform */
        void SetUp() override
        {
            ASSERT_EQ(mraa::getPlatformType(), mraa::MOCK_PLATFORM) <<
                "NMEA_GPS test requires mraa compiled with  mraa::MOCK_PLATFORM";
        }

        /* Per-test tear-down logic if needed */
        void TearDown() override {}
};

/* Basic tests */
TEST_F(nmea_gps_unit, DOA)
{
    upm::NMEAGPS gps(0, 115200, -1);

    /* Min queue size is 1 */
    gps.setMaxQueueDepth(0);
    ASSERT_EQ(gps.getMaxQueueDepth(), 1);

    /* Max queue size is 1000 */
    gps.setMaxQueueDepth(1001);
    ASSERT_EQ(gps.getMaxQueueDepth(), 1000);

    /* Queues should be empty */
    ASSERT_EQ(gps.fixQueueSize(), 0);
    ASSERT_EQ(gps.rawSentenceQueueSize(), 0);
}

/* Check parsing varying length sentences */
TEST_F(nmea_gps_unit, parse_max_size)
{
    upm::NMEAGPS gps(0, 115200, -1);

    /* Parse an sentence that is too long to be NMEA */
    gps.parseNMEASentence("$GPGSV,2,1,08,07,64,079,,08,39,066,,09,25,159,,09,25,159,,09,25,159,,09,25,159,,09,25,159,,09,25,159,,09,25,159,,07,64,079,*73");
    ASSERT_EQ(gps.rawSentenceQueueSize(), 1);
    ASSERT_EQ(gps.satellites().size(), 0);
}

/* Basic test */
TEST_F(nmea_gps_unit, parse_basic)
{
    upm::NMEAGPS gps(0, 115200, -1);

    /* Parse an invalid sentence */
    gps.parseNMEASentence("$GPGGA,182333.50,,,,,0,00,99.99,,,,,,*6B");
}

/* Parse an invalid sentence */
TEST_F(nmea_gps_unit, parse_gps_fix_invalid)
{
    upm::NMEAGPS gps(0, 115200, -1);

    /* Parse an invalid sentence */
    gps.parseNMEASentence("$GPGGA,182333.50,,,,,0,00,99.99,,,,,,*6B");

    /* Should be 1 entry in the raw queue */
    ASSERT_EQ(gps.rawSentenceQueueSize(), 1);
    /* Should be 0 in GPS fix queue */
    ASSERT_EQ(gps.fixQueueSize(), 0);

    /* Get the GPS fix */
    upm::gps_fix f = gps.getFix();
    ASSERT_EQ(f.valid, false);
    ASSERT_EQ(f.quality, upm::gps_fix_quality::no_fix);
    /* Call the string method for coverage */
    f.__str__();

    /* Get the 1 raw sentence */
    gps.getRawSentence();

    /* Should be 0 entries in each queue */
    ASSERT_EQ(gps.rawSentenceQueueSize(), 0);
    ASSERT_EQ(gps.fixQueueSize(), 0);
}

std::string randstr(size_t size)
{
    std::string retstr(".", size);

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<char> distr(32, 126);
    for (size_t i = 0; i < size; i++)
        retstr[i] = distr(eng);
    return retstr;
}

/* Parse bogus sentences */
TEST_F(nmea_gps_unit, parse_gps_fix_bogus)
{
    upm::NMEAGPS gps(0, 115200, -1);

    /* Parse some bogus sentences */
    std::string gga = "$GPGGA,182333.50,,,,,0,00,99.99,,,,,,*6B";
    gps.parseNMEASentence(gga);

    for (size_t i = 0; i < gga.size(); i++)
    {
        std::string tmp = gga;
        gps.parseNMEASentence(tmp.replace(i, 1, "x"));
    }

    for (int i = 0; i < 1000; i++)
        gps.parseNMEASentence(randstr(40));

    /* Still no GPS fix */
    ASSERT_EQ(gps.fixQueueSize(), 0);
}

/* Parse valid gga sentences */
TEST_F(nmea_gps_unit, parse_gps_fix_valid)
{
    upm::NMEAGPS gps(0, 115200, -1);

    /* Parse a valid sentence */
    gps.parseNMEASentence("$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,"
            + std::string("2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F"));

    /* Should be 1 entry in GPS fix queue */
    ASSERT_EQ(gps.fixQueueSize(), 1);

    upm::gps_fix f = gps.getFix();
    ASSERT_EQ(f.valid, true) << f.__str__();
}

/* Parse valid gsv sentences */
TEST_F(nmea_gps_unit, parse_gsv_valid)
{
    upm::NMEAGPS gps(0, 115200, -1);
    std::vector<std::string> snts =
    {
        "$GPGSV,2,1,08,07,64,079,,08,39,066,,09,25,159,,11,15,117,*7B",
        "$GPGSV,2,2,08,13,25,313,,30,78,336,22,48,37,194,,51,35,158,*75",
        "$GPGSV,2,2,08,13,25,313,,30,78,336,21,48,37,194,,51,35,158,*76",
        "$GPGSV,2,2,08,13,25,313,,30,78,336,,48,37,194,,51,35,158,*75",
        "$GPGSV,2,1,08,07,64,079,,08,39,066,,09,25,159,,11,15,117,*7B",
        "$GPGSV,2,2,08,13,25,313,,30,78,336,21,48,37,194,,51,35,158,*76",
        "$GPGSV,2,2,08,13,25,313,,30,78,336,20,48,37,194,,51,35,158,*77",
        "$GPGSV,2,1,08,07,64,079,,08,39,066,,09,25,159,,11,15,117,*7B",
        "$GPGSV,2,2,08,13,25,313,,30,78,336,18,48,37,194,,51,35,158,*7C",
        "$GPGSV,2,2,08,13,25,313,,30,78,336,17,48,37,194,,51,35,158,*73",
        "$GPGSV,2,1,08,07,64,080,,08,39,066,,09,25,159,,11,15,117,*7D",
        "$GPGSV,2,2,08,13,26,313,,30,78,336,13,48,37,194,,51,35,158,*74",
        "$GPGSV,2,1,08,07,64,080,,08,39,066,,09,25,159,,11,15,117,*7D",
        "$GPGSV,2,2,08,13,26,313,,30,78,336,09,48,37,194,,51,35,158,*7F",
        "$GPGSV,2,1,08,07,64,080,,08,39,066,,09,25,160,,11,15,117,*77",
        "$GPGSV,2,2,08,13,26,313,,30,78,336,23,48,37,194,,51,35,158,*77",
        "$GPGSV,3,3,09,51,35,158,*4E",
        "$GPGSV,2,1,08,07,64,080,22,08,39,066,,09,25,160,,11,15,117,*77",
        "$GPGSV,2,1,08,07,64,080,21,08,39,066,,09,25,160,,11,15,117,*74"
    };

    /* Parse the first sentence */
    gps.parseNMEASentence(snts.front());

    /* Should have 4 satellites */
    auto sats = gps.satellites();
    ASSERT_EQ(sats.size(), 4);

    ASSERT_EQ(sats[0].prn, "07");
    ASSERT_EQ(sats[0].elevation_deg, 64);
    ASSERT_EQ(sats[0].azimuth_deg, 79);
    ASSERT_EQ(sats[0].snr, 0);

    ASSERT_EQ(sats[1].prn, "08");
    ASSERT_EQ(sats[1].elevation_deg, 39);
    ASSERT_EQ(sats[1].azimuth_deg, 66);
    ASSERT_EQ(sats[1].snr, 0);

    ASSERT_EQ(sats[2].prn, "09");
    ASSERT_EQ(sats[2].elevation_deg, 25);
    ASSERT_EQ(sats[2].azimuth_deg, 159);
    ASSERT_EQ(sats[2].snr, 0);

    ASSERT_EQ(sats[3].prn, "11");
    ASSERT_EQ(sats[3].elevation_deg, 15);
    ASSERT_EQ(sats[3].azimuth_deg, 117);
    ASSERT_EQ(sats[3].snr, 0);

    /* Parse the rest */
    for(const auto& sentence : snts)
        gps.parseNMEASentence(sentence);

    /* Finish up with 8 satellites */
    sats = gps.satellites();
    ASSERT_EQ(sats.size(), 8);

    /* Verify the last satellite */
    ASSERT_EQ(sats.back().prn, "11");
    ASSERT_EQ(sats.back().elevation_deg, 15);
    ASSERT_EQ(sats.back().azimuth_deg, 117);
    ASSERT_EQ(sats.back().snr, 0);

    /* The 4th should have a non-zero snr */
    ASSERT_EQ(sats[4].snr, 21);
}

/* Parse valid gll sentences */
TEST_F(nmea_gps_unit, parse_gll_valid)
{
    upm::NMEAGPS gps(0, 115200, -1);
    std::vector<std::string> snts =
    {
      "$GPGLL,4532.55107,N,12257.68422,W,170004.20,A,A*74",
      "$GPGLL,4532.55008,N,12257.68195,W,170005.00,A,A*70",
      "$GPGLL,4532.55027,N,12257.68252,W,170006.10,A,A*77",
      "$GPGLL,4532.54370,N,12257.65873,W,170006.90,A,A*7B",
      "$GPGLL,4532.54230,N,12257.65302,W,170008.00,A,A*74"
    };

    /* Parse the first sentence */
    gps.parseNMEASentence(snts.front());

    /* Get the fix */
    upm::gps_fix f = gps.getFix();
    ASSERT_EQ(f.valid, true) << f.__str__();
    ASSERT_FLOAT_EQ(f.coordinates.latitude, 45.542517833333335) << f.__str__();
    ASSERT_FLOAT_EQ(f.coordinates.longitude, -122.96140366666667) << f.__str__();
    ASSERT_EQ(f.time_utc, "170004.20") << f.__str__();

    /* Parse the rest */
    for(const auto& sentence : snts)
        gps.parseNMEASentence(sentence);

    /* Should have 5 GPS fixes */
    ASSERT_EQ(gps.fixQueueSize(), 5);
}

/* Parse example txt sentences */
TEST_F(nmea_gps_unit, parse_txt_valid)
{
    upm::NMEAGPS gps(0, 115200, -1);
    std::vector<std::string> snts =
    {
       "$GPTXT,01,01,02,u-blox ag - www.u-blox.com*50",
       "$GPTXT,01,01,02,HW  UBX-G60xx  00040007 *52",
       "$GPTXT,01,01,02,EXT CORE 7.03 (45970) Mar 17 2011 16:26:24*44",
       "$GPTXT,01,01,02,ROM BASE 6.02 (36023) Oct 15 2009 16:52:08*58",
       "$GPTXT,01,01,02,MOD LEA-6H-0*2D",
       "$GPTXT,01,01,02,ANTSUPERV=AC SD PDoS SR*20",
       "$GPTXT,01,01,02,ANTSTATUS=OK*3B"
    };

    /* Parse the first sentence */
    gps.parseNMEASentence(snts.front());

    ASSERT_EQ(gps.txtMessageQueueSize(), 1);
    /* Get the message */
    upm::nmeatxt msg = gps.getTxtMessage();
    ASSERT_EQ(msg.severity, 2);

    /* Parse the rest */
    for(const auto& sentence : snts)
        gps.parseNMEASentence(sentence);

    /* Should have 5 GPS fixes */
    ASSERT_EQ(gps.txtMessageQueueSize(), 7);
}
