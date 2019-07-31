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
 * @brief Defines for the different Bluetooth low energy boards
 * @ingroup nrf8001
 */

#ifndef _BLE_BOARDS_H_
#define _BLE_BOARDS_H_

#define BOARD_DEFAULT               0 /**< Use this if you do not know the board you are using or you are creating a new one */
#define REDBEARLAB_SHIELD_V1_1      1 /**< Redbearlab Bluetooth low energy shield v1.1 */
#define REDBEARLAB_SHIELD_V2012_07  1 /**< Identical to Redbearlab v1.1 shield */
#define REDBEARLAB_SHIELD_V2        0 /**< Redbearlab Bluetooth low energy shield v2.x - No special handling required for pin reset same as default */

#endif
