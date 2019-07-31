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

#include <chrono>
#include <thread>

#include "gtest/gtest.h"
#include "upm_utilities.h"
#include "upm_utilities.hpp"

/* Average over AVG_CNT iterations */
#define AVG_CNT 5

/* Specify a delay for all tests under 1s */
#define ms_50 std::chrono::milliseconds(50)

/* Specify a +/- value for all tests under 1s.  Since the delay methods in
 * non-realtime operating systems can vary greatly, use a lenient value for
 * testing these methods. */
#define time_range std::chrono::milliseconds(5)

/* Helper defines */
#define to_ms std::chrono::duration_cast<std::chrono::milliseconds>
#define to_us std::chrono::duration_cast<std::chrono::microseconds>
#define to_ns std::chrono::duration_cast<std::chrono::nanoseconds>

/* Utilities test fixture */
class utilities_unit : public ::testing::Test
{
    protected:
        /* One-time setup logic if needed */
        utilities_unit() {}

        /* One-time tear-down logic if needed */
        virtual ~utilities_unit() {}

        /* Per-test setup logic if needed */
        virtual void SetUp() {}

        /* Per-test tear-down logic if needed */
        virtual void TearDown() {}
};

/* Sanity check on min_delay_ns */
TEST_F(utilities_unit, min_delay_LT_500us)
{
    /* Determine a rough-average for the minimum delay using chrono */
    std::chrono::nanoseconds min_delay_ns = std::chrono::nanoseconds::zero();
    for (int i = 0; i < AVG_CNT; i++)
    {
        auto start = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        auto end = std::chrono::steady_clock::now();
        min_delay_ns += to_ns(end-start);
    }
    min_delay_ns /= AVG_CNT;

    ASSERT_LT(to_us(min_delay_ns).count(), 3000);
}

/* Test the second delay method */
TEST_F(utilities_unit, test_upm_delay)
{
    upm_clock_t clock = upm_clock_init();

    /* Test a corner case */
    upm_delay(0);

    /* +- check for 0s */
    EXPECT_EQ(upm_elapsed_ms(&clock), 0);

    clock = upm_clock_init();
    upm_delay(1);

    /* +- check near 1s */
    EXPECT_NEAR(upm_elapsed_ms(&clock), 1000, time_range.count());
}

/* Test the millisecond delay method */
TEST_F(utilities_unit, test_upm_delay_ms)
{
    upm_clock_t clock = upm_clock_init();

    /* Test a corner case */
    upm_delay_ms(0);

    /* +- check for 0ms */
    EXPECT_EQ(upm_elapsed_ms(&clock), 0);

    clock = upm_clock_init();
    upm_delay_ms(ms_50.count() * AVG_CNT);

    /* +- check near 50ms */
    EXPECT_NEAR(upm_elapsed_ms(&clock)/AVG_CNT, ms_50.count(), time_range.count());
}

/* Test the microsecond delay method */
TEST_F(utilities_unit, test_upm_delay_us)
{
    upm_clock_t clock = upm_clock_init();

    /* Test a corner case */
    upm_delay_us(0);

    /* +- check for 0us +/- 100us */
    EXPECT_NEAR(upm_elapsed_us(&clock), 0, 100);

    clock = upm_clock_init();
    upm_delay_us(to_us(ms_50).count() * AVG_CNT);

    /* +- check near 50ms */
    EXPECT_NEAR(upm_elapsed_us(&clock)/AVG_CNT, to_us(ms_50).count(),
            to_us(time_range).count());
}

/* Test the nanosecond delay method */
TEST_F(utilities_unit, test_upm_delay_ns)
{
    upm_clock_t clock = upm_clock_init();

    /* Test a corner case */
    upm_delay_ns(0);

    /* +- check for 0us +/- 100us */
    EXPECT_NEAR(upm_elapsed_ns(&clock), 0, 100000);

    clock = upm_clock_init();
    upm_delay_ns(to_ns(ms_50).count() * AVG_CNT);

    /* +- check near 50ms */
    EXPECT_NEAR(upm_elapsed_ns(&clock)/AVG_CNT, to_ns(ms_50).count(),
            to_ns(time_range).count());
}

/* Test the max us delay (default to disabled) */
TEST_F(utilities_unit, DISABLED_test_upm_delay_us_max)
{
    upm_clock_t clock = upm_clock_init();
    upm_delay_us(4294967295);
    EXPECT_NEAR(upm_elapsed_us(&clock), 4294967295, 150);
}

/* Test the Air Quality Index method */
TEST_F(utilities_unit, test_upm_ugm3_to_aqi)
{
    EXPECT_EQ(upm_ugm3_to_aqi(10), 41);
}
