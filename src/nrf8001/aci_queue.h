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
