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


#include "aci.h"
#include "hal_aci_tl.h"
#include <lib_aci.h>
#include "aci_setup.h"


// aci_struct that will contain
// total initial credits
// current credit
// current state of the aci (setup/standby/active/sleep)
// open remote pipe pending
// close remote pipe pending
// Current pipe available bitmap
// Current pipe closed bitmap
// Current connection interval, slave latency and link supervision timeout
// Current State of the the GATT client (Service Discovery status)


// hal_aci_data_t msg_to_send;
extern hal_aci_data_t msg_to_send;


/**************************************************************************                */
/* Utility function to fill the the ACI command queue                                      */
/* aci_stat               Pointer to the ACI state                                         */
/* num_cmd_offset(in/out) Offset in the Setup message array to start from                  */
/*                        offset is updated to the new index after the queue is filled     */
/*                        or the last message us placed in the queue                       */
/* Returns                true if at least one message was transferred                     */
/***************************************************************************/
bool aci_setup_fill(aci_state_t *aci_stat, uint8_t *num_cmd_offset)
{
  bool ret_val = false;

  while (*num_cmd_offset < aci_stat->aci_setup_info.num_setup_msgs)
  {
    //Board dependent defines
    /*#if defined (__AVR__)
        //For Arduino copy the setup ACI message from Flash to RAM.
        memcpy_P(&msg_to_send, &(aci_stat->aci_setup_info.setup_msgs[*num_cmd_offset]),
                  pgm_read_byte_near(&(aci_stat->aci_setup_info.setup_msgs[*num_cmd_offset].buffer[0]))+2);
    #elif defined(__PIC32MX__)
        //In ChipKit we store the setup messages in RAM
        //Add 2 bytes to the length byte for status byte, length for the total number of bytes
        memcpy(&msg_to_send, &(aci_stat->aci_setup_info.setup_msgs[*num_cmd_offset]),
                  (aci_stat->aci_setup_info.setup_msgs[*num_cmd_offset].buffer[0]+2));
    #endif*/

    memcpy(&msg_to_send, &(aci_stat->aci_setup_info.setup_msgs[*num_cmd_offset]),
                  (aci_stat->aci_setup_info.setup_msgs[*num_cmd_offset].buffer[0]+2));

    //Put the Setup ACI message in the command queue
    if (!hal_aci_tl_send(&msg_to_send))
    {
      //ACI Command Queue is full
      // *num_cmd_offset is now pointing to the index of the Setup command that did not get sent
      return ret_val;
   }

    ret_val = true;

    (*num_cmd_offset)++;
  }

  return ret_val;
}

uint8_t do_aci_setup(aci_state_t *aci_stat)
{
  uint8_t setup_offset         = 0;
  uint32_t i                   = 0x0000;
  aci_evt_t * aci_evt          = NULL;
  aci_status_code_t cmd_status = ACI_STATUS_ERROR_CRC_MISMATCH;

  /*
  We are using the same buffer since we are copying the contents of the buffer
  when queuing and immediately processing the buffer when receiving
  */
  hal_aci_evt_t  *aci_data = (hal_aci_evt_t *)&msg_to_send;

  /* Messages in the outgoing queue must be handled before the Setup routine can run.
   * If it is non-empty we return. The user should then process the messages before calling
   * do_aci_setup() again.
   */
  if (!lib_aci_command_queue_empty())
  {
    return SETUP_FAIL_COMMAND_QUEUE_NOT_EMPTY;
  }

  /* If there are events pending from the device that are not relevant to setup, we return false
   * so that the user can handle them. At this point we don't care what the event is,
   * as any event is an error.
   */
  if (lib_aci_event_peek(aci_data))
  {
    return SETUP_FAIL_EVENT_QUEUE_NOT_EMPTY;
  }

  /* Fill the ACI command queue with as many Setup messages as it will hold. */
  aci_setup_fill(aci_stat, &setup_offset);

  while (cmd_status != ACI_STATUS_TRANSACTION_COMPLETE)
  {
    /* This counter is used to ensure that this function does not loop forever. When the device
     * returns a valid response, we reset the counter.
     */
    if (i++ > 0xFFFFE)
    {
      return SETUP_FAIL_TIMEOUT;
    }

    if (lib_aci_event_peek(aci_data))
    {
      aci_evt = &(aci_data->evt);

      if (ACI_EVT_CMD_RSP != aci_evt->evt_opcode)
      {
        //Receiving something other than a Command Response Event is an error.
        return SETUP_FAIL_NOT_COMMAND_RESPONSE;
      }

      cmd_status = (aci_status_code_t) aci_evt->params.cmd_rsp.cmd_status;
      switch (cmd_status)
      {
        case ACI_STATUS_TRANSACTION_CONTINUE:
          //As the device is responding, reset guard counter
          i = 0;

          /* As the device has processed the Setup messages we put in the command queue earlier,
           * we can proceed to fill the queue with new messages
           */
          aci_setup_fill(aci_stat, &setup_offset);
          break;

        case ACI_STATUS_TRANSACTION_COMPLETE:
          //Break out of the while loop when this status code appears
          break;

        default:
          //An event with any other status code should be handled by the application
          return SETUP_FAIL_NOT_SETUP_EVENT;
      }

      /* If we haven't returned at this point, the event was either ACI_STATUS_TRANSACTION_CONTINUE
       * or ACI_STATUS_TRANSACTION_COMPLETE. We don't need the event itself, so we simply
       * remove it from the queue.
       */
       lib_aci_event_get (aci_stat, aci_data);
    }
  }

  return SETUP_SUCCESS;
}


