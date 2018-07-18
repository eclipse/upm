/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2018 Intel Corporation.
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


#include "gtest/gtest.h"
#include "upm_utilities.h"
#include "upm_utilities.hpp"

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

/* Test the second delay method */
TEST_F(utilities_unit, test_upm_delay)
{
    upm_clock_t clock;
    upm_clock_init(&clock);

    /* Test a corner case */
    upm_delay(0);
    upm_delay(1);

    /* +- check for 1s +/- 1ms */
    ASSERT_NEAR(upm_elapsed_ms(&clock), 1000, 1);
}

/* Test the millisecond delay method */
TEST_F(utilities_unit, test_upm_delay_ms)
{
    upm_clock_t clock;
    upm_clock_init(&clock);

    /* Test a corner case */
    upm_delay_ms(0);
    upm_delay_ms(50);

    /* +- check for 50ms +/- 1ms */
    ASSERT_NEAR(upm_elapsed_ms(&clock), 50, 1);
}

/* Test the microsecond delay method */
TEST_F(utilities_unit, test_upm_delay_us)
{
    upm_clock_t clock;
    upm_clock_init(&clock);

    /* Test a corner case */
    upm_delay_us(0);
    upm_delay_us(1000);

    /* +- check for 1000us +/- 150us */
    ASSERT_NEAR(upm_elapsed_us(&clock), 1000, 150);
}
