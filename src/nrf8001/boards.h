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
