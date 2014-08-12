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
  @brief Implementation of the ACI library.
 */

#include "hal_platform.h"
#include "aci.h"
#include "aci_cmds.h"
#include "aci_evts.h"
#include "aci_protocol_defines.h"
#include "acilib_defs.h"
#include "acilib_if.h"
#include "hal_aci_tl.h"
#include "aci_queue.h"
#include "lib_aci.h"


#define LIB_ACI_DEFAULT_CREDIT_NUMBER   1

/*
Global additionally used used in aci_setup
*/
hal_aci_data_t  msg_to_send;


static services_pipe_type_mapping_t * p_services_pipe_type_map;
static hal_aci_data_t *               p_setup_msgs;




static bool is_request_operation_pending;
static bool is_indicate_operation_pending;
static bool is_open_remote_pipe_pending;
static bool is_close_remote_pipe_pending;

static uint8_t request_operation_pipe = 0;
static uint8_t indicate_operation_pipe = 0;


// The following structure (aci_cmd_params_open_adv_pipe) will be used to store the complete command
// including the pipes to be opened.
static aci_cmd_params_open_adv_pipe_t aci_cmd_params_open_adv_pipe;



extern aci_queue_t    aci_rx_q;
extern aci_queue_t    aci_tx_q;

bool lib_aci_is_pipe_available(aci_state_t *aci_stat, uint8_t pipe)
{
  uint8_t byte_idx;

  byte_idx = pipe / 8;
  if (aci_stat->pipes_open_bitmap[byte_idx] & (0x01 << (pipe % 8)))
  {
    return(true);
  }
  return(false);
}


bool lib_aci_is_pipe_closed(aci_state_t *aci_stat, uint8_t pipe)
{
  uint8_t byte_idx;

  byte_idx = pipe / 8;
  if (aci_stat->pipes_closed_bitmap[byte_idx] & (0x01 << (pipe % 8)))
  {
    return(true);
  }
  return(false);
}


bool lib_aci_is_discovery_finished(aci_state_t *aci_stat)
{
  return(aci_stat->pipes_open_bitmap[0]&0x01);
}

void lib_aci_board_init(aci_state_t *aci_stat)
{
    hal_aci_evt_t *aci_data = NULL;
    aci_data = (hal_aci_evt_t *)&msg_to_send;

    if (REDBEARLAB_SHIELD_V1_1 == aci_stat->aci_pins.board_name)
    {
      /*
      The Bluetooth low energy Arduino shield v1.1 requires about 100ms to reset.
      This is not required for the nRF2740, nRF2741 modules
      */
      usleep(100000);

      /*
      Send the soft reset command to the nRF8001 to get the nRF8001 to a known state.
      */
      lib_aci_radio_reset();

      while (1)
      {
        /*Wait for the command response of the radio reset command.
        as the nRF8001 will be in either SETUP or STANDBY after the ACI Reset Radio is processed
        */


        if (true == lib_aci_event_get(aci_stat, aci_data))
        {
          aci_evt_t * aci_evt;
          aci_evt = &(aci_data->evt);

          if (ACI_EVT_CMD_RSP == aci_evt->evt_opcode)
          {
                if (ACI_STATUS_ERROR_DEVICE_STATE_INVALID == aci_evt->params.cmd_rsp.cmd_status) //in SETUP
                {
                    //Inject a Device Started Event Setup to the ACI Event Queue
                    msg_to_send.buffer[0] = 4;    //Length
                    msg_to_send.buffer[1] = 0x81; //Device Started Event
                    msg_to_send.buffer[2] = 0x02; //Setup
                    msg_to_send.buffer[3] = 0;    //Hardware Error -> None
                    msg_to_send.buffer[4] = 2;    //Data Credit Available
                    aci_queue_enqueue(&aci_rx_q, &msg_to_send);
                }
                else if (ACI_STATUS_SUCCESS == aci_evt->params.cmd_rsp.cmd_status) //We are now in STANDBY
                {
                    //Inject a Device Started Event Standby to the ACI Event Queue
                    msg_to_send.buffer[0] = 4;    //Length
                    msg_to_send.buffer[1] = 0x81; //Device Started Event
                    msg_to_send.buffer[2] = 0x03; //Standby
                    msg_to_send.buffer[3] = 0;    //Hardware Error -> None
                    msg_to_send.buffer[4] = 2;    //Data Credit Available
                    aci_queue_enqueue(&aci_rx_q, &msg_to_send);
                }
                else if (ACI_STATUS_ERROR_CMD_UNKNOWN == aci_evt->params.cmd_rsp.cmd_status) //We are now in TEST
                {
                    //Inject a Device Started Event Test to the ACI Event Queue
                    msg_to_send.buffer[0] = 4;    //Length
                    msg_to_send.buffer[1] = 0x81; //Device Started Event
                    msg_to_send.buffer[2] = 0x01; //Test
                    msg_to_send.buffer[3] = 0;    //Hardware Error -> None
                    msg_to_send.buffer[4] = 0;    //Data Credit Available
                    aci_queue_enqueue(&aci_rx_q, &msg_to_send);
                }

                printf ("BREAK\n");
                //Break out of the while loop
                break;
          }
          else
          {
            //Serial.println(F("Discard any other ACI Events"));
          }

        }
      }
    }
}


void lib_aci_init(aci_state_t *aci_stat, bool debug)
{
    uint8_t i;

    for (i = 0; i < PIPES_ARRAY_SIZE; i++) {
        aci_stat->pipes_open_bitmap[i]          = 0;
        aci_stat->pipes_closed_bitmap[i]        = 0;
        aci_cmd_params_open_adv_pipe.pipes[i]   = 0;
    }

    is_request_operation_pending     = false;
    is_indicate_operation_pending    = false;
    is_open_remote_pipe_pending      = false;
    is_close_remote_pipe_pending     = false;

    request_operation_pipe           = 0;
    indicate_operation_pipe          = 0;

    p_services_pipe_type_map = aci_stat->aci_setup_info.services_pipe_type_mapping;
    p_setup_msgs             = aci_stat->aci_setup_info.setup_msgs;

    hal_aci_tl_init (&aci_stat->aci_pins, debug);
    lib_aci_board_init (aci_stat);
}


uint8_t lib_aci_get_nb_available_credits(aci_state_t *aci_stat)
{
  return aci_stat->data_credit_available;
}

uint16_t lib_aci_get_cx_interval_ms(aci_state_t *aci_stat)
{
  uint32_t cx_rf_interval_ms_32bits;
  uint16_t cx_rf_interval_ms;

  cx_rf_interval_ms_32bits  = aci_stat->connection_interval;
  cx_rf_interval_ms_32bits *= 125;                      // the connection interval is given in multiples of 0.125 milliseconds
  cx_rf_interval_ms         = cx_rf_interval_ms_32bits / 100;

  return cx_rf_interval_ms;
}


uint16_t lib_aci_get_cx_interval(aci_state_t *aci_stat)
{
  return aci_stat->connection_interval;
}


uint16_t lib_aci_get_slave_latency(aci_state_t *aci_stat)
{
  return aci_stat->slave_latency;
}


bool lib_aci_set_app_latency(uint16_t latency, aci_app_latency_mode_t latency_mode)
{
  aci_cmd_params_set_app_latency_t aci_set_app_latency;

  aci_set_app_latency.mode    = latency_mode;
  aci_set_app_latency.latency = latency;
  acil_encode_cmd_set_app_latency(&(msg_to_send.buffer[0]), &aci_set_app_latency);

  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_test(aci_test_mode_change_t enter_exit_test_mode)
{
  aci_cmd_params_test_t aci_cmd_params_test;
  aci_cmd_params_test.test_mode_change = enter_exit_test_mode;
  acil_encode_cmd_set_test_mode(&(msg_to_send.buffer[0]), &aci_cmd_params_test);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_sleep()
{
  acil_encode_cmd_sleep(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_radio_reset()
{
  acil_encode_baseband_reset(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_direct_connect()
{
  acil_encode_direct_connect(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_device_version()
{
  acil_encode_cmd_get_device_version(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_set_local_data(aci_state_t *aci_stat, uint8_t pipe, uint8_t *p_value, uint8_t size)
{
  aci_cmd_params_set_local_data_t aci_cmd_params_set_local_data;

  if ((p_services_pipe_type_map[pipe-1].location != ACI_STORE_LOCAL)
      ||
      (size > ACI_PIPE_TX_DATA_MAX_LEN))
  {
    return false;
  }

  aci_cmd_params_set_local_data.tx_data.pipe_number = pipe;
  memcpy(&(aci_cmd_params_set_local_data.tx_data.aci_data[0]), p_value, size);
  acil_encode_cmd_set_local_data(&(msg_to_send.buffer[0]), &aci_cmd_params_set_local_data, size);
  return hal_aci_tl_send(&msg_to_send);
}

bool lib_aci_connect(uint16_t run_timeout, uint16_t adv_interval)
{
  aci_cmd_params_connect_t aci_cmd_params_connect;
  aci_cmd_params_connect.timeout      = run_timeout;
  aci_cmd_params_connect.adv_interval = adv_interval;
  acil_encode_cmd_connect(&(msg_to_send.buffer[0]), &aci_cmd_params_connect);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_disconnect(aci_state_t *aci_stat, aci_disconnect_reason_t reason)
{
  bool ret_val;
  uint8_t i;
  aci_cmd_params_disconnect_t aci_cmd_params_disconnect;
  aci_cmd_params_disconnect.reason = reason;
  acil_encode_cmd_disconnect(&(msg_to_send.buffer[0]), &aci_cmd_params_disconnect);
  ret_val = hal_aci_tl_send(&msg_to_send);
  // If we have actually sent the disconnect
  if (ret_val)
  {
    // Update pipes immediately so that while the disconnect is happening,
    // the application can't attempt sending another message
    // If the application sends another message before we updated this
    //    a ACI Pipe Error Event will be received from nRF8001
    for (i=0; i < PIPES_ARRAY_SIZE; i++)
    {
      aci_stat->pipes_open_bitmap[i] = 0;
      aci_stat->pipes_closed_bitmap[i] = 0;
    }
  }
  return ret_val;
}


bool lib_aci_bond(uint16_t run_timeout, uint16_t adv_interval)
{
  aci_cmd_params_bond_t aci_cmd_params_bond;
  aci_cmd_params_bond.timeout = run_timeout;
  aci_cmd_params_bond.adv_interval = adv_interval;
  acil_encode_cmd_bond(&(msg_to_send.buffer[0]), &aci_cmd_params_bond);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_wakeup()
{
  acil_encode_cmd_wakeup(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_set_tx_power(aci_device_output_power_t tx_power)
{
  aci_cmd_params_set_tx_power_t aci_cmd_params_set_tx_power;
  aci_cmd_params_set_tx_power.device_power = tx_power;
  acil_encode_cmd_set_radio_tx_power(&(msg_to_send.buffer[0]), &aci_cmd_params_set_tx_power);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_get_address()
{
  acil_encode_cmd_get_address(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_get_temperature()
{
  acil_encode_cmd_temparature(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_get_battery_level()
{
  acil_encode_cmd_battery_level(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_send_data(uint8_t pipe, uint8_t *p_value, uint8_t size)
{
  bool ret_val = false;
  aci_cmd_params_send_data_t aci_cmd_params_send_data;


  if(!((p_services_pipe_type_map[pipe-1].pipe_type == ACI_TX) ||
      (p_services_pipe_type_map[pipe-1].pipe_type == ACI_TX_ACK)))
  {
    return false;
  }

  if (size > ACI_PIPE_TX_DATA_MAX_LEN)
  {
    return false;
  }
  {
      aci_cmd_params_send_data.tx_data.pipe_number = pipe;
      memcpy(&(aci_cmd_params_send_data.tx_data.aci_data[0]), p_value, size);
      acil_encode_cmd_send_data(&(msg_to_send.buffer[0]), &aci_cmd_params_send_data, size);

      ret_val = hal_aci_tl_send(&msg_to_send);
  }
  return ret_val;
}


bool lib_aci_request_data(aci_state_t *aci_stat, uint8_t pipe)
{
  bool ret_val = false;
  aci_cmd_params_request_data_t aci_cmd_params_request_data;

  if(!((p_services_pipe_type_map[pipe-1].location == ACI_STORE_REMOTE)&&(p_services_pipe_type_map[pipe-1].pipe_type == ACI_RX_REQ)))
  {
    return false;
  }


  {

    {



      aci_cmd_params_request_data.pipe_number = pipe;
      acil_encode_cmd_request_data(&(msg_to_send.buffer[0]), &aci_cmd_params_request_data);

      ret_val = hal_aci_tl_send(&msg_to_send);
    }
  }
  return ret_val;
}


bool lib_aci_change_timing(uint16_t minimun_cx_interval, uint16_t maximum_cx_interval, uint16_t slave_latency, uint16_t timeout)
{
  aci_cmd_params_change_timing_t aci_cmd_params_change_timing;
  aci_cmd_params_change_timing.conn_params.min_conn_interval = minimun_cx_interval;
  aci_cmd_params_change_timing.conn_params.max_conn_interval = maximum_cx_interval;
  aci_cmd_params_change_timing.conn_params.slave_latency     = slave_latency;
  aci_cmd_params_change_timing.conn_params.timeout_mult      = timeout;
  acil_encode_cmd_change_timing_req(&(msg_to_send.buffer[0]), &aci_cmd_params_change_timing);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_change_timing_GAP_PPCP()
{
  acil_encode_cmd_change_timing_req_GAP_PPCP(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_open_remote_pipe(aci_state_t *aci_stat, uint8_t pipe)
{
  bool ret_val = false;
  aci_cmd_params_open_remote_pipe_t aci_cmd_params_open_remote_pipe;

  if(!((p_services_pipe_type_map[pipe-1].location == ACI_STORE_REMOTE)&&
                ((p_services_pipe_type_map[pipe-1].pipe_type == ACI_RX)||
                (p_services_pipe_type_map[pipe-1].pipe_type == ACI_RX_ACK_AUTO)||
                (p_services_pipe_type_map[pipe-1].pipe_type == ACI_RX_ACK))))
  {
    return false;
  }


  {

    is_request_operation_pending = true;
    is_open_remote_pipe_pending = true;
    request_operation_pipe = pipe;
    aci_cmd_params_open_remote_pipe.pipe_number = pipe;
    acil_encode_cmd_open_remote_pipe(&(msg_to_send.buffer[0]), &aci_cmd_params_open_remote_pipe);
    ret_val = hal_aci_tl_send(&msg_to_send);
  }
  return ret_val;
}


bool lib_aci_close_remote_pipe(aci_state_t *aci_stat, uint8_t pipe)
{
  bool ret_val = false;
  aci_cmd_params_close_remote_pipe_t aci_cmd_params_close_remote_pipe;

  if(!((p_services_pipe_type_map[pipe-1].location == ACI_STORE_REMOTE)&&
        ((p_services_pipe_type_map[pipe-1].pipe_type == ACI_RX)||
         (p_services_pipe_type_map[pipe-1].pipe_type == ACI_RX_ACK_AUTO)||
         (p_services_pipe_type_map[pipe-1].pipe_type == ACI_RX_ACK))))
  {
    return false;
  }


  {

    is_request_operation_pending = true;
    is_close_remote_pipe_pending = true;
    request_operation_pipe = pipe;
    aci_cmd_params_close_remote_pipe.pipe_number = pipe;
    acil_encode_cmd_close_remote_pipe(&(msg_to_send.buffer[0]), &aci_cmd_params_close_remote_pipe);
    ret_val = hal_aci_tl_send(&msg_to_send);
  }
  return ret_val;
}


bool lib_aci_set_key(aci_key_type_t key_rsp_type, uint8_t *key, uint8_t len)
{
  aci_cmd_params_set_key_t aci_cmd_params_set_key;
  aci_cmd_params_set_key.key_type = key_rsp_type;
  memcpy((uint8_t*)&(aci_cmd_params_set_key.key), key, len);
  acil_encode_cmd_set_key(&(msg_to_send.buffer[0]), &aci_cmd_params_set_key);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_echo_msg(uint8_t msg_size, uint8_t *p_msg_data)
{
  aci_cmd_params_echo_t aci_cmd_params_echo;
  if(msg_size > (ACI_ECHO_DATA_MAX_LEN))
  {
    return false;
  }

  if (msg_size > (ACI_ECHO_DATA_MAX_LEN))
  {
    msg_size = ACI_ECHO_DATA_MAX_LEN;
  }

  memcpy(&(aci_cmd_params_echo.echo_data[0]), p_msg_data, msg_size);
  acil_encode_cmd_echo_msg(&(msg_to_send.buffer[0]), &aci_cmd_params_echo, msg_size);

  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_bond_request()
{
  acil_encode_cmd_bond_security_request(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}

bool lib_aci_event_peek(hal_aci_evt_t *p_aci_evt_data)
{
  return hal_aci_tl_event_peek((hal_aci_data_t *)p_aci_evt_data);
}

bool lib_aci_event_get(aci_state_t *aci_stat, hal_aci_evt_t *p_aci_evt_data)
{
  bool status = false;

  status = hal_aci_tl_event_get((hal_aci_data_t *)p_aci_evt_data);

  /**
  Update the state of the ACI with the
  ACI Events -> Pipe Status, Disconnected, Connected, Bond Status, Pipe Error
  */
  if (true == status)
  {
    aci_evt_t * aci_evt;

    aci_evt = &p_aci_evt_data->evt;

    switch(aci_evt->evt_opcode)
    {
        case ACI_EVT_PIPE_STATUS:
            {
                uint8_t i=0;

                for (i=0; i < PIPES_ARRAY_SIZE; i++)
                {
                  aci_stat->pipes_open_bitmap[i]   = aci_evt->params.pipe_status.pipes_open_bitmap[i];
                  aci_stat->pipes_closed_bitmap[i] = aci_evt->params.pipe_status.pipes_closed_bitmap[i];
                }
            }
            break;

        case ACI_EVT_DISCONNECTED:
            {
                uint8_t i=0;

                for (i=0; i < PIPES_ARRAY_SIZE; i++)
                {
                  aci_stat->pipes_open_bitmap[i] = 0;
                  aci_stat->pipes_closed_bitmap[i] = 0;
                }
                aci_stat->confirmation_pending = false;
                aci_stat->data_credit_available = aci_stat->data_credit_total;

            }
            break;

        case ACI_EVT_TIMING:
                aci_stat->connection_interval = aci_evt->params.timing.conn_rf_interval;
                aci_stat->slave_latency       = aci_evt->params.timing.conn_slave_rf_latency;
                aci_stat->supervision_timeout = aci_evt->params.timing.conn_rf_timeout;
            break;

        default:
            /* Need default case to avoid compiler warnings about missing enum
             * values on some platforms.
             */
            break;



    }
  }
  return status;
}


bool lib_aci_send_ack(aci_state_t *aci_stat, const uint8_t pipe)
{
  bool ret_val = false;
  {
    acil_encode_cmd_send_data_ack(&(msg_to_send.buffer[0]), pipe);

    ret_val = hal_aci_tl_send(&msg_to_send);
  }
  return ret_val;
}


bool lib_aci_send_nack(aci_state_t *aci_stat, const uint8_t pipe, const uint8_t error_code)
{
  bool ret_val = false;

  {

    acil_encode_cmd_send_data_nack(&(msg_to_send.buffer[0]), pipe, error_code);
    ret_val = hal_aci_tl_send(&msg_to_send);
  }
  return ret_val;
}


bool lib_aci_broadcast(const uint16_t timeout, const uint16_t adv_interval)
{
  aci_cmd_params_broadcast_t aci_cmd_params_broadcast;
  if (timeout > 16383)
  {
    return false;
  }

  // The adv_interval should be between 160 and 16384 (which translates to the advertisement
  // interval values 100 ms and 10.24 s.
  if ((160 > adv_interval) || (adv_interval > 16384))
  {
    return false;
  }

  aci_cmd_params_broadcast.timeout = timeout;
  aci_cmd_params_broadcast.adv_interval = adv_interval;
  acil_encode_cmd_broadcast(&(msg_to_send.buffer[0]), &aci_cmd_params_broadcast);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_open_adv_pipes(const uint8_t * const adv_service_data_pipes)
{
  uint8_t i;

  for (i = 0; i < PIPES_ARRAY_SIZE; i++)
  {
    aci_cmd_params_open_adv_pipe.pipes[i] = adv_service_data_pipes[i];
  }

  acil_encode_cmd_open_adv_pipes(&(msg_to_send.buffer[0]), &aci_cmd_params_open_adv_pipe);
  return hal_aci_tl_send(&msg_to_send);
}

bool lib_aci_open_adv_pipe(const uint8_t pipe)
{
  uint8_t byte_idx = pipe / 8;

  aci_cmd_params_open_adv_pipe.pipes[byte_idx] |= (0x01 << (pipe % 8));
  acil_encode_cmd_open_adv_pipes(&(msg_to_send.buffer[0]), &aci_cmd_params_open_adv_pipe);
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_read_dynamic_data()
{
  acil_encode_cmd_read_dynamic_data(&(msg_to_send.buffer[0]));
  return hal_aci_tl_send(&msg_to_send);
}


bool lib_aci_write_dynamic_data(uint8_t sequence_number, uint8_t* dynamic_data, uint8_t length)
{
  acil_encode_cmd_write_dynamic_data(&(msg_to_send.buffer[0]), sequence_number, dynamic_data, length);
  return hal_aci_tl_send(&msg_to_send);
}

bool lib_aci_dtm_command(uint8_t dtm_command_msbyte, uint8_t dtm_command_lsbyte)
{
  aci_cmd_params_dtm_cmd_t aci_cmd_params_dtm_cmd;
  aci_cmd_params_dtm_cmd.cmd_msb = dtm_command_msbyte;
  aci_cmd_params_dtm_cmd.cmd_lsb = dtm_command_lsbyte;
  acil_encode_cmd_dtm_cmd(&(msg_to_send.buffer[0]), &aci_cmd_params_dtm_cmd);
  return hal_aci_tl_send(&msg_to_send);
}

void lib_aci_flush(void)
{
  hal_aci_tl_q_flush();
}

void lib_aci_debug_print(bool enable)
{
  hal_aci_tl_debug_print(enable);

}

void lib_aci_pin_reset(void)
{
    hal_aci_tl_pin_reset();
}

bool lib_aci_event_queue_empty(void)
{
  return hal_aci_tl_rx_q_empty();
}

bool lib_aci_event_queue_full(void)
{
  return hal_aci_tl_rx_q_full();
}

bool lib_aci_command_queue_empty(void)
{
  return hal_aci_tl_tx_q_empty();
}

bool lib_aci_command_queue_full(void)
{
  return hal_aci_tl_tx_q_full();
}
