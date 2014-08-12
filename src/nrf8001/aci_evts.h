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
 * @ingroup aci
 *
 * @brief Definitions for the ACI (Application Control Interface) events
 */

#ifndef ACI_EVTS_H__
#define ACI_EVTS_H__

#include "aci.h"

/**
 * @enum aci_evt_opcode_t
 * @brief ACI event opcodes
 */
typedef enum
{
 /**
  * Invalid event code
  */
  ACI_EVT_INVALID                     = 0x00,
 /**
  * Sent every time the device starts
  */
  ACI_EVT_DEVICE_STARTED              = 0x81,
 /**
  * Mirrors the ACI_CMD_ECHO
  */
  ACI_EVT_ECHO                        = 0x82,
 /**
  * Asynchronous hardware error event
  */
  ACI_EVT_HW_ERROR                  = 0x83,
 /**
  * Event opcode used as a event response for all commands
  */
  ACI_EVT_CMD_RSP                     = 0x84,
 /**
  * Link connected
  */
  ACI_EVT_CONNECTED                   = 0x85,
 /**
  * Link disconnected
  */
  ACI_EVT_DISCONNECTED                = 0x86,
 /**
  * Bond completion result
  */
  ACI_EVT_BOND_STATUS                 = 0x87,
  /**
  * Pipe bitmap for available pipes
  */
  ACI_EVT_PIPE_STATUS             = 0x88,
 /**
  * Sent to the application when the radio enters a connected state
  * or when the timing of the radio connection changes
  */
  ACI_EVT_TIMING                      = 0x89,
 /**
  * Notification to the application that transmit credits are
  * available
  */
  ACI_EVT_DATA_CREDIT                 = 0x8A,
 /**
  * Data acknowledgement event
  */
  ACI_EVT_DATA_ACK                    = 0x8B,
 /**
  * Data received notification event
  */
  ACI_EVT_DATA_RECEIVED               = 0x8C,
 /**
  * Error notification event
  */
  ACI_EVT_PIPE_ERROR                  = 0x8D,
 /**
  * Display Passkey Event
  */
  ACI_EVT_DISPLAY_PASSKEY             = 0x8E,
 /**
  * Security Key request
  */
  ACI_EVT_KEY_REQUEST                 = 0x8F

} _aci_packed_ aci_evt_opcode_t;

ACI_ASSERT_SIZE(aci_evt_opcode_t, 1);

/**
 * @struct aci_evt_params_device_started_t
 * @brief Structure for the ACI_EVT_DEVICE_STARTED event return parameters
 */
typedef struct
{
  aci_device_operation_mode_t device_mode; /**< Mode in which the device is being started */
  aci_hw_error_t hw_error;  /**< Hardware Error if available for the start */
  uint8_t credit_available; /**< Flow control credit available for this specific FW build */
} _aci_packed_ aci_evt_params_device_started_t;

ACI_ASSERT_SIZE(aci_evt_params_device_started_t, 3);

/**
 * @struct aci_evt_params_hw_error_t
 * @brief Structure for the ACI_EVT_HW_ERROR event return parameters
 */
typedef struct
{
  uint16_t line_num;
  uint8_t file_name[20];
} _aci_packed_ aci_evt_params_hw_error_t;

ACI_ASSERT_SIZE(aci_evt_params_hw_error_t, 22);

/**
 * @struct aci_evt_cmd_rsp_params_dtm_cmd_t
 * @brief Structure for the ACI_EVT_CMD_RSP event with opcode=ACI_CMD_DTM_CMD event return parameters
 */
typedef struct
{
  uint8_t  evt_msb;
  uint8_t  evt_lsb;
} _aci_packed_ aci_evt_cmd_rsp_params_dtm_cmd_t;

/**
 * @struct aci_evt_cmd_rsp_read_dynamic_data_t
 * @brief Structure for the ACI_EVT_CMD_RSP event with opcode=ACI_CMD_READ_DYNAMIC_DATA event return parameters
 * @note Dynamic data chunk size in this event is defined to go up to ACI_PACKET_MAX_LEN - 5
 */
typedef struct
{
  uint8_t seq_no;
  uint8_t dynamic_data[1];
} _aci_packed_ aci_evt_cmd_rsp_read_dynamic_data_t;

/**
 * @struct aci_evt_cmd_rsp_params_get_device_version_t
 * @brief Structure for the ACI_EVT_CMD_RSP event with opcode=ACI_CMD_GET_DEVICE_VERSION event return parameters
 */
typedef struct
{
  uint16_t  configuration_id;
  uint8_t   aci_version;
  uint8_t   setup_format;
  uint32_t  setup_id;
  uint8_t   setup_status;
} _aci_packed_ aci_evt_cmd_rsp_params_get_device_version_t;

ACI_ASSERT_SIZE(aci_evt_cmd_rsp_params_get_device_version_t, 9);

/**
 * @struct aci_evt_cmd_rsp_params_get_device_address_t
 * @brief Structure for the ACI_EVT_CMD_RSP event with opcode=ACI_CMD_GET_DEVICE_ADDRESS event return parameters
 */
typedef struct
{
  uint8_t  bd_addr_own[BTLE_DEVICE_ADDRESS_SIZE];
  aci_bd_addr_type_t bd_addr_type;
} _aci_packed_ aci_evt_cmd_rsp_params_get_device_address_t;

ACI_ASSERT_SIZE(aci_evt_cmd_rsp_params_get_device_address_t, BTLE_DEVICE_ADDRESS_SIZE + 1);

/**
 * @struct aci_evt_cmd_rsp_params_get_battery_level_t
 * @brief Structure for the ACI_EVT_CMD_RSP event with opcode=ACI_CMD_GET_BATTERY_LEVEL event return parameters
 */
typedef struct
{
  uint16_t battery_level;
} _aci_packed_ aci_evt_cmd_rsp_params_get_battery_level_t;

/**
 * @struct aci_evt_cmd_rsp_params_get_temperature_t
 * @brief Structure for the ACI_EVT_CMD_RSP event with opcode=ACI_CMD_GET_TEMPERATURE event return parameters
 */
typedef struct
{
  int16_t temperature_value;
} _aci_packed_ aci_evt_cmd_rsp_params_get_temperature_t;

/**
 * @struct aci_evt_params_cmd_rsp_t
 * @brief Structure for the ACI_EVT_CMD_RSP event return parameters
 */
typedef struct
{
  aci_cmd_opcode_t cmd_opcode; /**< Command opcode for which the event response is being sent */
  aci_status_code_t cmd_status; /**< Status of the command that was sent. Used in the context of the command. */
  union
  {
    aci_evt_cmd_rsp_params_dtm_cmd_t dtm_cmd;
    aci_evt_cmd_rsp_read_dynamic_data_t read_dynamic_data;
    aci_evt_cmd_rsp_params_get_device_version_t get_device_version;
    aci_evt_cmd_rsp_params_get_device_address_t get_device_address;
    aci_evt_cmd_rsp_params_get_battery_level_t  get_battery_level;
    aci_evt_cmd_rsp_params_get_temperature_t    get_temperature;
    uint8_t                                     padding[29];
  } params;
} _aci_packed_ aci_evt_params_cmd_rsp_t;

ACI_ASSERT_SIZE(aci_evt_params_cmd_rsp_t, 31);

/**
 * @struct aci_evt_params_connected_t
 * @brief Structure for the ACI_EVT_CONNECTED event return parameters
 */
typedef struct
{
  aci_bd_addr_type_t dev_addr_type;
  uint8_t  dev_addr[BTLE_DEVICE_ADDRESS_SIZE];
  uint16_t conn_rf_interval;  /**< rf_interval = conn_rf_interval * 1.25 ms Range:0x0006 to 0x0C80 */
  uint16_t conn_slave_rf_latency; /**< Number of RF events the slave can skip */
  uint16_t conn_rf_timeout; /**< Timeout as a multiple of 10ms i.e timeout = conn_rf_timeout * 10ms Range: 0x000A to 0x0C80 */
  aci_clock_accuracy_t master_clock_accuracy; /**< Clock accuracy of Bluetooth master: Enumerated list of values from 500 ppm to 20 ppm */
} _aci_packed_ aci_evt_params_connected_t;

ACI_ASSERT_SIZE(aci_evt_params_connected_t, 14);

/**
 * @struct aci_evt_params_disconnected_t
 * @brief Structure for the ACI_EVT_DISCONNECTED event return parameters
 */
typedef struct
{
  aci_status_code_t   aci_status;
  uint8_t btle_status;
} _aci_packed_ aci_evt_params_disconnected_t;

ACI_ASSERT_SIZE(aci_evt_params_disconnected_t, 2);

/**
 * @struct aci_evt_params_bond_status_t
 * @brief Structure for the ACI_EVT_BOND_STATUS event return parameters
 */
typedef struct
{
  aci_bond_status_code_t status_code;
  aci_bond_status_source_t status_source;
  uint8_t secmode1_bitmap;
  uint8_t secmode2_bitmap;
  uint8_t keys_exchanged_slave;
  uint8_t keys_exchanged_master;
} _aci_packed_ aci_evt_params_bond_status_t;

ACI_ASSERT_SIZE(aci_evt_params_bond_status_t, 6);

/**
 * @struct aci_evt_params_pipe_status_t
 * @brief Structure for the ACI_EVT_PIPE_STATUS event return parameters
 */
typedef struct
{
  uint8_t  pipes_open_bitmap[8];
  uint8_t  pipes_closed_bitmap[8];
} _aci_packed_ aci_evt_params_pipe_status_t;

ACI_ASSERT_SIZE(aci_evt_params_pipe_status_t, 16);

/**
 * @struct aci_evt_params_timing_t
 * @brief Structure for the ACI_EVT_TIMING event return parameters
 */
typedef struct
{
  uint16_t conn_rf_interval;  /**< rf_interval = conn_rf_interval * 1.25 ms Range:0x0006 to 0x0C80 */
  uint16_t conn_slave_rf_latency; /**< Number of RF events the slave can skip */
  uint16_t conn_rf_timeout; /**< Timeout as a multiple of 10ms i.e timeout = conn_rf_timeout * 10ms Range: 0x000A to 0x0C80 */
} _aci_packed_ aci_evt_params_timing_t;

ACI_ASSERT_SIZE(aci_evt_params_timing_t, 6);

/**
 * @struct aci_evt_params_data_credit_t
 * @brief Structure for the ACI_EVT_DATA_CREDIT event return parameters
 */
typedef struct
{
  uint8_t credit;
} _aci_packed_ aci_evt_params_data_credit_t;

/**
 * @struct aci_evt_params_data_ack_t
 * @brief Structure for the ACI_EVT_DATA_ACK event return parameters
 */
typedef struct
{
  uint8_t pipe_number;
} _aci_packed_ aci_evt_params_data_ack_t;

/**
 * @struct aci_evt_params_data_received_t
 * @brief Structure for the ACI_EVT_DATA_RECEIVED event return parameters
 */
typedef struct
{
  aci_rx_data_t rx_data;
} _aci_packed_ aci_evt_params_data_received_t;

typedef struct
{
  uint8_t content[1];
} _aci_packed_ error_data_t;

/**
 * @struct aci_evt_params_pipe_error_t
 * @brief Structure for the ACI_EVT_PIPE_ERROR event return parameters
 */
typedef struct
{
  uint8_t pipe_number;
  uint8_t error_code;
  union
  {
    error_data_t  error_data;
  } params;
} _aci_packed_ aci_evt_params_pipe_error_t;

/**
 * @struct aci_evt_params_display_passkey_t
 * @brief Structure for the ACI_EVT_DISPLAY_PASSKEY event return parameters
 */
typedef struct
{
  uint8_t passkey[6];
} _aci_packed_ aci_evt_params_display_passkey_t;

/**
 * @struct aci_evt_params_key_request_t
 * @brief Structure for the ACI_EVT_KEY_REQUEST event return parameters
 */
typedef struct
{
  aci_key_type_t key_type;
} _aci_packed_ aci_evt_params_key_request_t;

/**
 * @struct aci_event_params_echo_t
 * @brief  Structure for the ACI_EVT_ECHO ACI event parameters
 */
typedef struct
{
  uint8_t echo_data[ACI_ECHO_DATA_MAX_LEN];
} _aci_packed_ aci_evt_params_echo_t;

/**
 * @struct aci_evt_t
 * @brief  Encapsulates a generic ACI event
 */
typedef struct
{
  uint8_t len;
  aci_evt_opcode_t evt_opcode;
  union
  {
    aci_evt_params_device_started_t                     device_started;
    aci_evt_params_echo_t                               echo;
    aci_evt_params_hw_error_t                           hw_error;
    aci_evt_params_cmd_rsp_t                            cmd_rsp;
    aci_evt_params_connected_t                          connected;
    aci_evt_params_disconnected_t                       disconnected;
    aci_evt_params_bond_status_t                        bond_status;
    aci_evt_params_pipe_status_t                        pipe_status;
    aci_evt_params_timing_t                             timing;
    aci_evt_params_data_credit_t                        data_credit;
    aci_evt_params_data_ack_t                           data_ack;
    aci_evt_params_data_received_t                      data_received;
    aci_evt_params_pipe_error_t                         pipe_error;
    aci_evt_params_display_passkey_t                    display_passkey;
    aci_evt_params_key_request_t                        key_request;
  } params;
} _aci_packed_ aci_evt_t;

ACI_ASSERT_SIZE(aci_evt_t, 33);

#endif // ACI_EVTS_H__
