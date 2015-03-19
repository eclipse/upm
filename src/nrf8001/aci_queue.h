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
 * @brief Interface for buffer.
 * @ingroup aci
 */

/**
@{
*/

#ifndef ACI_QUEUE_H__
#define ACI_QUEUE_H__

#include "aci.h"
#include "hal_aci_tl.h"

/***********************************************************************    */
/* The ACI_QUEUE_SIZE determines the memory usage of the system.            */
/* Successfully tested to a ACI_QUEUE_SIZE of 4 (interrupt) and 4 (polling) */
/***********************************************************************    */
#define ACI_QUEUE_SIZE  4

/** Data type for queue of data packets to send/receive from radio.
 *
 *  A FIFO queue is maintained for packets. New packets are added (enqueued)
 *  at the tail and taken (dequeued) from the head. The head variable is the
 *  index of the next packet to dequeue while the tail variable is the index of
 *  where the next packet should be queued.
 */

typedef struct {
    hal_aci_data_t           aci_data[ACI_QUEUE_SIZE];
    uint8_t                  head;
    uint8_t                  tail;
} aci_queue_t;

void aci_queue_init(aci_queue_t *aci_q);

bool aci_queue_dequeue(aci_queue_t *aci_q, hal_aci_data_t *p_data);
bool aci_queue_dequeue_from_isr(aci_queue_t *aci_q, hal_aci_data_t *p_data);

bool aci_queue_enqueue(aci_queue_t *aci_q, hal_aci_data_t *p_data);
bool aci_queue_enqueue_from_isr(aci_queue_t *aci_q, hal_aci_data_t *p_data);

bool aci_queue_is_empty(aci_queue_t *aci_q);
bool aci_queue_is_empty_from_isr(aci_queue_t *aci_q);

bool aci_queue_is_full(aci_queue_t *aci_q);
bool aci_queue_is_full_from_isr(aci_queue_t *aci_q);

bool aci_queue_peek(aci_queue_t *aci_q, hal_aci_data_t *p_data);
bool aci_queue_peek_from_isr(aci_queue_t *aci_q, hal_aci_data_t *p_data);

#endif /* ACI_QUEUE_H__ */
/** @} */
