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

#include <string>

#include "mcp2515.h"

namespace upm {

    /**
     * @brief MCP2515 CAN Bus Controller
     * @defgroup mcp2515 libupm-mcp2515
     * @ingroup spi gpio
     */

    /**
     * @library mcp2515
     * @sensor mcp2515
     * @comname CAN Bus Controller
     * @man seeed
     * @con spi gpio
     * @web https://www.seeedstudio.com/CANBUS-Shield-p-2256.html
     *
     * @brief API for the MCP2515 CAN bus controller
     *
     * The MCP2515 is a stand-alone CAN controller developed to
     * simplify applications that require interfacing with a CAN bus.
     *
     * This driver was developed using the Grove CAN bus shield
     * version 1.2.
     *
     * An example using the loopback mode.
     * @snippet mcp2515.cxx Interesting
     * A simple transmit and receive example.
     * @snippet mcp2515-txrx.cxx Interesting
     */
    class MCP2515 {
    public:

        /**
         * MCP2515 constructor.
         *
         * @param bus spi bus to use
         * @param csPin The GPIO pin to use for Chip Select (CS).  Pass
         * pass -1 if your CS is handled automatically by your SPI
         * implementation (Edison).
         */
        MCP2515(int bus, int csPin);

        /**
         * MCP2515 Destructor
         */
        ~MCP2515();

        /**
         * Reset the device.
         *
         */
        void reset();

        /**
         * Set the operating mode of the device.  After initialization,
         * the device mode will be set to NORMAL.  Note that some
         * operations require the device to be placed into CONFIG mode.
         * This function will wait until the selected mode has been
         * entered.
         *
         * @param opmode One of the MCP2515_OPMODE_T values.
         */
        void setOpmode(MCP2515_OPMODE_T opmode);

        /**
         * Set the baud rate of the CAN bus.  All devices on a given
         * CAN bus must be operating at the correct speed.  The device
         * must be switched into CONFIG mode bofore this function will
         * have any effect.  On initialization, the default CAN bus
         * speed will be set to 50Kbps.
         *
         * @param speed One of the MCP2515_SPEED_T values.
         */
        void setSpeed(MCP2515_SPEED_T speed);

        /**
         * Load a transmit buffer with the id, payload, and other
         * information.  This function does not actually transmit the
         * buffer.  There are 3 TX buffers available.  The buffer must be
         * free (ie: not awaiting transmit) before it can be loaded.  Once
         * a TX buffer has been successfully loaded, it can be transmitted
         * with the TransmitBuffer() method.
         *
         * @param bufnum The buffer to load.  One of the
         * MCP2515_TX_BUFFER_T values.
         * @param id The integer representation of the CAN bus ID.
         * @param ext True if the ID is an extended identifier, false otherwise.
         * @param rtr True if this is a Remote Transfer Request, false
         * otherwise.
         * @param payload A string containing the payload bytes.
         * Maximum length is 8.
         */
        void loadTXBuffer(MCP2515_TX_BUFFER_T bufnum,
                          int id, bool ext, bool rtr,
                          std::string payload);

        /**
         * Transmit a buffer already loaded by loadTXBuffer().
         *
         * @param bufnum The buffer to transmit.  One of the
         * MCP2515_TX_BUFFER_T values.
         * @param wait True if the function should wait until transmission
         * is complete before returning, false otherwise.
         */
        void transmitBuffer(MCP2515_TX_BUFFER_T bufnum, bool wait);

        /**
         * Determine whether a TX buffer is available for use or not. A TX
         * buffer is unavailable if a transmit request is pending on it,
         * but transmission has not yet completed.
         *
         * @param bufnum The buffer to check. One of the
         * MCP2515_TX_BUFFER_T values.
         * @return True if the buffer is availabe, false otherwise.
         */
        bool isTXBufferFree(MCP2515_TX_BUFFER_T bufnum);

        /**
         * Find a free TX buffer and return it.
         *
         * @return One of the MCP2515_TX_BUFFER_T values.  If no buffers
         * are available, MCP2515_TX_NO_BUFFERS will be returned.
         */
        MCP2515_TX_BUFFER_T findFreeTXBuffer();

        /**
         * Set the transmit priority of a TX buffer.  Higher priority
         * buffers are always transmitted before lower priority buffers.
         * This function can be called on a buffer at any time prior to
         * actual transmission.
         *
         * @param bufnum The buffer to set priority for.  One of the
         * MCP2515_TX_BUFFER_T values.
         * @param priority The priority to set for the buffer.  One of the
         * MCP2515_TXP_T values.
         */
        void setTXBufferPriority(MCP2515_TX_BUFFER_T bufnum,
                                 MCP2515_TXP_T priority);

        /**
         * Abort a transmission that has been queued, but not yet
         * completed.  This will also free up the TX buffer for future
         * use.  Note, if you abort a transmission, but transmission has
         * already started, this call will have no effect, and the buffer
         * will complete transmission.
         *
         * @param bufnum The buffer to abort.  One of the
         * MCP2515_TX_BUFFER_T values.
         */
        void abortTX(MCP2515_TX_BUFFER_T bufnum);

        /**
         * Set the mode for an RX buffer.  The mode specifies, at a high
         * level, what packets should be captured from the bus and placed
         * into an RX buffer.  See the datasheet for details, but the
         * default, MCP2515_RXMODE_ANY_FILTER, should be sufficient in
         * most cases.  It is also possible to use this to restrict the
         * types of CAN ids accepted (extended only, standard only) as
         * well as a debug ANY_NOFILTER mode.
         *
         * @param bufnum The buffer to set the mode for.  One of the
         * MCP2515_RX_BUFFER_T values.
         * @param rxm The mode to set.  One of the MCP2515_RXMODE_T values.
         */
        void setRXBufferMode(MCP2515_RX_BUFFER_T bufnum,
                             MCP2515_RXMODE_T rxm);

        /**
         * Return a bitmask indicating which of the 2 RX buffers have
         * packets waiting in them.  This can be 0 (no packets), 1(RXB0),
         * 2(RXB1), or 3 (RXB0 and RXB1).  This information is retrieved
         * using the MCP2515_CMD_RX_STATUS command.
         *
         * @return A bitmask indicating which RX buffers (if any) have
         * packets in them.  One of the MCP2515_RXMSG_T values.
         */
        MCP2515_RXMSG_T rxStatusMsgs();

        /**
         * Return the message type present in one of the RX buffers.  RXB0
         * has the highest priority, so if both RX buffers are full, this
         * function will only return data for the packet in RXB0.  This
         * information is retrieved using the MCP2515_CMD_RX_STATUS
         * command.
         *
         * @return One of the MCP2515_MSGTYPE_T values.
         */
        MCP2515_MSGTYPE_T rxStatusMsgType();

        /**
         * Return the filter that matched an RX buffer.  RXB0 has the
         * highest priority, so if both RX buffers are full, this function
         * will only return data for the packet in RXB0.  This information
         * is retrieved using the MCP2515_CMD_RX_STATUS command.
         *
         * @return One of the MCP2515_FILTERMATCH_T values.
         */
        MCP2515_FILTERMATCH_T rxStatusFiltermatch();

        /**
         * This function retrieves a message from the specified RX
         * buffer.  The message (MCP2515_MSG_T) contains all of the
         * data in the packet, including id, rtr, ext, payload and
         * payload length.  In addition, after retrieving the message,
         * the RX buffer is freed to receive further data from the CAN
         * bus.  The message is stored within the class.
         *
         * @param bufnum The buffer to retrieve.  One of the
         * MCP2515_RX_BUFFER_T values.
         */
        upm_result_t getRXMsg(MCP2515_RX_BUFFER_T bufnum);

        /**
         * This is a utility function prints the current (last
         * received) messages decoded contents.  This is of
         * primary importance for debugging, and to simplify the
         * examples somewhat.
         *
         * The output will look similar to:
         *
         * id 00000000 ext 0 rtr 0 filt 0 len 8
         *          payload: 0xc8 0x01 0x02 0x03 0x04 0x05 0x06 0x07
         *
         */
        void printMsg();


        /**
         * This method returns the id of a received message.  It will
         * only be valid after a successful completion of rxGetMsg().
         *
         * @return ID of the last received message.
         */
        int msgGetID()
        {
            return m_message.id;
        }

        /**
         * This method returns the RTR flag of a received message.  It will
         * only be valid after a successful completion of rxGetMsg().
         *
         * @return True if the message has the RTR flag set, false otherwise.
         */
        bool msgGetRTR()
        {
            return m_message.rtr;
        }

        /**
         * This method returns the EXT (extended ID) flag of a
         * received message.  It will only be valid after a successful
         * completion of rxGetMsg().
         *
         * @return True if the message has an extended ID, false otherwise.
         */
        bool msgGetEXT()
        {
            return m_message.ext;
        }

        /**
         * This method returns the filter number that caused the
         * message to be stored in the RX buffer.  It will only be
         * valid after a successful completion of rxGetMsg().
         *
         * @return The filter number that was matched.
         */
        int msgGetFilterNum()
        {
            return m_message.filter_num;
        }

        /**
         * This method returns the length of the payload of the RX
         * buffer.  It will only be valid after a successful
         * completion of rxGetMsg().
         *
         * @return Length of the payload in bytes, max 8.
         */
        int msgGetPayloadLen()
        {
            return m_message.len;
        }

        /**
         * This method returns the contents of the payload in the last
         * received message.  It will only be valid after a successful
         * completion of rxGetMsg().
         *
         * @return String containing the payload.
         */
        std::string msgGetPayload()
        {
            return std::string((char *)m_message.pkt.data, m_message.len);
        }


        /**
         * Installs an interrupt service routine (ISR) to be called when
         * an interrupt occurs.
         *
         * @param pin GPIO pin to use as the interrupt pin.
         * @param isr Pointer to a function to be called on interrupt.
         * @param arg Pointer to an object to be supplied as an
         * argument to the ISR.
         */
        void installISR(int pin, void (*isr)(void *), void *arg);


        /**
         * Uninstalls the previously installed ISR
         *
         * @param dev Device context.
         */
        void uninstallISR();

        /**
         * Set the interrupt enables register.
         *
         * @param enables A bitmask of interrupts to enable from
         * MCP2515_CANINT_BITS_T.
         */
        void setIntrEnables(uint8_t enables);

        /**
         * Retrieve the interrupt flags register.
         *
         * @return A bitmask that will be filled with values from
         * MCP2515_CANINT_BITS_T, indicating which interrupt flags are
         * set.
         */
        uint8_t getIntrFlags();

        /**
         * This function allows you to set specific interrupt flags.  If
         * the corresponding interrupt enable is set, an interrupt will be
         * generated.
         *
         * @param flags A bitmask of interrupt flags to set, from
         * MCP2515_CANINT_BITS_T values.
         */
        void setIntrFlags(uint8_t flags);

        /**
         * This function allows you to clear specific interrupt flags.
         * See the datasheet.  Some flags cannot be cleared until the
         * underlying cause has been corrected.
         *
         * @param flags A bitmask of interrupt flags to clear, from
         * MCP2515_CANINT_BITS_T values.
         */
        void clearIntrFlags(uint8_t flags);

        /**
         * Retrieve the error flags register
         *
         * @return A bitmask that will be filled with values from
         * MCP2515_EFLG_BITS_T, indicating which error flags are set.
         */
        uint8_t getErrorFlags();

        /**
         * Clear error flags.  Note, some flags cannot be cleared
         * until the underlying issues is resolved.
         *
         * @return A bitmask of values from MCP2515_EFLG_BITS_T,
         * indicating which error flags to clear.
         */
        void clearErrorFlags(uint8_t flags);

        /**
         * This function allows you to set one of the 6 RX filters
         * available.  Filters 0 and 1 are for RXB0 only, while filters
         * 2-5 are for RXB1.  See the datasheet for details on how these
         * filters (along with the masks) are used to select candidate CAN
         * bus data for retrieval from the CAN bus.
         *
         * These can only be set when the device is in CONFIG mode.
         *
         * @param filter One of the 6 MCP2515_RX_FILTER_T values.
         * @param ext True if the id is extended, false for standard.
         * @param id Integer representation of a CAN bus ID.
         */
        void setFilter(MCP2515_RX_FILTER_T filter, bool ext, int id);

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
         * @param mask One of the 2 MCP2515_RX_MASK_T values.
         * @param ext True if the id is extended, false for standard.
         * @param id Integer representation of a CAN bus ID.
         */
        void setMask(MCP2515_RX_MASK_T mask, bool ext, int id);


    protected:
        mcp2515_context m_mcp2515;

        // We operate only on this message (for received messages) to
        // simplify SWIG accesses.
        MCP2515_MSG_T m_message;

        /**
         * Perform a bus read.  This function is exposed here for those
         * users wishing to perform their own low level accesses.  This is
         * a low level function, and should not be used unless you know
         * what you are doing.
         *
         * @param cmd The command to send.
         * @param args String containing arguments, or empty for no arguments.
         * @param datalen The length of the data to read.
         * @return A string containing the data.
         */
        std::string busRead(uint8_t cmd, std::string args, int datalen);

        /**
         * Perform a bus write.  This function is exposed here for those
         * users wishing to perform their own low level accesses.  This is
         * a low level function, and should not be used unless you know
         * what you are doing.
         *
         * @param cmd The command to send.
         * @param data A string containing the data to write.
         * @param len The number of bytes to write.
         */
        void busWrite(uint8_t cmd, std::string data);

        /**
         * Write to a specific register.  This function is exposed here
         * for those users wishing to perform their own low level
         * accesses.  This is a low level function, and should not be used
         * unless you know what you are doing.
         *
         * @param reg The register to write to.
         * @param value The byte to write.
         */
        void writeReg(uint8_t reg, uint8_t value);

        /**
         * Write to multiple consecutive registers.  This function is
         * exposed here for those users wishing to perform their own low
         * level accesses.  This is a low level function, and should not
         * be used unless you know what you are doing.
         *
         * @param reg The register to start writing to.
         * @param buffer A string containing data to write.
         */
        void writeRegs(uint8_t reg, std::string buffer);

        /**
         * Read a register.  This function is exposed here for those users
         * wishing to perform their own low level accesses.  This is a low
         * level function, and should not be used unless you know what you
         * are doing.
         *
         * @param reg The register to read.
         * @return The register contents.
         */
        uint8_t readReg(uint8_t reg);

        /**
         * Perform a bit modify operation on a register.  Only certain
         * registers support this method of access - check the datasheet.
         * This function is exposed here for those users wishing to
         * perform their own low level accesses.  This is a low level
         * function, and should not be used unless you know what you are
         * doing.
         *
         * @param addr The address of the register to access.
         * @param mask A bitmask used to mask off value bits.
         * @param value The value to write (bits masked by mask).
         */
        void bitModify(uint8_t addr, uint8_t mask, uint8_t value);

    private:
        /* Disable implicit copy and assignment operators */
        MCP2515(const MCP2515&) = delete;
        MCP2515 &operator=(const MCP2515&) = delete;
    };
}
