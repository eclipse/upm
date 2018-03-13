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

/* Currently no need for a custom main (use gtest's)
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/
