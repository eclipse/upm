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
 * @brief Definitions for the acilib interfaces
 */

#ifndef _acilib_DEFS_H_
#define _acilib_DEFS_H_

#define ACIL_DECODE_EVT_GET_LENGTH(buffer_in) (*(buffer_in + OFFSET_ACI_EVT_T_LEN))

#define ACIL_DECODE_EVT_GET_OPCODE(buffer_in) (*(buffer_in + OFFSET_ACI_EVT_T_EVT_OPCODE))

#endif /* _acilib_DEFS_H_ */
