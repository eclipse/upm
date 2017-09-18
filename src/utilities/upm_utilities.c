/*
 * Authors:
 *          Jon Trulson <jtrulson@ics.com>
 * Contributions: Rex Tsai <rex.cc.tsai@gmail.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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

#ifndef _POSIX_C_SOURCE
// We need at least 199309L for nanosleep()
# define _POSIX_C_SOURCE 200809L
#endif
#include <time.h>
#include <errno.h>
#include "upm_platform.h"
#include "upm_utilities.h"

// https://www3.epa.gov/airnow/aqi-technical-assistance-document-may2016.pdf
static struct aqi {
    float clow;
    float chigh;
    int llow;
    int lhigh;
} aqi[] = {
  {0.0,    12.0,   0, 50},
  {12.1,   35.4,  51, 100},
  {35.5,   55.4, 101, 150},
  {55.5,  150.4, 151, 200},
  {150.5, 250.4, 201, 300},
  {250.5, 350.4, 301, 400},
  {350.5, 500.4, 401, 500},
};

void upm_delay(unsigned int time)
{
    if (time <= 0)
        time = 1;

#if defined(UPM_PLATFORM_LINUX)

    struct timespec delay_time;

    delay_time.tv_sec  = time;
    delay_time.tv_nsec = 0;
    // The advantage over sleep(3) here is that it will not use
    // an alarm signal or handler.

    // here we spin until the delay is complete - detecting signals
    // and continuing where we left off
    while (nanosleep(&delay_time, &delay_time) && errno == EINTR)
        ; // loop

#elif defined(UPM_PLATFORM_ZEPHYR)
# if KERNEL_VERSION_MAJOR == 1 && KERNEL_VERSION_MINOR >= 6

    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);
    k_timer_start(&timer, time * 1000, 0);
    k_timer_status_sync(&timer);

# else

    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, SECONDS(time) + 1);
    nano_timer_test(&timer, TICKS_UNLIMITED);

# endif

#endif
}

void upm_delay_ms(unsigned int time)
{
    if (time <= 0)
        time = 1;

#if defined(UPM_PLATFORM_LINUX)

    struct timespec delay_time;

    delay_time.tv_sec  = time / 1000;
    delay_time.tv_nsec = (time % 1000) * 1000000;
    // here we spin until the delay is complete - detecting signals
    // and continuing where we left off
    while (nanosleep(&delay_time, &delay_time) && errno == EINTR)
        ; // loop

#elif defined(UPM_PLATFORM_ZEPHYR)
# if KERNEL_VERSION_MAJOR == 1 && KERNEL_VERSION_MINOR >= 6

    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);
    k_timer_start(&timer, time, 0);
    k_timer_status_sync(&timer);

# else

    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, MSEC(time) + 1);
    nano_timer_test(&timer, TICKS_UNLIMITED);

# endif
#endif
}

void upm_delay_us(unsigned int time)
{
    if (time <= 0)
        time = 1;

#if defined(UPM_PLATFORM_LINUX)

    struct timespec delay_time;

    delay_time.tv_sec  = time / 1000000;
    delay_time.tv_nsec = (time % 1000000) * 1000;
    // here we spin until the delay is complete - detecting signals
    // and continuing where we left off
    while (nanosleep(&delay_time, &delay_time) && errno == EINTR)
        ; // loop

#elif defined(UPM_PLATFORM_ZEPHYR)
# if KERNEL_VERSION_MAJOR == 1 && KERNEL_VERSION_MINOR >= 6
    // we will use a upm_clock to do microsecond timings here as k_timer has
    // only a millisecond resolution.  So we init a clock and spin.

    upm_clock_t timer;
    upm_clock_init(&timer);
    while (upm_elapsed_us(&timer) < time)
        ; // spin

# else

    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, USEC(time) + 1);
    nano_timer_test(&timer, TICKS_UNLIMITED);

# endif

#endif
}

void upm_clock_init(upm_clock_t *clock)
{
#if defined(UPM_PLATFORM_LINUX)

    gettimeofday(clock, NULL);

#elif defined(UPM_PLATFORM_ZEPHYR)
    *clock = sys_cycle_get_32();
#endif
}

uint32_t upm_elapsed_ms(upm_clock_t *clock)
{
#if defined(UPM_PLATFORM_LINUX)

    struct timeval elapsed, now;
    uint32_t elapse;

    // get current time
    gettimeofday(&now, NULL);

    struct timeval startTime = *clock;

    // compute the delta since startTime
    if( (elapsed.tv_usec = now.tv_usec - startTime.tv_usec) < 0 )
    {
        elapsed.tv_usec += 1000000;
        elapsed.tv_sec = now.tv_sec - startTime.tv_sec - 1;
    }
    else
    {
        elapsed.tv_sec = now.tv_sec - startTime.tv_sec;
    }

    elapse = (uint32_t)((elapsed.tv_sec * 1000) + (elapsed.tv_usec / 1000));

    // never return 0
    if (elapse == 0)
        elapse = 1;

    return elapse;

#elif defined(UPM_PLATFORM_ZEPHYR)
    uint32_t now = sys_cycle_get_32();

    uint32_t elapsed =
        (uint32_t)(SYS_CLOCK_HW_CYCLES_TO_NS64(now - *clock)/(uint64_t)1000000);

    if (elapsed == 0)
        elapsed = 1;

    return elapsed;
#endif
}

uint32_t upm_elapsed_us(upm_clock_t *clock)
{
#if defined(UPM_PLATFORM_LINUX)

    struct timeval elapsed, now;
    uint32_t elapse;

    // get current time
    gettimeofday(&now, NULL);

    struct timeval startTime = *clock;

    // compute the delta since startTime
    if( (elapsed.tv_usec = now.tv_usec - startTime.tv_usec) < 0 )
    {
        elapsed.tv_usec += 1000000;
        elapsed.tv_sec = now.tv_sec - startTime.tv_sec - 1;
    }
    else
    {
        elapsed.tv_sec = now.tv_sec - startTime.tv_sec;
    }

    elapse = (uint32_t)((elapsed.tv_sec * 1000000) + elapsed.tv_usec);

    // never return 0
    if (elapse == 0)
        elapse = 1;

    return elapse;

#elif defined(UPM_PLATFORM_ZEPHYR)
    uint32_t now = sys_cycle_get_32();

    uint32_t elapsed =
        (uint32_t)(SYS_CLOCK_HW_CYCLES_TO_NS64(now - *clock)/(uint64_t)1000);

    // never return 0
    if (elapsed == 0)
        elapsed = 1;

    return elapsed;
#endif
}

int upm_ugm3_to_aqi (double ugm3)
{
  int i;

  for (i = 0; i < 7; i++) {
    if (ugm3 >= aqi[i].clow &&
        ugm3 <= aqi[i].chigh) {
        // Ip =  [(Ihi-Ilow)/(BPhi-BPlow)] (Cp-BPlow)+Ilow,
        return ((aqi[i].lhigh - aqi[i].llow) / (aqi[i].chigh - aqi[i].clow)) * 
            (ugm3 - aqi[i].clow) + aqi[i].llow;
    }
  }

  return 0;
}
