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
#include <ctype.h>
#include <assert.h>

#include "rn2903.h"

#include "upm_utilities.h"
#include "upm_platform.h"

// we use small buffers of this size to build certain compound
// commands
#define RN2903_CMD_BUFFER_32B       (32) // 32 bytes

// maximum number of autobaud retries
#define RN2903_AUTOBAUD_RETRIES     (10)

// some useful macros to save on typing and text wrapping
#undef _SHIFT
#define _SHIFT(x) (_RN2903_##x##_SHIFT)

#undef _MASK
#define _MASK(x) (_RN2903_##x##_MASK)

#undef _SHIFTMASK
#define _SHIFTMASK(x) (_MASK(x) << _SHIFT(x))

// disable printf to stdout if on Zephyr, and stdout isn't available.
#if defined(UPM_PLATFORM_ZEPHYR) && !defined(CONFIG_STDOUT_CONSOLE)
# define printf printk
#endif


static bool validate_hex_str(const char *hex)
{
    assert(hex != NULL);

    int len = strlen(hex);
    if ((len % 2) != 0)
    {
        printf("%s: strlen(hex) must be a multiple of 2\n",
               __FUNCTION__);
        return false;
    }

    for (int i=0; i<len; i++)
    {
        if ( !((hex[i] >= '0' && hex[i] <= '9') ||
               (tolower(hex[i]) >= 'a' && tolower(hex[i]) <= 'f')) )
        {
            printf("%s: invalid hex character at position %d\n",
                   __FUNCTION__, i);
            return false;
        }
    }

    return true;
}

static rn2903_context _rn2903_preinit()
{
    // make sure MRAA is initialized
    int mraa_rv;
    if ((mraa_rv = mraa_init()) != MRAA_SUCCESS)
    {
        printf("%s: mraa_init() failed (%d).\n", __FUNCTION__, mraa_rv);
        return NULL;
    }

    rn2903_context dev =
        (rn2903_context)malloc(sizeof(struct _rn2903_context));

    if (!dev)
        return NULL;

    // zero out context
    memset((void *)dev, 0, sizeof(struct _rn2903_context));

    // first response wait time
    dev->cmd_resp_wait_ms = RN2903_DEFAULT_RESP_DELAY;
    // optional second response wait time
    dev->cmd_resp2_wait_ms = RN2903_DEFAULT_RESP2_DELAY;

    // init stored baudrate to RN2903_DEFAULT_BAUDRATE
    dev->baudrate = RN2903_DEFAULT_BAUDRATE;

    // uncomment for "early" debugging
    // dev->debug = true;

    return dev;
}

static rn2903_context _rn2903_postinit(rn2903_context dev,
                                       unsigned int baudrate)
{
    assert(dev != NULL);

    if (rn2903_set_baudrate(dev, baudrate))
    {
        printf("%s: rn2903_set_baudrate() failed.\n", __FUNCTION__);
        rn2903_close(dev);
        return NULL;
    }

    if (rn2903_set_flow_control(dev, RN2903_FLOW_CONTROL_NONE))
    {
        printf("%s: rn2903_set_flow_control() failed.\n", __FUNCTION__);
        rn2903_close(dev);
        return NULL;
    }

    // turn off debugging
    rn2903_set_debug(dev, false);

    // reset the device
    if (rn2903_reset(dev))
    {
        printf("%s: rn2903_reset() failed.\n", __FUNCTION__);
        rn2903_close(dev);
        return NULL;
    }

    // now get and store our hardware EUI
    if (rn2903_command(dev, "sys get hweui"))
    {
        printf("%s: rn2903_command(sys get hweui) failed.\n", __FUNCTION__);
        rn2903_close(dev);
        return NULL;
    }
    memcpy(dev->hardware_eui, dev->resp_data, RN2903_MAX_HEX_EUI64);

    return dev;
}

// uart init
rn2903_context rn2903_init(unsigned int uart, unsigned int baudrate)
{
    rn2903_context dev;

    if (!(dev = _rn2903_preinit()))
        return NULL;

    // initialize the MRAA context

    // uart, default should be 8N1
    if (!(dev->uart = mraa_uart_init(uart)))
    {
        printf("%s: mraa_uart_init() failed.\n", __FUNCTION__);
        rn2903_close(dev);
        return NULL;
    }

    return _rn2903_postinit(dev, baudrate);
}

// uart tty init
rn2903_context rn2903_init_tty(const char *uart_tty, unsigned int baudrate)
{
    rn2903_context dev;

    if (!(dev = _rn2903_preinit()))
        return NULL;

    // initialize the MRAA context

    // uart, default should be 8N1
    if (!(dev->uart = mraa_uart_init_raw(uart_tty)))
    {
        printf("%s: mraa_uart_init_raw() failed.\n", __FUNCTION__);
        rn2903_close(dev);
        return NULL;
    }

    return _rn2903_postinit(dev, baudrate);
}

void rn2903_close(rn2903_context dev)
{
    assert(dev != NULL);

    if (dev->to_hex_buf)
        free(dev->to_hex_buf);
    if (dev->from_hex_buf)
        free(dev->from_hex_buf);

    if (dev->uart)
        mraa_uart_stop(dev->uart);

    free(dev);
}

int rn2903_read(const rn2903_context dev, char *buffer, size_t len)
{
    assert(dev != NULL);

    // uart
    return mraa_uart_read(dev->uart, buffer, len);
}

int rn2903_write(const rn2903_context dev, const char *buffer, size_t len)
{
    assert(dev != NULL);

    int rv = mraa_uart_write(dev->uart, buffer, len);
    mraa_uart_flush(dev->uart);

    return rv;
}

bool rn2903_data_available(const rn2903_context dev, unsigned int millis)
{
    assert(dev != NULL);

    if (mraa_uart_data_available(dev->uart, millis))
        return true;
    else
        return false;
}

upm_result_t rn2903_set_baudrate(const rn2903_context dev,
                                 unsigned int baudrate)
{
    assert(dev != NULL);

    if (dev->debug)
        printf("%s: Setting baudrate to %d\n", __FUNCTION__,
               baudrate);

    if (mraa_uart_set_baudrate(dev->uart, baudrate))
    {
        printf("%s: mraa_uart_set_baudrate() failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    dev->baudrate = baudrate;

    if (!rn2903_autobaud(dev, RN2903_AUTOBAUD_RETRIES))
    {
        printf("%s: rn2903_autobaud detection failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

void rn2903_set_debug(const rn2903_context dev, bool enable)
{
    assert(dev != NULL);

    dev->debug = enable;
}

void rn2903_set_response_wait_time(const rn2903_context dev,
                                   unsigned int wait_time)
{
    assert(dev != NULL);

    dev->cmd_resp_wait_ms = wait_time;
}

void rn2903_set_response2_wait_time(const rn2903_context dev,
                                    unsigned int wait_time)
{
    assert(dev != NULL);

    dev->cmd_resp2_wait_ms = wait_time;
}

void rn2903_drain(const rn2903_context dev)
{
    assert(dev != NULL);

    char resp[RN2903_MAX_BUFFER];
    int rv;
    while (rn2903_data_available(dev, 0))
    {
        rv = rn2903_read(dev, resp, RN2903_MAX_BUFFER);
        if (rv < 0)
        {
            printf("%s: read failed\n", __FUNCTION__);
            return;
        }
        // printf("%s: Tossed %d bytes\n", __FUNCTION__, rv);
    }

    return;
}

RN2903_RESPONSE_T rn2903_waitfor_response(const rn2903_context dev,
                                          int wait_ms)
{
    assert(dev != NULL);

    memset(dev->resp_data, 0, RN2903_MAX_BUFFER);
    dev->resp_len = 0;

    upm_clock_t clock = upm_clock_init();
    uint32_t elapsed = 0;

    do
    {
        if (rn2903_data_available(dev, 1))
        {
            int rv = rn2903_read(dev, &(dev->resp_data[dev->resp_len]), 1);

            if (rv < 0)
                return RN2903_RESPONSE_UPM_ERROR;

            // discard CR's
            if (dev->resp_data[dev->resp_len] == '\r')
                continue;

            // got a LF, we are done - discard and finish
            if (dev->resp_data[dev->resp_len] == '\n')
            {
                dev->resp_data[dev->resp_len] = 0;
                break;
            }

            // too much data?
            if (dev->resp_len >= RN2903_MAX_BUFFER - 1)
                break;

            dev->resp_len++;
        }
    } while ( (int)(elapsed = upm_elapsed_ms(&clock)) < wait_ms);

    if (dev->debug)
        printf("\tRESP (%d): '%s'\n", (int)dev->resp_len,
               (dev->resp_len) ? dev->resp_data : "");

    // check for and return obvious errors
    if ((int)elapsed >= wait_ms)
        return RN2903_RESPONSE_TIMEOUT;
    else if (rn2903_find(dev, RN2903_PHRASE_INV_PARAM))
        return RN2903_RESPONSE_INVALID_PARAM;
    else
        return RN2903_RESPONSE_OK; // either data or "ok"
}

RN2903_RESPONSE_T rn2903_command(const rn2903_context dev, const char *cmd)
{
    assert(dev != NULL);
    assert(cmd != NULL);

    rn2903_drain(dev);

    if (dev->debug)
        printf("CMD: '%s'\n", cmd);

    if (rn2903_write(dev, cmd, strlen(cmd)) < 0)
    {
        printf("%s: rn2903_write(cmd) failed\n", __FUNCTION__);
        return RN2903_RESPONSE_UPM_ERROR;
    }

    // now write the termination string (CR/LF)
    if (rn2903_write(dev, RN2903_PHRASE_TERM, RN2903_PHRASE_TERM_LEN) < 0)
    {
        printf("%s: rn2903_write(TERM) failed\n", __FUNCTION__);
        return RN2903_RESPONSE_UPM_ERROR;
    }

    return rn2903_waitfor_response(dev, dev->cmd_resp_wait_ms);
}

RN2903_RESPONSE_T rn2903_command_with_arg(const rn2903_context dev,
                                          const char *cmd, const char *arg)
{
    assert(dev != NULL);
    assert(cmd != NULL);
    assert(arg != NULL);

    // cmd<space>arg<0-terminator>
    int buflen = strlen(cmd) + 1 + strlen(arg) + 1;
    char buf[buflen];
    memset(buf, 0, buflen);

    snprintf(buf, buflen, "%s %s", cmd, arg);

    return rn2903_command(dev, buf);
}

const char *rn2903_get_response(const rn2903_context dev)
{
    assert(dev != NULL);

    return dev->resp_data;
}

size_t rn2903_get_response_len(const rn2903_context dev)
{
    assert(dev != NULL);

    return dev->resp_len;
}

const char *rn2903_to_hex(const rn2903_context dev, const char *src, int len)
{
    assert(dev != NULL);
    assert(src != NULL);

    static const char hdigits[16] = "0123456789ABCDEF";

    // first free previous destination hex buffer if allocated
    if (dev->to_hex_buf)
    {
        free(dev->to_hex_buf);
        dev->to_hex_buf = NULL;
    }

    if (len == 0)
        return NULL;

    int dlen = (len * 2) + 1;

    if (!(dev->to_hex_buf = malloc(dlen)))
    {
        printf("%s: malloc(%d) failed\n", __FUNCTION__, dlen);
        return NULL;
    }
    memset(dev->to_hex_buf, 0, dlen);

    char *dptr = dev->to_hex_buf;
    char *sptr = (char *)src;
    for (int i=0; i<len; i++)
    {
        *dptr++ = hdigits[(sptr[i] >> 4) & 0x0f];
        *dptr++ = hdigits[(sptr[i] & 0x0f)];
    }

    // the memset() will have ensured the last byte is 0
    return dev->to_hex_buf;
}

const char *rn2903_from_hex(const rn2903_context dev,
                            const char *src)
{
    assert(dev != NULL);
    assert(src != NULL);

    // first free previous destination hex buffer if allocated
    if (dev->from_hex_buf)
    {
        free(dev->from_hex_buf);
        dev->from_hex_buf = NULL;
    }

    int len = strlen(src);
    if (len == 0)
        return NULL;

    if (!validate_hex_str(src))
        return NULL;

    // add a byte for 0 termination, just in case we're decoding a
    // string
    int dlen = (len / 2) + 1;

    if (!(dev->from_hex_buf = malloc(dlen)))
    {
        printf("%s: malloc(%d) failed\n", __FUNCTION__, dlen);
        return NULL;
    }
    memset(dev->from_hex_buf, 0, dlen);

    char *dptr = dev->from_hex_buf;
    for (int i=0; i<(dlen - 1); i++)
    {
        char tbuf[3] = { src[i*2], src[(i*2)+1], 0 };
        *dptr++ = (char)strtol(tbuf, NULL, 16);
    }

    return dev->from_hex_buf;
}

const char *rn2903_get_hardware_eui(const rn2903_context dev)
{
    assert(dev != NULL);

    return dev->hardware_eui;
}

upm_result_t rn2903_update_mac_status(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac get status"))
    {
        printf("%s: rn2903_command(mac get status) failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // make sure we actually got a hex value of 4 bytes
    if (!validate_hex_str(dev->resp_data) || dev->resp_len != 4)
    {
        printf("%s: invalid mac status.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // convert it
    const char *statPtr = rn2903_from_hex(dev, dev->resp_data);
    if (!statPtr)
    {
        printf("%s: from_hex conversion failed.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    // now play pointer games.  We should have 2 bytes which we'll
    // turn into a uint16_t (LE), which we will then stuff into our
    // mac_status_word field in the context.
    uint16_t status16 = (statPtr[0] << 8) | statPtr[1];

    // store the mac_status_word, then decode the actual mac_status
    // (state) enumeration

    dev->mac_status_word = status16;
    dev->mac_mac_status =
        (RN2903_MAC_STATUS_T)((status16 & _SHIFTMASK(MAC_STATUS_MAC_STATUS))
                              >> _SHIFT(MAC_STATUS_MAC_STATUS));

    return UPM_SUCCESS;
}

uint16_t rn2903_get_mac_status_word(const rn2903_context dev)
{
    assert(dev != NULL);

    return dev->mac_status_word;
}

RN2903_MAC_STATUS_T rn2903_get_mac_status(const rn2903_context dev)
{
    assert(dev != NULL);

    return dev->mac_mac_status;
}

upm_result_t rn2903_reset(const rn2903_context dev)
{
    assert(dev != NULL);

    rn2903_autobaud(dev, RN2903_AUTOBAUD_RETRIES);

    if (rn2903_command(dev, "sys reset"))
    {
        // this command will reset the baudrate back to the default if
        // we changed it previously.  We do not report an error here,
        // if we are not using the default baudrate, since we've now
        // switched to a different baudrate than we had, and cannot
        // read the response anyway.
        if (dev->baudrate == RN2903_DEFAULT_BAUDRATE)
            return UPM_ERROR_OPERATION_FAILED;
    }

    // to be safe, always set the baudrate after a reset
    if (rn2903_set_baudrate(dev, dev->baudrate))
        return UPM_ERROR_OPERATION_FAILED;

    upm_delay_ms(100);

    return UPM_SUCCESS;
}

RN2903_JOIN_STATUS_T rn2903_join(const rn2903_context dev,
                                 RN2903_JOIN_TYPE_T type)
{
    assert(dev != NULL);

    // first, do a couple of initial checks...

    // get the mac status and ensure that 1) we are not already
    // joined, 2) the mac status is idle, 3) we have not been
    // silenced, and 4) MAC has not been paused.

    if (rn2903_update_mac_status(dev))
    {
        printf("%s: rn2903_update_mac_status() failed\n", __FUNCTION__);
        return RN2903_JOIN_STATUS_UPM_ERROR;
    }

    // if the radio is not idle, we aren't going anywhere
    RN2903_MAC_STATUS_T mac_status = rn2903_get_mac_status(dev);
    if (mac_status != RN2903_MAC_STAT_IDLE)
        return RN2903_JOIN_STATUS_BUSY;

    // now check the rest of the status bits...
    uint16_t status = rn2903_get_mac_status_word(dev);
    if (status & RN2903_MAC_STATUS_JOINED)
        return RN2903_JOIN_STATUS_ALREADY_JOINED;
    else if (status & RN2903_MAC_STATUS_SILENT)
        return RN2903_JOIN_STATUS_SILENT;
    else if (status & RN2903_MAC_STATUS_PAUSED)
        return RN2903_JOIN_STATUS_MAC_PAUSED;

    // so far, so good... now build the command

    char cmd[RN2903_CMD_BUFFER_32B] = {};
    snprintf(cmd, RN2903_CMD_BUFFER_32B, "mac join %s",
             (type == RN2903_JOIN_TYPE_OTAA) ? "otaa" : "abp");

    // run the command.  We will get two responses back - one
    // immediately if there is an error or if the join operation was
    // successfully submitted to the radio for transmission, and
    // another indicating whether the join was granted, or failed.
    // ABP joins will always succeed immediately.

    RN2903_RESPONSE_T rv;
    if ((rv = rn2903_command(dev, cmd)))
    {
        // a failure of some sort.  We've already screened for most of
        // them, but there are a couple that we can't detect until we
        // try (UPM).
        printf("%s: join command failed (%d).\n", __FUNCTION__, rv);
        return RN2903_JOIN_STATUS_UPM_ERROR;
    }

    // if we are here, then we either got an "ok" or another error we
    // couldn't screen for.  Check for them.

    if (rn2903_find(dev, "no_free_ch"))
        return RN2903_JOIN_STATUS_NO_CHAN;
    else if (rn2903_find(dev, "keys_not_init"))
        return RN2903_JOIN_STATUS_BAD_KEYS;

    // now we wait awhile for another response indicating whether the
    // join request was accepted or not

    if ((rv = rn2903_waitfor_response(dev, dev->cmd_resp2_wait_ms)))
    {
        printf("%s: join second response failed (%d).\n", __FUNCTION__, rv);
        return RN2903_JOIN_STATUS_UPM_ERROR;
    }

    if (rn2903_find(dev, "denied"))
        return RN2903_JOIN_STATUS_DENIED;
    else if (rn2903_find(dev, "accepted"))
        return RN2903_JOIN_STATUS_ACCEPTED;

    // if it's anything else, we failed :(
    printf("%s: unexpected response to join request.\n", __FUNCTION__);

    return RN2903_JOIN_STATUS_UPM_ERROR;
}

upm_result_t rn2903_set_flow_control(const rn2903_context dev,
                                     RN2903_FLOW_CONTROL_T fc)
{
    assert(dev != NULL);

    mraa_result_t rv = MRAA_SUCCESS;

    switch(fc)
    {
    case RN2903_FLOW_CONTROL_NONE:
        rv = mraa_uart_set_flowcontrol(dev->uart, false, false);
        break;

    case RN2903_FLOW_CONTROL_HARD:
        rv = mraa_uart_set_flowcontrol(dev->uart, false, true);
        break;

    default:
        return UPM_ERROR_INVALID_PARAMETER;
    }

    if (rv == MRAA_SUCCESS)
        return UPM_SUCCESS;
    else
        return UPM_ERROR_OPERATION_FAILED;
}

bool rn2903_find(const rn2903_context dev, const char *str)
{
    assert(dev != NULL);
    assert(str != NULL);

    return ((strstr(dev->resp_data, str) == dev->resp_data) ? true : false);
}

upm_result_t rn2903_set_device_eui(const rn2903_context dev,
                                   const char *str)
{
    assert(dev != NULL);
    assert(str != NULL);

    // first verify that the string is a valid hex string of the right
    // size (16 bytes for the 8 byte EUI)

    if (!validate_hex_str(str) || strlen(str) != 16)
    {
        printf("%s: invalid 16 byte device EUI hex string.\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (rn2903_command_with_arg(dev, "mac set deveui", str))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_get_device_eui(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac get deveui"))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_set_application_eui(const rn2903_context dev,
                                        const char *str)
{
    assert(dev != NULL);
    assert(str != NULL);

    // first verify that the string is a valid hex string of the right
    // size (16 bytes for the 8 byte EUI)

    if (!validate_hex_str(str) || strlen(str) != 16)
    {
        printf("%s: invalid 16 byte application EUI hex string.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (rn2903_command_with_arg(dev, "mac set appeui", str))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_get_application_eui(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac get appeui"))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_set_application_key(const rn2903_context dev,
                                        const char *str)
{
    assert(dev != NULL);
    assert(str != NULL);

    // first verify that the string is a valid hex string of the right
    // size (32 bytes for the 16 byte key)

    if (!validate_hex_str(str) || strlen(str) != 32)
    {
        printf("%s: invalid 32 byte application key hex string.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (rn2903_command_with_arg(dev, "mac set appkey", str))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_get_application_key(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac get appkey"))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_set_device_addr(const rn2903_context dev,
                                    const char *str)
{
    assert(dev != NULL);
    assert(str != NULL);

    // first verify that the string is a valid hex string of the right
    // size (8 bytes for the 4 byte key)

    if (!validate_hex_str(str) || strlen(str) != 8)
    {
        printf("%s: invalid 8 byte device addr hex string.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (rn2903_command_with_arg(dev, "mac set devaddr", str))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_get_device_addr(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac get devaddr"))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_set_network_session_key(const rn2903_context dev,
                                            const char *str)
{
    assert(dev != NULL);
    assert(str != NULL);

    // first verify that the string is a valid hex string of the right
    // size (32 bytes for the 16 byte key)

    if (!validate_hex_str(str) || strlen(str) != 32)
    {
        printf("%s: invalid 32 byte network session key hex string.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (rn2903_command_with_arg(dev, "mac set nwkskey", str))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_set_application_session_key(const rn2903_context dev,
                                                const char *str)
{
    assert(dev != NULL);
    assert(str != NULL);

    // first verify that the string is a valid hex string of the right
    // size (32 bytes for the 16 byte key)

    if (!validate_hex_str(str) || strlen(str) != 32)
    {
        printf("%s: invalid 32 byte application session key hex string.\n",
               __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    if (rn2903_command_with_arg(dev, "mac set appskey", str))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_mac_save(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac save"))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_mac_pause(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac pause"))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

upm_result_t rn2903_mac_resume(const rn2903_context dev)
{
    assert(dev != NULL);

    if (rn2903_command(dev, "mac resume"))
        return UPM_ERROR_OPERATION_FAILED;

    return UPM_SUCCESS;
}

RN2903_MAC_TX_STATUS_T rn2903_mac_tx(const rn2903_context dev,
                                     RN2903_MAC_MSG_TYPE_T type,
                                     int port, const char *payload)
{
    assert(dev != NULL);
    assert(payload != NULL);

    // check some things

    // port can only be between 1 and 223
    if (port < 1 || port > 223)
    {
        printf("%s: port must be between 1 and 223\n", __FUNCTION__);
        return RN2903_MAC_TX_STATUS_UPM_ERROR;
    }

    // make sure payload is a valid hex string
    if (!validate_hex_str(payload))
    {
        printf("%s: payload is not a valid hex string\n", __FUNCTION__);
        return RN2903_MAC_TX_STATUS_UPM_ERROR;
    }

    // get the mac status and ensure that 1) we are joined to a
    // LoRaWAN network, 2) the mac status is idle, 3) we have not been
    // silenced, and 4) MAC has not been paused.

    if (rn2903_update_mac_status(dev))
    {
        printf("%s: rn2903_update_mac_status() failed\n", __FUNCTION__);
        return RN2903_MAC_TX_STATUS_UPM_ERROR;
    }

    // if the radio is not idle, we aren't going anywhere
    RN2903_MAC_STATUS_T mac_status = rn2903_get_mac_status(dev);
    if (mac_status != RN2903_MAC_STAT_IDLE)
        return RN2903_MAC_TX_STATUS_BUSY;

    // now check the rest of the status bits...
    uint16_t status = rn2903_get_mac_status_word(dev);
    if (!(status & RN2903_MAC_STATUS_JOINED))
        return RN2903_MAC_TX_STATUS_NOT_JOINED;
    else if (status & RN2903_MAC_STATUS_SILENT)
        return RN2903_MAC_TX_STATUS_SILENT;
    else if (status & RN2903_MAC_STATUS_PAUSED)
        return RN2903_MAC_TX_STATUS_MAC_PAUSED;

    // good so far, build and send the command.  Then we check for
    // more things.

    char cmd[RN2903_CMD_BUFFER_32B] = {};
    snprintf(cmd, RN2903_CMD_BUFFER_32B, "mac tx %s %d",
             (type == RN2903_MAC_MSG_TYPE_CONFIRMED) ? "cnf" : "uncnf",
             port);

    RN2903_RESPONSE_T rv;
    if ((rv = rn2903_command_with_arg(dev, cmd, payload)))
    {
        printf("%s: mac tx command failed (%d).\n", __FUNCTION__, rv);
        return RN2903_MAC_TX_STATUS_UPM_ERROR;
    }

    // now check for some other things we couldn't check before
    if (rn2903_find(dev, "no_free_ch"))
        return RN2903_MAC_TX_STATUS_NO_CHAN;
    else if (rn2903_find(dev, "frame_counter_err_rejoin_needed"))
        return RN2903_MAC_TX_STATUS_FC_NEED_REJOIN;
    else if (rn2903_find(dev, "invalid_data_len"))
        return RN2903_MAC_TX_STATUS_BAD_DATA_LEN;

    // now we wait for transmission to complete, and a possible
    // downlink packet.

    if ((rv = rn2903_waitfor_response(dev, dev->cmd_resp2_wait_ms)))
    {
        printf("%s: mac tx second response failed (%d).\n", __FUNCTION__, rv);
        return RN2903_MAC_TX_STATUS_UPM_ERROR;
    }

    if (rn2903_find(dev, "mac_tx_ok"))
        return RN2903_MAC_TX_STATUS_TX_OK;
    else if (rn2903_find(dev, "mac_err"))
        return RN2903_MAC_TX_STATUS_MAC_ERR;
    else if (rn2903_find(dev, "invalid_data_len"))
        return RN2903_MAC_TX_STATUS_BAD_DATA_LEN;
    else if (rn2903_find(dev, "mac_rx"))
        return RN2903_MAC_TX_STATUS_RX_RECEIVED; // we got a downlink
                                                 // packet in the
                                                 // response buffer

    // if it's anything else, we failed :(
    printf("%s: unexpected response to mac tx command\n", __FUNCTION__);

    return RN2903_MAC_TX_STATUS_UPM_ERROR;
}

RN2903_RESPONSE_T rn2903_radio_tx(const rn2903_context dev,
                                  const char *payload)
{
    assert(dev != NULL);
    assert(payload != NULL);

    // check some things

    // make sure payload is a valid hex string
    if (!validate_hex_str(payload))
    {
        printf("%s: payload is not a valid hex string\n", __FUNCTION__);
        return RN2903_RESPONSE_UPM_ERROR;
    }

    // get the mac status and ensure that we are paused
    if (rn2903_update_mac_status(dev))
    {
        printf("%s: rn2903_update_mac_status() failed\n", __FUNCTION__);
        return RN2903_RESPONSE_UPM_ERROR;
    }

    uint16_t status = rn2903_get_mac_status_word(dev);
    if (!(status & RN2903_MAC_STATUS_PAUSED))
    {
        printf("%s: MAC must be paused first\n", __FUNCTION__);
        return RN2903_RESPONSE_UPM_ERROR;
    }

    // good so far, build and send the command

    RN2903_RESPONSE_T rv;
    if ((rv = rn2903_command_with_arg(dev, "radio tx", payload)))
    {
        printf("%s: radio tx command failed (%d).\n", __FUNCTION__, rv);
        return rv;
    }

    // we're done
    return RN2903_RESPONSE_OK;
}

RN2903_RESPONSE_T rn2903_radio_rx(const rn2903_context dev,
                                  int window_size)
{
    assert(dev != NULL);

    // check some things

    // get the mac status and ensure that we are paused
    if (rn2903_update_mac_status(dev))
    {
        printf("%s: rn2903_update_mac_status() failed\n", __FUNCTION__);
        return RN2903_RESPONSE_UPM_ERROR;
    }

    uint16_t status = rn2903_get_mac_status_word(dev);
    if (!(status & RN2903_MAC_STATUS_PAUSED))
    {
        printf("%s: MAC must be paused first\n", __FUNCTION__);
        return RN2903_RESPONSE_UPM_ERROR;
    }

    // good so far, build and send the command

    char cmd[RN2903_CMD_BUFFER_32B] = {};
    snprintf(cmd, RN2903_CMD_BUFFER_32B, "radio rx %d", window_size);

    RN2903_RESPONSE_T rv;
    if ((rv = rn2903_command(dev, cmd)))
    {
        printf("%s: radio tx command failed (%d).\n", __FUNCTION__, rv);
        return rv;
    }

    // now, wait for and return the second response
    return rn2903_waitfor_response(dev, dev->cmd_resp2_wait_ms);
}

upm_result_t rn2903_mac_set_battery(const rn2903_context dev, int level)
{
    assert(dev != NULL);

    if (level < 0 || level > 255)
    {
        printf("%s: level must be between 0 and 255\n", __FUNCTION__);
        return UPM_ERROR_OPERATION_FAILED;
    }

    char cmd[RN2903_CMD_BUFFER_32B] = {};
    snprintf(cmd, RN2903_CMD_BUFFER_32B, "mac set bat %d", level);

    RN2903_RESPONSE_T rv;
    if ((rv = rn2903_command(dev, cmd)))
    {
        printf("%s: mac tx command failed (%d).\n", __FUNCTION__, rv);
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}

bool rn2903_autobaud(const rn2903_context dev, int retries)
{
    assert(dev != NULL);

    do
    {
        // trigger rn2903 auto-baud detection

        // send a break signal, then a 0x55, then try a command
        mraa_result_t rv;
        if ((rv = mraa_uart_sendbreak(dev->uart, 0)))
        {
            // we don't want to fail here if break not implemented or
            // supported
            if (rv != MRAA_ERROR_FEATURE_NOT_IMPLEMENTED &&
                rv != MRAA_ERROR_FEATURE_NOT_SUPPORTED)
            {

                printf("%s: mraa_uart_sendbreak() failed.\n", __FUNCTION__);
                return UPM_ERROR_OPERATION_FAILED;
            }
        }

        upm_delay_ms(100);

        // The magic autobaud detection character
        char buf = 0x55;
        rn2903_write(dev, &buf, 1);

        upm_delay_ms(100);

        // try a command to verify speed
        if (!rn2903_command(dev, "sys get ver"))
            break;

        if (dev->debug)
            printf("%s: RETRIES %d: FAIL!\n", __FUNCTION__, retries);
    } while (retries-- > 0);

    if (retries <= 0)
        return false;

    if (dev->debug)
        printf("%s: RETRIES %d: success!\n", __FUNCTION__, retries);

    return true;
}

const char *rn2903_get_radio_rx_payload(const rn2903_context dev)
{
    assert(dev != NULL);

    // first make sure we have the right data in the response buffer.
    // The response buffer should contain "radio_rx<sp><sp><hex_payload>"
    // (note the two spaces between radio_rx and the payload.)
    if (rn2903_find(dev, "radio_rx") || dev->resp_len > 10)
        return &(dev->resp_data[10]);

    return NULL;
}
