/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <upm.h>

#include <mraa/i2c.h>
#include <mraa/spi.h>
#include <mraa/gpio.h>

#include "mcp2515_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file mcp2515
     * @library mcp2515
     * @brief C API for the MCP2515 CAN bus controller
     *
     * @include mcp2515.c
     * @include mcp2515_regs.c
     */

    /**
     * Device context
     */
    typedef struct _mcp2515_context {
        mraa_spi_context        spi;

        // CS pin, if we are using one
        mraa_gpio_context       gpio;

        // interrupt, if enabled
        mraa_gpio_context       intr;
    } *mcp2515_context;

    /**
     * MCP2515 initializer
     *
     * @param bus spi bus to use
     * @param cs_pin The GPIO pin to use for Chip Select (CS).  Pass
     * pass -1 if your CS is handled automatically by your SPI
     * implementation (Edison).
     * @return an initialized device context on success, NULL on error.
     */
    mcp2515_context mcp2515_init(int bus, int cs_pin);

    /**
     * MCP2515 close
     *
     * @param dev Device context.
     */
    void mcp2515_close(mcp2515_context dev);

    /**
     * Reset the device.
     *
     * @param dev Device context.
     * @return UPM result.
     */
    upm_result_t mcp2515_reset(const mcp2515_context dev);

    /**
     * Set the operating mode of the device.  After initialization
     * (mcp2515_init()), the device mode will be set to NORMAL.  Note
     * that some operations require the device to be placed into CONFIG
     * mode.  This function will wait until the selected mode has been
     * entered.
     *
     * @param dev Device context.
     * @param opmode One of the MCP2515_OPMODE_T values.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_opmode(const mcp2515_context dev,
                                    MCP2515_OPMODE_T opmode);

    /**
     * Set the baud rate of the CAN bus.  All devices on a given CAN
     * bus must be operating at the correct speed.  The device must be
     * switched into CONFIG mode bofore this function will have any
     * effect.  On initialization (mcp2515_init()), the default CAN
     * bus speed will be set to 50Kbps.
     *
     * @param dev Device context.
     * @param speed One of the MCP2515_SPEED_T values.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_speed(const mcp2515_context dev,
                                   MCP2515_SPEED_T speed);

    /**
     * Load a transmit buffer with the id, payload, and other
     * information.  This function does not actually transmit the
     * buffer.  There are 3 TX buffers available.  The buffer must be
     * free (ie: not awaiting transmit) before it can be loaded.  Once
     * a TX buffer has been successfully loaded, it can be transmitted
     * with the mcp2515_transmit_buffer() function.
     *
     * @param dev Device context.
     * @param bufnum The buffer to load.  One of the
     * MCP2515_TX_BUFFER_T values.
     * @param id The integer representation of the CAN bus ID.
     * @param ext True if the ID is an extended identifier, false otherwise.
     * @param rtr True if this is a Remote Transfer Request, false otherwise.
     * @param payload A pointer to an array of bytes that make up the
     * payload to send.
     * @param len The length of the payload.  The length is limited to
     * 8.
     * @return UPM result.
     */
    upm_result_t mcp2515_load_tx_buffer(const mcp2515_context dev,
                                        MCP2515_TX_BUFFER_T bufnum,
                                        int id, bool ext, bool rtr,
                                        uint8_t *payload, int len);

    /**
     * Transmit a buffer already loaded by mcp2515_load_tx_buffer().
     *
     * @param dev Device context.
     * @param bufnum The buffer to transmit.  One of the
     * MCP2515_TX_BUFFER_T values.
     * @param wait True if the function should wait until transmission
     * is complete before returning, false otherwise.
     * @return UPM result.
     */
    upm_result_t mcp2515_transmit_buffer(const mcp2515_context dev,
                                         MCP2515_TX_BUFFER_T bufnum,
                                         bool wait);

    /**
     * Determine whether a TX buffer is available for use or not. A TX
     * buffer is unavailable if a transmit request is pending on it,
     * but transmission has not yet completed.
     *
     * @param dev Device context.
     * @param bufnum The buffer to check. One of the
     * MCP2515_TX_BUFFER_T values.
     * @return True if the buffer is availabe, false otherwise.
     */
    bool mcp2515_tx_buffer_is_free(const mcp2515_context dev,
                                   MCP2515_TX_BUFFER_T bufnum);

    /**
     * Find a free TX buffer and return it.
     *
     * @param dev Device context.
     * @return One of the MCP2515_TX_BUFFER_T values.  If no buffers
     * are available, MCP2515_TX_NO_BUFFERS will be returned.
     */
    MCP2515_TX_BUFFER_T mcp2515_find_free_tx_buffer(const mcp2515_context dev);

    /**
     * Set the transmit priority of a TX buffer.  Higher priority
     * buffers are always transmitted before lower priority buffers.
     * This function can be called on a buffer at any time prior to
     * actual transmission.
     *
     * @param dev Device context.
     * @param bufnum The buffer to set priority for.  One of the
     * MCP2515_TX_BUFFER_T values.
     * @param priority The priority to set for the buffer.  One of the
     * MCP2515_TXP_T values.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_tx_buffer_priority(const mcp2515_context dev,
                                                MCP2515_TX_BUFFER_T bufnum,
                                                MCP2515_TXP_T priority);

    /**
     * Abort a transmission that has been queued, but not yet
     * completed.  This will also free up the TX buffer for future
     * use.  Note, if you abort a transmission, but transmission has
     * already started, this call will have no effect, and the buffer
     * will complete transmission.
     *
     * @param dev Device context.
     * @param bufnum The buffer to abort.  One of the
     * MCP2515_TX_BUFFER_T values.
     * @return UPM result.
     */
    upm_result_t mcp2515_abort_tx(const mcp2515_context dev,
                                  MCP2515_TX_BUFFER_T bufnum);

    /**
     * Set the mode for an RX buffer.  The mode specifies, at a high
     * level, what packets should be captured from the bus and placed
     * into an RX buffer.  See the datasheet for details, but the
     * default, MCP2515_RXMODE_ANY_FILTER, should be sufficient in
     * most cases.  It is also possible to use this to restrict the
     * types of CAN ids accepted (extended only, standard only) as
     * well as a debug ANY_NOFILTER mode.
     *
     * @param dev Device context.
     * @param bufnum The buffer to set the mode for.  One of the
     * MCP2515_RX_BUFFER_T values.
     * @param rxm The mode to set.  One of the MCP2515_RXMODE_T values.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_rx_buffer_mode(const mcp2515_context dev,
                                            MCP2515_RX_BUFFER_T bufnum,
                                            MCP2515_RXMODE_T rxm);

    /**
     * Return a bitmask indicating which of the 2 RX buffers have
     * packets waiting in them.  This can be 0 (no packets), 1(RXB0),
     * 2(RXB1), or 3 (RXB0 and RXB1).  This information is retrieved
     * using the MCP2515_CMD_RX_STATUS command.
     *
     * @param dev Device context.
     * @return A bitmask indicating which RX buffers (if any) have
     * packets in them.  One of the MCP2515_RXMSG_T values.
     */
    MCP2515_RXMSG_T mcp2515_rx_status_msgs(const mcp2515_context dev);

    /**
     * Return the message type present in one of the RX buffers.  RXB0
     * has the highest priority, so if both RX buffers are full, this
     * function will only return data for the packet in RXB0.  This
     * information is retrieved using the MCP2515_CMD_RX_STATUS
     * command.
     *
     * @param dev Device context.
     * @return One of the MCP2515_MSGTYPE_T values.
     */
    MCP2515_MSGTYPE_T mcp2515_rx_status_msg_type(const mcp2515_context dev);

    /**
     * Return the filter that matched an RX buffer.  RXB0 has the
     * highest priority, so if both RX buffers are full, this function
     * will only return data for the packet in RXB0.  This information
     * is retrieved using the MCP2515_CMD_RX_STATUS command.
     *
     * @param dev Device context.
     * @return One of the MCP2515_FILTERMATCH_T values.
     */
    MCP2515_FILTERMATCH_T mcp2515_rx_status_filtermatch(const mcp2515_context dev);

    /**
     * This function retrieves a message from the specified RX buffer.
     * The message (MCP2515_MSG_T) contains all of the data in the
     * packet, including id, rtr, ext, payload and payload length.  In
     * addition, after retrieving the message, the RX buffer is freed
     * to receive further data from the CAN bus.
     *
     * @param dev Device context.
     * @param bufnum The buffer to retrieve.  One of the
     * MCP2515_RX_BUFFER_T values.
     * @param msg A pointer to a MCP2515_MSG_T, which will be filled
     * with the decoded contents of the RX buffer.
     * @return UPM result.
     */
    upm_result_t mcp2515_get_rx_msg(const mcp2515_context dev,
                                    MCP2515_RX_BUFFER_T bufnum,
                                    MCP2515_MSG_T *msg);

    /**
     * This is a utility function that accepts a MCP2515_MSG_T and
     * prints out it's decoded contents.  This is of primary
     * importance for debugging, and to simplify the examples
     * somewhat.
     *
     * The output will look similar to:
     *
     * id 00000000 ext 0 rtr 0 filt 0 len 8
     *          payload: 0xc8 0x01 0x02 0x03 0x04 0x05 0x06 0x07
     *
     * @param dev Device context.
     * @param msg A pointer to a MCP2515_MSG_T containing the data to
     * be printed out.
     */
    void mcp2515_print_msg(const mcp2515_context dev, MCP2515_MSG_T *msg);

    /**
     * Installs an interrupt service routine (ISR) to be called when
     * an interrupt occurs.
     *
     * @param dev Device context.
     * @param pin GPIO pin to use as the interrupt pin.
     * @param isr Pointer to a function to be called on interrupt.
     * @param arg Pointer to an object to be supplied as an
     * argument to the ISR.
     * @return UPM result.
     */
    upm_result_t mcp2515_install_isr(const mcp2515_context dev, int pin,
                                     void (*isr)(void *), void *arg);

    /**
     * Uninstalls the previously installed ISR
     *
     * @param dev Device context.
     */
    void mcp2515_uninstall_isr(const mcp2515_context dev);

    /**
     * Set the interrupt enables register.
     *
     * @param dev Device context.
     * @param enables A bitmask of interrupts to enable from
     * MCP2515_CANINT_BITS_T.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_intr_enables(const mcp2515_context dev,
                                          uint8_t enables);

    /**
     * Retrieve the interrupt flags register.
     *
     * @param dev Device context.
     * @param flags A pointer to a bitmask that will be filled with
     * values from MCP2515_CANINT_BITS_T, indicating which interrupt
     * flags are set.
     * @return UPM result.
     */
    upm_result_t mcp2515_get_intr_flags(const mcp2515_context dev,
                                        uint8_t *flags);

    /**
     * This function allows you to set specific interrupt flags.  If
     * the corresponding interrupt enable is set, an interrupt will be
     * generated.
     *
     * @param dev Device context.
     * @param flags A bitmask of interrupt flags to set, from
     * MCP2515_CANINT_BITS_T values.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_intr_flags(const mcp2515_context dev,
                                        uint8_t flags);

    /**
     * This function allows you to clear specific interrupt flags.
     * See the datasheet.  Some flags cannot be cleared until the
     * underlying cause has been corrected.
     *
     * @param dev Device context.
     * @param flags A bitmask of interrupt flags to clear, from
     * MCP2515_CANINT_BITS_T values.
     * @return UPM result.
     */
    upm_result_t mcp2515_clear_intr_flags(const mcp2515_context dev,
                                          uint8_t flags);

    /**
     * Retrieve the error flags register
     *
     * @param dev Device context.
     * @param flags A pointer to a uint8_t that will hold the flags
     * with values from MCP2515_EFLG_BITS_T.
     * @return UPM result.
     */
    upm_result_t mcp2515_get_error_flags(const mcp2515_context dev,
                                         uint8_t *flags);

    /**
     * Clear error flags.  Note, some flags cannot be cleared
     * until the underlying issues is resolved.
     *
     * @param dev Device context.
     * @param flags A bitmask of values from MCP2515_EFLG_BITS_T
     * specifying the flags to clear.
     * @return UPM result.
     */
    upm_result_t mcp2515_clear_error_flags(const mcp2515_context dev,
                                           uint8_t flags);

    /**
     * This function allows you to set one of the 6 RX filters
     * available.  Filters 0 and 1 are for RXB0 only, while filters
     * 2-5 are for RXB1.  See the datasheet for details on how these
     * filters (along with the masks) are used to select candidate CAN
     * bus data for retrieval from the CAN bus.
     *
     * These can only be set when the device is in CONFIG mode.
     *
     * @param dev Device context.
     * @param filter One of the 6 MCP2515_RX_FILTER_T values.
     * @param ext True if the id is extended, false for standard.
     * @param id Integer representation of a CAN bus ID.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_filter(const mcp2515_context dev,
                                    MCP2515_RX_FILTER_T filter, bool ext,
                                    int id);

    /**
     * This function allows you to set one of the 2 RX masks.  Mask 0
     * is for RXB0, mask 1 is for RXB1.  The masks specify which bits
     * in the filters are used for matching CAN bus data.  See the
     * datasheet for details on how these masks (along with the
     * filters) are used to select candidate CAN bus data for retrieval
     * from the CAN bus.
     *
     * These can only be set when the device is in CONFIG mode.
     *
     * @param dev Device context.
     * @param mask One of the 2 MCP2515_RX_MASK_T values.
     * @param ext True if the id is extended, false for standard.
     * @param id Integer representation of a CAN bus ID.
     * @return UPM result.
     */
    upm_result_t mcp2515_set_mask(const mcp2515_context dev,
                                  MCP2515_RX_MASK_T mask, bool ext, int id);

    /**
     * Perform a bus read.  This function is exposed here for those
     * users wishing to perform their own low level accesses.  This is
     * a low level function, and should not be used unless you know
     * what you are doing.
     *
     * @param dev Device context.
     * @param cmd The command to send.
     * @param args An array of arguments, or NULL.
     * @param arglen The number of args, or 0.
     * @param data A pointer to a buffer in which data will be read into.
     * @param len The number of bytes to read.
     * @return UPM result.
     */
    upm_result_t mcp2515_bus_read(const mcp2515_context dev, uint8_t cmd,
                                  uint8_t *args, unsigned int arglen,
                                  uint8_t *data, uint8_t len);

    /**
     * Perform a bus write.  This function is exposed here for those
     * users wishing to perform their own low level accesses.  This is
     * a low level function, and should not be used unless you know
     * what you are doing.
     *
     * @param dev Device context
     * @param cmd The command to send.
     * @param data A pointer to a buffer containing data to write.
     * @param len The number of bytes to write.
     * @return UPM Status
     */
    upm_result_t mcp2515_bus_write(const mcp2515_context dev, uint8_t cmd,
                                   uint8_t *data, uint8_t len);

    /**
     * Write to a specific register.  This function is exposed here
     * for those users wishing to perform their own low level
     * accesses.  This is a low level function, and should not be used
     * unless you know what you are doing.
     *
     * @param dev Device context
     * @param reg The register to write to.
     * @param value The byte to write.
     * @return UPM Status
     */
    upm_result_t mcp2515_write_reg(const mcp2515_context dev, uint8_t reg,
                                   uint8_t value);

    /**
     * Write to multiple consecutive registers.  This function is
     * exposed here for those users wishing to perform their own low
     * level accesses.  This is a low level function, and should not
     * be used unless you know what you are doing.
     *
     * @param dev Device context
     * @param reg The register to start writing to.
     * @param buffer A pointer to an array of bytes to write.
     * @param len The number of bytes to write.
     * @return UPM Status
     */
    upm_result_t mcp2515_write_regs(const mcp2515_context dev, uint8_t reg,
                                    uint8_t *buffer, int len);

    /**
     * Read a register.  This function is exposed here for those users
     * wishing to perform their own low level accesses.  This is a low
     * level function, and should not be used unless you know what you
     * are doing.
     *
     * @param dev Device context
     * @param reg The register to read.
     * @param buffer A pointer to a a byte where the value will be placed.
     * @return UPM Status
     */
    upm_result_t mcp2515_read_reg(const mcp2515_context dev, uint8_t reg,
                                  uint8_t *value);

    /**
     * Perform a bit modify operation on a register.  Only certain
     * registers support this method of access - check the datasheet.
     * This function is exposed here for those users wishing to
     * perform their own low level accesses.  This is a low level
     * function, and should not be used unless you know what you are
     * doing.
     *
     * @param dev Device context
     * @param addr The address of the register to access.
     * @param mask A bitmask used to mask off value bits.
     * @param value The value to write (bits masked by mask).
     * @return UPM Status
     */
    upm_result_t mcp2515_bit_modify(const mcp2515_context dev, uint8_t addr,
                                    uint8_t mask, uint8_t value);

#ifdef __cplusplus
}
#endif
