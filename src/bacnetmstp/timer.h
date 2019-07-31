/**************************************************************************
*
* Copyright (C) 2009 Steve Karg <skarg@users.sourceforge.net>
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/
#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>   /* for timeval */

/* Timer Module */
#ifndef MAX_MILLISECOND_TIMERS
#define TIMER_SILENCE 0
#define MAX_MILLISECOND_TIMERS 1
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    uint32_t timeGetTime(
        void);

    void timer_init(
        void);
    uint32_t timer_milliseconds(
        unsigned index);
    bool timer_elapsed_milliseconds(
        unsigned index,
        uint32_t value);
    bool timer_elapsed_seconds(
        unsigned index,
        uint32_t value);
    bool timer_elapsed_minutes(
        unsigned index,
        uint32_t seconds);
    uint32_t timer_milliseconds_set(
        unsigned index,
        uint32_t value);
    uint32_t timer_reset(
        unsigned index);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
