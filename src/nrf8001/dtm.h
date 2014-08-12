/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file
 *
 * @ingroup group_acilib
 *
 * @brief Internal prototype for acilib module.
 */

#ifndef DTM_H__
#define DTM_H__

/** @brief DTM command codes (upper two bits in the DTM command), use a bitwise OR with the frequency N = 0x00 - 0x27: N = (F-2402)/2 Frequency Range 2402 MHz
to 2480 MHz*/
#define DTM_LE_CMD_RESET            0x00
#define DTM_LE_CMD_RECEIVER_TEST    0x40
#define DTM_LE_CMD_TRANSMITTER_TEST 0x80
#define DTM_LE_CMD_TEST_END         0xC0


/** @brief Defined packet types for DTM */
#define DTM_LE_PKT_PRBS9      0x00       /**< Bit pattern PRBS9.    */
#define DTM_LE_PKT_0X0F       0x01       /**< Bit pattern 11110000 (LSB is the leftmost bit). */
#define DTM_LE_PKT_0X55       0x02       /**< Bit pattern 10101010 (LSB is the leftmost bit). */
#define DTM_LE_PKT_VENDOR     0x03       /**< Vendor specific. Nordic: continous carrier test */

/** @brief Defined bit fields for DTM responses. */
#define LE_PACKET_REPORTING_EVENT_MSB_BIT   0x80
#define LE_TEST_STATUS_EVENT_LSB_BIT        0x01

/** @brief DTM response types. */
#define LE_TEST_STATUS_EVENT                0x00
#define LE_TEST_PACKET_REPORT_EVENT         0x80

/** @brief DTM return values. */
#define LE_TEST_STATUS_SUCCESS              0x00
#define LE_TEST_STATUS_FAILURE              0x01



#endif //DTM_H__

/** @} */
