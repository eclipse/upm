/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <assert.h>

#include "uartat.h"

#include "upm_utilities.h"

#define UARTAT_MAX_BUFFER (1024)

// milliseconds
#define UARTAT_MAX_WAIT   (1000)

// milliseconds
#define UARTAT_DEFAULT_RESP_DELAY   (250)

static uartat_context _uartat_preinit()
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    uartat_context dev =
        (uartat_context)malloc(sizeof(struct _uartat_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _uartat_context));

    dev->cmd_resp_wait_ms = UARTAT_DEFAULT_RESP_DELAY;

    return dev;
}

static uartat_context _uartat_postinit(uartat_context dev,
                                       unsigned int baudrate)
{
    assert(dev != NULL);

    if (uartat_set_baudrate(dev, baudrate))
    {
        printf("%s: uartat_set_baudrate() failed.\n", __FUNCTION__);
        uartat_close(dev);
        return NULL;
    }

    if (uartat_set_flow_control(dev, UARTAT_FLOW_CONTROL_NONE))
    {
        printf("%s: uartat_set_flow_control() failed.\n", __FUNCTION__);
        uartat_close(dev);
        return NULL;
    }

    return dev;
}

// uart init
uartat_context uartat_init(unsigned int uart, unsigned int baudrate)
{
    uartat_context dev;

    if (!(dev = _uartat_preinit()))
        return NULL;

    // initialize the MRAA context

    // uart, default should be 8N1
    if (!(dev->uart = mraa_uart_init(uart)))
    {
        printf("%s: mraa_uart_init() failed.\n", __FUNCTION__);
        uartat_close(dev);
        return NULL;
    }

    return _uartat_postinit(dev, baudrate);
}

// uart tty init
uartat_context uartat_init_tty(const char *uart_tty, unsigned int baudrate)
{
    uartat_context dev;

    if (!(dev = _uartat_preinit()))
        return NULL;

    // initialize the MRAA context

    // uart, default should be 8N1
    if (!(dev->uart = mraa_uart_init_raw(uart_tty)))
    {
        printf("%s: mraa_uart_init_raw() failed.\n", __FUNCTION__);
        uartat_close(dev);
        return NULL;
    }

    return _uartat_postinit(dev, baudrate);
}

void uartat_close(uartat_context dev)
{
    assert(dev != NULL);

    if (dev->uart)
        mraa_uart_stop(dev->uart);

    free(dev);
}

int uartat_read(const uartat_context dev, char *buffer, size_t len)
{
    assert(dev != NULL);

    // uart
    return mraa_uart_read(dev->uart, buffer, len);
}

int uartat_write(const uartat_context dev, const char *buffer, size_t len)
{
    assert(dev != NULL);

    int rv = mraa_uart_write(dev->uart, buffer, len);
    mraa_uart_flush(dev->uart);

    return rv;
}

bool uartat_data_available(const uartat_context dev, unsigned int millis)
{
    assert(dev != NULL);

    if (mraa_uart_data_available(dev->uart, millis))
        return true;
    else
        return false;
}

upm_result_t uartat_set_baudrate(const uartat_context dev,
                                 unsigned int baudrate)
{
    assert(dev != NULL);

    if (mraa_uart_set_baudrate(dev->uart, baudrate))
    {
        printf("%s: mraa_uart_set_baudrate() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

void uartat_set_response_wait_time(const uartat_context dev,
                                   unsigned int wait_time)
{
    assert(dev != NULL);

    dev->cmd_resp_wait_ms = wait_time;
}

bool uartat_command_mode(const uartat_context dev, const char *cmd_chars,
                         unsigned int guard_ms)
{
    assert(dev != NULL);

    uartat_drain(dev);

    upm_delay_ms(guard_ms);

    uartat_write(dev, cmd_chars, strlen(cmd_chars));

    upm_delay_ms(guard_ms);

    char resp[UARTAT_MAX_BUFFER];
    if (uartat_data_available(dev, UARTAT_MAX_WAIT))
    {
        int rv = uartat_read(dev, resp, UARTAT_MAX_BUFFER);

        if (rv > 0 && (strstr(resp, "OK") || strstr(resp, "0")))
            return true;
    }

    return false;
}

bool uartat_in_command_mode(const uartat_context dev)
{
    assert(dev != NULL);

    static int buflen = 32;
    char buffer[buflen];

    if (uartat_command_with_response(dev, "AT\r", buffer, buflen) <= 0)
        return false;

    // depending on verbosity, you may get "OK" or "0".  Try to catch both.
    if (strstr(buffer, "OK") || strstr(buffer, "0"))
        return true;
    else
        return false;
}

void uartat_drain(const uartat_context dev)
{
    assert(dev != NULL);

    char resp[UARTAT_MAX_BUFFER];
    int rv;
    while (uartat_data_available(dev, 0))
    {
        rv = uartat_read(dev, resp, UARTAT_MAX_BUFFER);
        if (rv < 0)
        {
            printf("%s: read failed\n", __FUNCTION__);
            return;
        }
        // printf("%s: Tossed %d bytes\n", __FUNCTION__, rv);
    }

    return;
}

int uartat_command_with_response(const uartat_context dev,
                                 const char *cmd,
                                 char *resp, size_t resp_len)
{
    assert(dev != NULL);
    assert(cmd != NULL);

    uartat_drain(dev);
    if (uartat_write(dev, cmd, strlen(cmd)) < 0)
    {
        printf("%s: uartat_write failed\n", __FUNCTION__);
        return -1;
    }

    if (resp && resp_len > 1)
    {
        memset(resp, 0, resp_len);

        upm_clock_t clock = upm_clock_init();

        size_t idx = 0;

        do
        {
            if (uartat_data_available(dev, 1))
            {
                int rv = uartat_read(dev, &resp[idx], 1);

                if (rv < 0)
                    return rv;

                if (dev->filter_cr && resp[idx] == '\r')
                    continue;

                if (idx >= resp_len - 1)
                    return idx;

                idx++;
            }
        } while (upm_elapsed_ms(&clock) < dev->cmd_resp_wait_ms);

        return idx;
    }
    else
    {
        upm_delay_ms(dev->cmd_resp_wait_ms);
        uartat_drain(dev);
    }

    return 0;
}

bool uartat_command_waitfor(const uartat_context dev, const char *cmd,
                            char *resp, size_t resp_len,
                            const char *wait_string,
                            unsigned int millis)
{
    assert(dev != NULL);
    assert(cmd != NULL);
    assert(resp != NULL);
    assert(resp_len > 0);
    assert(wait_string != NULL);

    uartat_drain(dev);
    if (uartat_write(dev, cmd, strlen(cmd)) < 0)
    {
        printf("%s: uartat_write failed\n", __FUNCTION__);
        return -1;
    }

    memset(resp, 0, resp_len);

    upm_clock_t clock = upm_clock_init();

    size_t idx = 0;

    do
    {
        if (uartat_data_available(dev, 1))
        {
            int rv = uartat_read(dev, &resp[idx], 1);

            if (rv < 0)
                return false;

            if (dev->filter_cr && resp[idx] == '\r')
                continue;

            if (idx >= resp_len - 1)
            {
                // one last check
                if (uartat_find(dev, resp, wait_string))
                    return true;
                else
                    return false;
            }
            idx++;

            // see if the string is present
            if (uartat_find(dev, resp, wait_string))
                return true;
        }
    } while (upm_elapsed_ms(&clock) < millis);

    return false;
}

void uartat_command(const uartat_context dev, const char *cmd)
{
    assert(dev != NULL);
    assert(cmd != NULL);

    uartat_command_with_response(dev, cmd, NULL, 0);
}

upm_result_t uartat_set_flow_control(const uartat_context dev,
                                     UARTAT_FLOW_CONTROL_T fc)
{
    assert(dev != NULL);

    mraa_result_t rv = MRAA_SUCCESS;

    switch(fc)
    {
    case UARTAT_FLOW_CONTROL_NONE:
        rv = mraa_uart_set_flowcontrol(dev->uart, false, false);
        break;

    case UARTAT_FLOW_CONTROL_HARD:
        rv = mraa_uart_set_flowcontrol(dev->uart, false, true);
        break;

    case UARTAT_FLOW_CONTROL_SOFT:
        rv = mraa_uart_set_flowcontrol(dev->uart, true, false);
        break;

    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    if (rv == MRAA_SUCCESS)
        return UPM_SUCCESS;
    else
        return UPM_ERROR_OPERATION_FAILED;
}

bool uartat_find(const uartat_context dev, const char *buffer, const char *str)
{
    assert(dev != NULL);
    assert(buffer != NULL);
    assert(str != NULL);

    return ((strstr(buffer, str)) ? true : false);
}

void uartat_filter_cr(const uartat_context dev, bool enable)
{
    assert(dev != NULL);

    dev->filter_cr = enable;
}
