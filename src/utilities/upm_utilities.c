/*
 * Authors:
 *          Jon Trulson <jtrulson@ics.com>
 * Contributions: Rex Tsai <rex.cc.tsai@gmail.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _POSIX_C_SOURCE
// We need at least 199309L for clock_nanosleep()
# define _POSIX_C_SOURCE 200809L
#endif
#include <assert.h>
#include <time.h>
#include <errno.h>
#include "upm_platform.h"
#include "upm_utilities.h"


/**
 * Calculate the delta of two upm_clock_t values as
 *      delta = finish - start
 *
 * @param finish Ending upm_clock_t time
 * @param start Beginning upm_clock_t time
 * @return Time in nanoseconds
 */
static uint64_t _delta_ns(const upm_clock_t* finish, const upm_clock_t* start)
{
    uint64_t delta;
    assert((finish != NULL) && (start != NULL) && "_delta_ns, arguments cannot be NULL");

#if defined(UPM_PLATFORM_ZEPHYR)
    delta = SYS_CLOCK_HW_CYCLES_TO_NS64(*finish - *start);
#elif defined(UPM_PLATFORM_LINUX)

    delta = (finish->tv_sec * 1000000000UL + finish->tv_nsec) -
        (start->tv_sec * 1000000000UL + start->tv_nsec);
#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif
    return delta;
}

void upm_delay(uint32_t time)
{
    /* Return if time == 0 */
    if (!time)
        return;

#if defined(UPM_PLATFORM_LINUX)

    upm_clock_t delay_time = {time, 0};

    // here we spin until the delay is complete - detecting signals
    // and continuing where we left off
    while (clock_nanosleep(CLOCK_MONOTONIC, 0, &delay_time, &delay_time) == EINTR);

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

#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif
}

void upm_delay_ms(uint32_t time)
{
    /* Return if time == 0 */
    if (!time)
        return;

#if defined(UPM_PLATFORM_LINUX)

    upm_clock_t delay_time = {time / 1000, (time % 1000) * 1000000UL};

    // here we spin until the delay is complete - detecting signals
    // and continuing where we left off
    while (clock_nanosleep(CLOCK_MONOTONIC, 0, &delay_time, &delay_time) == EINTR);

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
#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif
}

void upm_delay_us(uint32_t time)
{
    /* Return if time == 0 */
    if (!time)
        return;

#if defined(UPM_PLATFORM_LINUX)

    upm_clock_t delay_time = {time / 1000000, (time % 1000000) * 1000};

    // here we spin until the delay is complete - detecting signals
    // and continuing where we left off
    while (clock_nanosleep(CLOCK_MONOTONIC, 0, &delay_time, &delay_time) == EINTR);

#elif defined(UPM_PLATFORM_ZEPHYR)
# if KERNEL_VERSION_MAJOR == 1 && KERNEL_VERSION_MINOR >= 6
    // we will use a upm_clock to do microsecond timings here as k_timer has
    // only a millisecond resolution.  So we init a clock and spin.

    upm_clock_t timer = upm_clock_init();
    while (upm_elapsed_us(&timer) < time); // spin

# else

    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, USEC(time) + 1);
    nano_timer_test(&timer, TICKS_UNLIMITED);

# endif

#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif
}

void upm_delay_ns(uint64_t time)
{
    /* Return if time == 0 */
    if (!time)
        return;

#if defined(UPM_PLATFORM_LINUX)

    upm_clock_t delay_time = {time / 1000000000UL, time % 1000000000UL};

    // here we spin until the delay is complete - detecting signals
    // and continuing where we left off
    while (clock_nanosleep(CLOCK_MONOTONIC, 0, &delay_time, &delay_time) == EINTR);

#elif defined(UPM_PLATFORM_ZEPHYR)
# if KERNEL_VERSION_MAJOR == 1 && KERNEL_VERSION_MINOR >= 6
    // we will use a upm_clock to do microsecond timings here as k_timer has
    // only a millisecond resolution.  So we init a clock and spin.

    upm_clock_t timer = upm_clock_init();
    while (upm_elapsed_ns(&timer) < time); // spin

# else

    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, time + 1);
    nano_timer_test(&timer, TICKS_UNLIMITED);

# endif

#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif
}

upm_clock_t upm_clock_init(void)
{
    upm_clock_t clock = {0};
#if defined(UPM_PLATFORM_LINUX)
    clock_gettime(CLOCK_MONOTONIC, &clock);
#elif defined(UPM_PLATFORM_ZEPHYR)
    clock = sys_cycle_get_32();
#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif

    return clock;
}

uint64_t upm_elapsed_ms(const upm_clock_t *clock)
{
    assert((clock != NULL) && "upm_elapsed_ms, clock cannot be NULL");

    upm_clock_t now = {0};

#if defined(UPM_PLATFORM_LINUX)
    clock_gettime(CLOCK_MONOTONIC, &now);
#elif defined(UPM_PLATFORM_ZEPHYR)
    now = sys_cycle_get_32();
#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif

    return _delta_ns(&now, clock)/1000000;
}

uint64_t upm_elapsed_us(const upm_clock_t *clock)
{
    assert((clock != NULL) && "upm_elapsed_us, clock cannot be NULL");

    upm_clock_t now = {0};

#if defined(UPM_PLATFORM_LINUX)
    clock_gettime(CLOCK_MONOTONIC, &now);
#elif defined(UPM_PLATFORM_ZEPHYR)
    now = sys_cycle_get_32();
#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif

    return _delta_ns(&now, clock)/1000;
}

uint64_t upm_elapsed_ns(const upm_clock_t *clock)
{
    assert((clock != NULL) && "upm_elapsed_ns, clock cannot be NULL");

    upm_clock_t now = {0};

#if defined(UPM_PLATFORM_LINUX)
    clock_gettime(CLOCK_MONOTONIC, &now);
#elif defined(UPM_PLATFORM_ZEPHYR)
    now = sys_cycle_get_32();
#else
#error "Unknown platform, valid platforms are {UPM_PLATFORM_ZEPHYR, UPM_PLATFORM_LINUX}"
#endif

    return _delta_ns(&now, clock);
}

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
