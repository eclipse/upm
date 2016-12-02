/*
 * Authors:
 *          Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include <upm_platform.h>
#include <upm_utilities.h>

void upm_delay(int time){
#if defined(UPM_PLATFORM_LINUX)
    sleep(time);
#elif defined(UPM_PLATFORM_ZEPHYR)
# if SYS_KERNEL_VER_MAJOR(KERNEL_VERSION_NUMBER) == 1 && \
     SYS_KERNEL_VER_MINOR(KERNEL_VERSION_NUMBER) >= 6

    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);
    k_timer_start(&timer, SECONDS(time) + 1, 0);
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

void upm_delay_ms(int time){
#if defined(UPM_PLATFORM_LINUX)
    usleep(1000 * time);
#elif defined(UPM_PLATFORM_ZEPHYR)
# if SYS_KERNEL_VER_MAJOR(KERNEL_VERSION_NUMBER) == 1 && \
     SYS_KERNEL_VER_MINOR(KERNEL_VERSION_NUMBER) >= 6

    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);
    k_timer_start(&timer, MSEC(time) + 1, 0);
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

void upm_delay_us(int time){
#if defined(UPM_PLATFORM_LINUX)
    usleep(time);
#elif defined(UPM_PLATFORM_ZEPHYR)
# if SYS_KERNEL_VER_MAJOR(KERNEL_VERSION_NUMBER) == 1 && \
     SYS_KERNEL_VER_MINOR(KERNEL_VERSION_NUMBER) >= 6

    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);
    k_timer_start(&timer, USEC(time) + 1, 0);
    k_timer_status_sync(&timer);

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
