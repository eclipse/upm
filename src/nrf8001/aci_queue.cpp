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

 /** @file
@brief Implementation of a circular queue for ACI data
*/

#include "hal_aci_tl.h"
#include "aci_queue.h"

void aci_queue_init(aci_queue_t *aci_q)
{
  uint8_t loop;

  // ble_assert(NULL != aci_q);

  aci_q->head = 0;
  aci_q->tail = 0;
  for(loop=0; loop<ACI_QUEUE_SIZE; loop++)
  {
    aci_q->aci_data[loop].buffer[0] = 0x00;
    aci_q->aci_data[loop].buffer[1] = 0x00;
  }
}

bool aci_queue_dequeue(aci_queue_t *aci_q, hal_aci_data_t *p_data)
{
  // ble_assert(NULL != aci_q);
  // ble_assert(NULL != p_data);

  if (aci_queue_is_empty(aci_q))
  {
    return false;
  }

  memcpy((uint8_t *)p_data, (uint8_t *)&(aci_q->aci_data[aci_q->head]), sizeof(hal_aci_data_t));
  aci_q->head = (aci_q->head + 1) % ACI_QUEUE_SIZE;

  return true;
}

bool aci_queue_dequeue_from_isr(aci_queue_t *aci_q, hal_aci_data_t *p_data)
{
  // ble_assert(NULL != aci_q);
  // ble_assert(NULL != p_data);

  if (aci_queue_is_empty_from_isr(aci_q))
  {
    return false;
  }

  memcpy((uint8_t *)p_data, (uint8_t *)&(aci_q->aci_data[aci_q->head]), sizeof(hal_aci_data_t));
  aci_q->head = (aci_q->head + 1) % ACI_QUEUE_SIZE;

  return true;
}

bool aci_queue_enqueue(aci_queue_t *aci_q, hal_aci_data_t *p_data)
{
  const uint8_t length = p_data->buffer[0];

  // ble_assert(NULL != aci_q);
  // ble_assert(NULL != p_data);

  if (aci_queue_is_full(aci_q))
  {
    return false;
  }

  aci_q->aci_data[aci_q->tail].status_byte = 0;
  memcpy((uint8_t *)&(aci_q->aci_data[aci_q->tail].buffer[0]), (uint8_t *)&p_data->buffer[0], length + 1);
  aci_q->tail = (aci_q->tail + 1) % ACI_QUEUE_SIZE;

  return true;
}

bool aci_queue_enqueue_from_isr(aci_queue_t *aci_q, hal_aci_data_t *p_data)
{
  const uint8_t length = p_data->buffer[0];

  // ble_assert(NULL != aci_q);
  // ble_assert(NULL != p_data);

  if (aci_queue_is_full_from_isr(aci_q))
  {
    return false;
  }

  aci_q->aci_data[aci_q->tail].status_byte = 0;
  memcpy((uint8_t *)&(aci_q->aci_data[aci_q->tail].buffer[0]), (uint8_t *)&p_data->buffer[0], length + 1);
  aci_q->tail = (aci_q->tail + 1) % ACI_QUEUE_SIZE;

  return true;
}

bool aci_queue_is_empty(aci_queue_t *aci_q)
{
  bool state = false;

  // ble_assert(NULL != aci_q);

  //Critical section
  // noInterrupts();
  if (aci_q->head == aci_q->tail)
  {
    state = true;
  }
  // interrupts();

  return state;
}

bool aci_queue_is_empty_from_isr(aci_queue_t *aci_q)
{
  // ble_assert(NULL != aci_q);

  return aci_q->head == aci_q->tail;
}

bool aci_queue_is_full(aci_queue_t *aci_q)
{
  uint8_t next;
  bool state;

  // ble_assert(NULL != aci_q);

  //This should be done in a critical section
  // noInterrupts();
  next = (aci_q->tail + 1) % ACI_QUEUE_SIZE;

  if (next == aci_q->head)
  {
    state = true;
  }
  else
  {
    state = false;
  }

  // interrupts();
  //end

  return state;
}

bool aci_queue_is_full_from_isr(aci_queue_t *aci_q)
{
  const uint8_t next = (aci_q->tail + 1) % ACI_QUEUE_SIZE;

  // ble_assert(NULL != aci_q);

  return next == aci_q->head;
}

bool aci_queue_peek(aci_queue_t *aci_q, hal_aci_data_t *p_data)
{
  // ble_assert(NULL != aci_q);
  // ble_assert(NULL != p_data);

  if (aci_queue_is_empty(aci_q))
  {
    return false;
  }

  memcpy((uint8_t *)p_data, (uint8_t *)&(aci_q->aci_data[aci_q->head]), sizeof(hal_aci_data_t));

  return true;
}

bool aci_queue_peek_from_isr(aci_queue_t *aci_q, hal_aci_data_t *p_data)
{
  // ble_assert(NULL != aci_q);
  // ble_assert(NULL != p_data);

  if (aci_queue_is_empty_from_isr(aci_q))
  {
    return false;
  }

  memcpy((uint8_t *)p_data, (uint8_t *)&(aci_q->aci_data[aci_q->head]), sizeof(hal_aci_data_t));

  return true;
}
