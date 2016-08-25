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
 * @defgroup aci-lib aci-library
 * @brief ACI library
 *
 * Library for the logical part of the Application Controller Interface (ACI)
 * 
 * @ingroup nrf8001
 */
 
#ifndef LIB_ACI_H__
#define LIB_ACI_H__

/**
* @{
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_platform.h"
#include "hal_aci_tl.h"
#include "aci_queue.h"
#include "aci.h"
#include "aci_cmds.h"
#include "aci_evts.h"

#define EVT_CMD_RESPONSE_MIN_LENGTH              3

#define PIPES_ARRAY_SIZE                ((ACI_DEVICE_MAX_PIPES + 7)/8)

/* Same size as a hal_aci_data_t */
typedef struct {
  uint8_t   debug_byte;
  aci_evt_t evt;
} _aci_packed_ hal_aci_evt_t;

ACI_ASSERT_SIZE(hal_aci_evt_t, 34);

typedef struct
{
  uint8_t  location; /**< enum aci_pipe_store_t */
  aci_pipe_type_t   pipe_type;
} services_pipe_type_mapping_t;

typedef struct aci_setup_info_t
{
  services_pipe_type_mapping_t *services_pipe_type_mapping;
  uint8_t                       number_of_pipes;
  hal_aci_data_t               *setup_msgs;
  uint8_t                       num_setup_msgs;
} aci_setup_info_t;



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
// Relationship of bond to peer address
typedef struct aci_state_t
{
    aci_pins_t                    aci_pins;                               /* Pins on the MCU used to connect to the nRF8001 */
    aci_setup_info_t              aci_setup_info;                         /* Data structures that are created from nRFgo Studio */
    uint8_t                       bonded;                                 /* ( aci_bond_status_code_t ) Is the nRF8001 bonded to a peer device */
    uint8_t                       data_credit_total;                      /* Total data credit available for the specific version of the nRF8001, total equals available when a link is established */
    aci_device_operation_mode_t   device_state;                           /* Operating mode of the nRF8001 */

    /* */

    /* Start : Variables that are valid only when in a connection */
    uint8_t                       data_credit_available;                  /* Available data credits at a specific point of time, ACI_EVT_DATA_CREDIT updates the available credits */

    uint16_t                      connection_interval;                    /* Multiply by 1.25 to get the connection interval in milliseconds*/
    uint16_t                      slave_latency;                          /* Number of consecutive connection intervals that the nRF8001 is not required to transmit. Use this to save power */
    uint16_t                      supervision_timeout;                    /* Multiply by 10 to get the supervision timeout in milliseconds */

    uint8_t                       pipes_open_bitmap[PIPES_ARRAY_SIZE];    /* Bitmap -> pipes are open and can be used for sending data over the air */
    uint8_t                       pipes_closed_bitmap[PIPES_ARRAY_SIZE];  /* Bitmap -> pipes are closed and cannot be used for sending data over the air */
    bool                          confirmation_pending;                   /* Attribute protocol Handle Value confirmation is pending for a Handle Value Indication
                                                                        (ACK is pending for a TX_ACK pipe) on local GATT Server*/
    /* End : Variables that are valid only when in a connection */

} aci_state_t;



#define DISCONNECT_REASON_CX_TIMEOUT                 0x08
#define DISCONNECT_REASON_CX_CLOSED_BY_PEER_DEVICE   0x13
#define DISCONNECT_REASON_POWER_LOSS                 0x14
#define DISCONNECT_REASON_CX_CLOSED_BY_LOCAL_DEVICE  0x16
#define DISCONNECT_REASON_ADVERTISER_TIMEOUT         0x50


/** @name Functions for library management */
//@{

/** @brief Function to enable printing of all ACI commands sent and ACI events received
 *  @details This function shall be used to enable or disable the debug printing.
              Debug printing is disabled by default.
 */
void lib_aci_debug_print(bool enable);

/** @brief Function to pin reset the nRF8001
 *  @details Pin resets the nRF8001 also handles differences between development boards
 */
void lib_aci_pin_reset(void);

/** @brief Initialization function.
 *  @details This function shall be used to initialize/reset ACI Library and also Resets the
 *           nRF8001 by togging the reset pin of the nRF8001. This function will reset
 *           all the variables locally used by ACI library to their respective default values.
 *  @param bool True if the data was successfully queued for sending,
 *  false if there is no more space to store messages to send.
 */
void lib_aci_init(aci_state_t *aci_stat, bool debug);


/** @brief Gets the number of currently available ACI credits.
 *  @return Number of ACI credits.
 */
uint8_t lib_aci_get_nb_available_credits(aci_state_t *aci_stat);

/** @brief Gets the connection interval in milliseconds.
 *  @return Connection interval in milliseconds.
 */
uint16_t lib_aci_get_cx_interval_ms(aci_state_t *aci_stat);

/** @brief Gets the connection interval in multiple of 1.25&nbsp;ms.
 *  @return Connection interval in multiple of 1.25&nbsp;ms.
 */
uint16_t lib_aci_get_cx_interval(aci_state_t *aci_stat);

/** @brief Gets the current slave latency.
 *  @return Current slave latency.
 */
uint16_t lib_aci_get_slave_latency(aci_state_t *aci_stat);

/** @brief Checks if a given pipe is available.
 *  @param pipe Pipe to check.
 *  @return True if the pipe is available, otherwise false.
 */
bool lib_aci_is_pipe_available(aci_state_t *aci_stat, uint8_t pipe);

/** @brief Checks if a given pipe is closed.
 *  @param pipe Pipe to check.
 *  @return True if the pipe is closed, otherwise false.
 */
bool lib_aci_is_pipe_closed(aci_state_t *aci_stat, uint8_t pipe);

/** @brief Checks if the discovery operation is finished.
 *  @return True if the discovery is finished.
 */
bool lib_aci_is_discovery_finished(aci_state_t *aci_stat);



//@}

/** @name ACI Commands available in all modes */
//@{

/** @brief Sets the radio in sleep mode.
 *  @details The function sends a @c sleep command to the radio.
 *  If the radio is advertising or connected, it sends back an error, then use lib_aci_radio_reset
 *  if advertising or disconnect if in a connection.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_sleep(void);

/** @brief Resets the radio.
 *  @details The function sends a @c BasebandReset command to the radio.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_radio_reset(void);

/** @brief Radio starts directed advertising to bonded device.
 *  @details The function sends a @c DirectedConnect command to the radio.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_direct_connect(void);

/** @brief Gets the radio's version.
 *  @details This function sends a @c GetDeviceVersion command.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_device_version(void);

/** @brief Gets the device address.
 *  @details This function sends a @c GetDeviceAddress command.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_get_address(void);

/** @brief Gets the temperature.
 *  @details This function sends a @c GetTemperature command. lib_aci
 *  calls the @ref lib_aci_transaction_finished_hook() function when the temperature is received.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_get_temperature(void);

/** @brief Gets the battery level.
 *  @details This function sends a @c GetBatteryLevel command.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_get_battery_level(void);

//@}

/** @name ACI commands available in Sleep mode */
//@{

/** @brief Wakes up the radio.
 *  @details This function sends a @c Wakeup command to wake up the radio from
 *  sleep mode. When woken up the radio sends a @c DeviceStartedEvent and
 *  a @c CommandResponseEvent.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_wakeup(void);

//@}

/** @name ACI commands available in Active mode */
//@{

/** @brief Sets the radio in test mode.
 *  @details This function sends a @c Test command to the radio. There are two
 *  Test modes available:
 *  - UART: DTM commands are received over UART.
 *  - ACI: DTM commands are received over ACI.
 *  The same command is used to exit the test mode When receiving
 *  a @c DeviceStartedEvent the radio has entered the new mode.
 *  @param enter_exit_test_mode Enter a Test mode, or exit Test mode.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_test(aci_test_mode_change_t enter_exit_test_mode);

/** @brief Sets the radio's TX power.
 *  @details This function sends a @c SetTxPower command.
 *  @param tx_power TX power to be used by the radio.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_set_tx_power(aci_device_output_power_t tx_power);

/** @brief Tries to connect to a peer device.
 *  @details This function sends a @c Connect command to the radio.
 *  @param run_timeout Maximum advertising time in seconds (0 means infinite).
 *  @param adv_interval Advertising interval (in multiple of 0.625&nbsp;ms).
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_connect(uint16_t run_timeout, uint16_t adv_interval);

/** @brief Tries to bond with a peer device.
 *  @details This function sends a @c Bond command to the radio.
 *  @param run_timeout Maximum advertising time in seconds (0 means infinite).
 *  @param adv_interval Advertising interval (in multiple of 0.625&nbsp;ms).
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_bond(uint16_t run_timeout, uint16_t adv_interval);

/** @brief Disconnects from peer device.
 *  @details This function sends a @c Disconnect command to the radio.
 *  @param reason Reason for disconnecting.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_disconnect(aci_state_t *aci_stat, aci_disconnect_reason_t reason);

/**@brief Sets Local Data.
 *  @details
 *  This command updates the value of the characteristic value or the characteristic descriptor stored locally on the device.
 *  Can be called for all types of pipes as long as the data is stored locally.
 *  @param ACI state structure
 *  @param pipe Pipe number on which the data should be set.
 *  @param value Pointer to the data to set.
 *  @param size Size of the data to set.
 *  @return True if the transaction is successfully initiated.
*/
bool lib_aci_set_local_data(aci_state_t *aci_stat, uint8_t pipe, uint8_t *value, uint8_t size);

/** @brief Sends Broadcast message to the radio.
 *  @details The Broadcast message starts advertisement procedure
 *  using the given interval with the intention of broadcasting data to a peer device.
 *  @param timeout Time, in seconds, to advertise before exiting to standby mode (0 means infinite).
 *  Valid values: 0 to 16383.
 *  @param adv_interval Advertising interval (in multiple of 0.625&nbsp;ms).
 *  Valid values: 160 to 16384 (which corresponds to an interval from 100 ms to 10.24 s).
 *  @return True if the broadcast message is sent successfully to the radio.
*/
bool lib_aci_broadcast(const uint16_t timeout, const uint16_t adv_interval);

/** @name Open Advertising Pipes.  */

/** @brief Sends a command to the radio to set the input pipe to be placed in Advertisement Service Data.
 *  @details This function sends a command to the radio that places the pipe in
 *  advertisement service data.  To start advertising service data, call this function before
 *  Connecting, Broadcasting or Bonding to peer. The data that should be sent in the advertisement packets
 *  must be set using the @c lib_aci_set_local_data function. This function can be called during
 *  advertising to enable/disable broadcast pipes.
 *  @param pipe The pipe that has to be placed in advertising service data.
 *  @return True if the Open Adv Pipe message is sent successfully to the radio.
*/
bool lib_aci_open_adv_pipe(const uint8_t pipe);


/** @name Open Advertising Pipes  */

/** @brief Sends a command to the radio to set the pipes to be placed in Advertisement Service Data.
 *  @details This function will send a command to the radio that will set the pipes to be placed in
 *  advertisement Service Data.  To start advertising service data, this function should be called before
 *  Connecting, Broadcasting or Bonding to peer. This function can be called during
 *  advertising to enable/disable broadcast pipes. Use this as an alternative to @ref lib_aci_open_adv_pipe
 *  to avoid multiple function calls for placing multiple pipes in the adv data.
 *  @param adv_service_data_pipes Pipe bitmap, where '1' indicates that the corresponding
 *  Valid Values: 0000000000000000 to FEFFFFFFFFFFFF7F (See the ACI Pipe Status Evt bitmap in the nRF8001 datasheet
 *  TX_BROADCAST pipe data is to be placed in Advertising Service Data fields
 *  @return true if the Open Adv Pipe message was sent successfully to the radio.
*/
bool lib_aci_open_adv_pipes(const uint8_t * const adv_service_data_pipes);


//@}

/** @name ACI commands available in Connected mode */
//@{


/** @brief Sets a given application latency.
 *  @details This function sends a @c setApplicationLatency command.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_set_app_latency(uint16_t latency, aci_app_latency_mode_t latency_mode);

/** @brief Opens a remote pipe.
 *  @details This function sends an @c OpenRemotePipe command.
 *  @param pipe Number of the pipe to open.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_open_remote_pipe(aci_state_t *aci_stat, uint8_t pipe);

/** @brief Closes a remote pipe.
 *  @details This function sends an @c CloseRemotePipe command.
 *  @param pipe Pipe number to close.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_close_remote_pipe(aci_state_t *aci_stat, uint8_t pipe);

/** @brief Sends data on a given pipe.
 *  @details This function sends a @c SendData command with application data to
 *  the radio. This function memorizes credit use, and checks that
 *  enough credits are available.
 *  @param pipe Pipe number on which the data should be sent.
 *  @param value Pointer to the data to send.
 *  @param size Size of the data to send.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_send_data(uint8_t pipe, uint8_t *value, uint8_t size);

/** @brief Requests data from a given pipe.
 *  @details This function sends a @c RequestData command to the radio. This
 *  function memorizes credit uses, and check that enough credits are available.
 *  After this command, the radio sends back either a @c DataReceivedEvent
 *  or a @c PipeErrorEvent.
 *  @param pipe Pipe number on which the data is requested.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_request_data(aci_state_t *aci_stat, uint8_t pipe);

/** @brief Sends a L2CAP change connection parameters request.
 *  @details This function sends a @c ChangeTiming command to the radio.  This command triggers a "L2CAP change connection parameters" request
 *  to the master. If the master rejects or accepts but doesn't change the connection parameters within
 *  30 seconds, a timing event with the unchanged connection parameters is sent by the radio.
 *  If the request is accepted and the master changes connection parameters, a timing event with
 *  the new connection parameters is sent by the radio.
 *  If the master doesn't reply to the request within 60 seconds, the radio disconnects.
 *  @param minimun_cx_interval Minimum connection interval requested, in multiple of 1.25&nbsp;ms.
 *  @param maximum_cx_interval Maximum connection interval requested, in multiple of 1.25&nbsp;ms.
 *  @param slave_latency requested slave latency.
 *  @param timeout requested slave timeout, in multiple of 10&nbsp;ms.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_change_timing(uint16_t minimun_cx_interval, uint16_t maximum_cx_interval, uint16_t slave_latency, uint16_t timeout);

/** @brief Sends a L2CAP change connection parameters request with the connection predefined preffered connection parameters.
 *  @details This function sends a @c ChangeTiming command to the radio. This command triggers a "L2CAP change connection parameters" request
 *  to the master. If the master rejects or accepts but doesn't change the connection parameters within
 *  30 seconds, a timing event with the unchanged connection parameters is sent by the radio.
 *  If the request is accepted and the master changes connection parameters, a timing event with
 *  the new connection parameters is sent by the radio.
 *  If the master doesn't reply to the request within 60 seconds, the radio disconnects.
 *  The timing parameters used are the Timing parameters in the GAP settings in the nRFgo Studio.
 *  The Timing parameters as stored as the GAP Preferred Peripheral Connection Parameters.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_change_timing_GAP_PPCP(void);

/** @brief Sends acknowledgement message to peer.
 *  @details This function sends @c SendDataAck command to radio. The radio is expected
 *  to send either Handle Value Confirmation or Write response depending
 *  on whether the data is stored remotely or locally.
 *  @param pipe Pipe number for which the acknowledgement is to be sent.
 *  @return True if the ack was sent successfully. False otherwise.
*/
bool lib_aci_send_ack(aci_state_t *aci_stat, const uint8_t pipe);

/** @brief Sends negative acknowledgement message to peer.
 *  @details This function sends @c SendDataNack command to radio. The radio is expected
 *  to send Error Response to the peer.
 *  @param pipe Pipe number for which the nack is to be sent.
 *  @param error_code Error code to be sent in the NACk.
 *  @return True if the nack was sent successfully. False otherwise.
*/
bool lib_aci_send_nack(aci_state_t *aci_stat, const uint8_t pipe, const uint8_t error_code);

/** @brief Sends ReadDynamicData command to the host.
 *  @details This function sends @c ReadDynamicData command to host. The host is expected
 *  to send @c CommandResponse back with the dynamic data. The application is expected to
 *  call this function in a loop until all the dynamic data is read out from the host.
 *  As long as there is dynamic data to be read from the host, the command response
 *  for this message has its status field set to ACI_STATUS_TRANSACTION_CONTINUE (0x01).
 *  The application may chose to store this read out data in a non-volatile memory location
 *  and later chose to write it back using the function lib_aci_write_dynamic_data.
 *  @return True if the command was sent successfully through the ACI. False otherwise.
*/
bool lib_aci_read_dynamic_data(void);

/** @brief Sends WriteDynamicData command to the host.
 *  @details This function sends @c WriteDynamicData command to host. The host is expected
 *  to send @c CommandResponse with the status of this operation. As long as the status field
 *  in the @c CommandResponse is ACI_STATUS_TRANSACTION_CONTINUE (0x01), the hosts expects
 *  more dynamic data to be written. This function should ideally be called in a cycle,
 *  until all the stored dynamic data is sent to the host. This function should be
 *  called with the dynamic data obtained from the response to a @c ReadDynamicData
 *  (see @c lib_aci_read_dynamic_data) command.
 *  @param sequence_number Sequence number of the dynamic data to be sent.
 *  @param dynamic_data Pointer to the dynamic data.
 *  @param length Length of the dynamic data.
 *  @return True if the command was sent successfully through the ACI. False otherwise.
*/
bool lib_aci_write_dynamic_data(uint8_t sequence_number, uint8_t* dynamic_data, uint8_t length);
//@}

/** @name ACI commands available while connected in Bond mode */
//@{

/** @brief Sends a SMP Security Request.
 *  @details This function send a @c BondRequest command to the radio.
 *  This command triggers a SMP Security Request to the master. If the
 *  master rejects with a pairing failed or if the bond timer expires the connection is closed.
 *  @return True if the transaction is successfully initiated.
 */
bool lib_aci_bond_request(void);

/** @brief Set the key requested by the 8001.
 *  @details This function sends an @c SetKey command to the radio.
 *  @param key_rsp_type Type of key.
 *  @param key Pointer to the key to set.
 *  @param len Length of the key.
 *  @return True if the transaction is successfully initiated.
*/
bool lib_aci_set_key(aci_key_type_t key_rsp_type, uint8_t *key, uint8_t len);

//@}



/** @name ACI commands available in Test mode */
//@{

/** @brief Sends an echo message
 *  @details This function sends an @c Echo command to the radio. lib_aci
 *  places the Echp ACI command in the ACI command queue
 *  @param message_size Length of the data to send.
 *  @param message_data Pointer to the data to send.
 *  @return True if the transaction is successfully initiated.
*/
bool lib_aci_echo_msg(uint8_t message_size, uint8_t *message_data);

/** @brief Sends an DTM command
 *  @details This function sends an @c DTM command to the radio.
 *  @param dtm_command_msbyte Most significant byte of the DTM command.
 *  @param dtm_command_lsbyte Least significant byte of the DTM command.
 *  @return True if the transaction is successfully initiated.
*/
bool lib_aci_dtm_command(uint8_t dtm_command_msbyte, uint8_t dtm_command_lsbyte);

/** @brief Gets an ACI event from the ACI Event Queue
 *  @details This function gets an ACI event from the ACI event queue.
 *  The queue is updated by the SPI driver for the ACI running in the interrupt context
 *  @param aci_stat pointer to the state of the ACI.
 *  @param p_aci_data pointer to the ACI Event. The ACI Event received will be copied into this pointer.
 *  @return True if an ACI Event was copied to the pointer.
*/
bool lib_aci_event_get(aci_state_t *aci_stat, hal_aci_evt_t * aci_evt);

/** @brief Peeks an ACI event from the ACI Event Queue
 * @details This function peeks at the top event in the ACI event queue.
 * In polling mode, this function will query the nRF8001 for pending events, but unlike
 * lib_aci_event_get() it will not dequeue the event from the local queue, but will instead
 * only peek at it.
 * @return True if an ACI Event was copied to the pointer.
*/
bool lib_aci_event_peek(hal_aci_evt_t *p_aci_evt_data);

/** @brief Flushes the events in the ACI command queues and ACI Event queue
 *
*/
void lib_aci_flush(void);

/** @brief Return full status of the Event queue
 *  @details
 *
 */
 bool lib_aci_event_queue_full(void);

 /** @brief Return empty status of the Event queue
 *  @details
 *
 */
 bool lib_aci_event_queue_empty(void);

/** @brief Return full status of Command queue
 *  @details
 *
 */
 bool lib_aci_command_queue_full(void);

 /** @brief Return empty status of Command queue
 *  @details
 *
 */
 bool lib_aci_command_queue_empty(void);

//@}

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* LIB_ACI_H__ */
