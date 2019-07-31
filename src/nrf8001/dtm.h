/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file
 *
 * @ingroup aci-lib
 *
 * @brief Internal prototype for acilib module.
 */

#ifndef DTM_H__
#define DTM_H__

/** @brief DTM command codes (upper two bits in the DTM command), use a bitwise OR with the frequency N = 0x00 - 0x27: N = (F-2402)/2 Frequency Range 2402 MHz
to 2480 MHz*/
#define DTM_LE_CMD_RESET            0x00 /**< DTM reset command code */
#define DTM_LE_CMD_RECEIVER_TEST    0x40 /**< DTM receiver test command code */
#define DTM_LE_CMD_TRANSMITTER_TEST 0x80 /**< DTM transmitter test command code */
#define DTM_LE_CMD_TEST_END         0xC0 /**< DTM test end command code */


/** @brief Defined packet types for DTM */
#define DTM_LE_PKT_PRBS9      0x00       /**< Bit pattern PRBS9.    */
#define DTM_LE_PKT_0X0F       0x01       /**< Bit pattern 11110000 (LSB is the leftmost bit). */
#define DTM_LE_PKT_0X55       0x02       /**< Bit pattern 10101010 (LSB is the leftmost bit). */
#define DTM_LE_PKT_VENDOR     0x03       /**< Vendor specific. Nordic: continous carrier test */

/** @brief Defined bit fields for DTM responses. */
#define LE_PACKET_REPORTING_EVENT_MSB_BIT   0x80 /**< Bit field for most significant bit */
#define LE_TEST_STATUS_EVENT_LSB_BIT        0x01 /**< Bit field for least significant bit */

/** @brief DTM response types. */
#define LE_TEST_STATUS_EVENT                0x00 /**< DTM event response */
#define LE_TEST_PACKET_REPORT_EVENT         0x80 /**< DTM packet report reponse */

/** @brief DTM return values. */
#define LE_TEST_STATUS_SUCCESS              0x00 /**< DTM success return value */
#define LE_TEST_STATUS_FAILURE              0x01 /**< DTM failure return value */



#endif //DTM_H__

/** @} */
