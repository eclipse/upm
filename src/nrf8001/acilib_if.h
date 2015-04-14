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
 * @ingroup aci-lib
 *
 * @brief Prototypes for the acilib interfaces.
 */

#ifndef _acilib_IF_H_
#define _acilib_IF_H_

/** @brief Encode the ACI message for set test mode command
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *  @param[in]      test_mode   Pointer to the test mode in ::aci_cmd_params_test_t
 *
 *  @return         None
 */
void acil_encode_cmd_set_test_mode(uint8_t *buffer, aci_cmd_params_test_t *p_aci_cmd_params_test);

/** @brief Encode the ACI message for sleep command
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_sleep(uint8_t *buffer);

/** @brief Encode the ACI message for get device version
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_get_device_version(uint8_t *buffer);

/** @brief Encode the ACI message for set local data
 *
 *  @param[in,out]  buffer                           Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_set_local_data  Pointer to the local data parameters in ::aci_cmd_params_set_local_data_t
 *  @param[in]      data_size                        Size of data message
 *
 *  @return         None
 */
void acil_encode_cmd_set_local_data(uint8_t *buffer, aci_cmd_params_set_local_data_t *p_aci_cmd_params_set_local_data, uint8_t data_size);

/** @brief Encode the ACI message to connect
 *
 *  @param[in,out]  buffer                    Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_connect  Pointer to the run parameters in ::aci_cmd_params_connect_t
 *
 *  @return         None
 */
void acil_encode_cmd_connect(uint8_t *buffer, aci_cmd_params_connect_t *p_aci_cmd_params_connect);

/** @brief Encode the ACI message to bond
 *
 *  @param[in,out]  buffer                 Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_bond  Pointer to the run parameters in ::aci_cmd_params_bond_t
 *
 *  @return         None
 */
void acil_encode_cmd_bond(uint8_t *buffer, aci_cmd_params_bond_t *p_aci_cmd_params_bond);

/** @brief Encode the ACI message to disconnect
 *
 *  @param[in,out]  buffer                       Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_disconnect  Pointer to the run parameters in ::aci_cmd_params_disconnect_t
 *
 *  @return         None
 */
void acil_encode_cmd_disconnect(uint8_t *buffer, aci_cmd_params_disconnect_t *p_aci_cmd_params_disconnect);

/** @brief Encode the ACI message to baseband reset
 *
 *  @param[in,out]  buffer        Pointer to ACI message buffer
 *
 *  @return         None
 */
 void acil_encode_baseband_reset(uint8_t *buffer);

 /** @brief Encode the ACI message for Directed Advertising
 *
 *  @param[in,out]  buffer        Pointer to ACI message buffer
 *
 *  @return         None
 */
 void acil_encode_direct_connect(uint8_t *buffer);

/** @brief Encode the ACI message to wakeup
 *
 *  @param[in,out]  buffer        Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_wakeup(uint8_t *buffer);

/** @brief Encode the ACI message for set radio Tx power
 *
 *  @param[in,out]  buffer                         Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_set_tx_power  Pointer to the set Tx power parameters in ::aci_cmd_params_set_tx_power_t
 *
 *  @return         None
 */
void acil_encode_cmd_set_radio_tx_power(uint8_t *buffer, aci_cmd_params_set_tx_power_t *p_aci_cmd_params_set_tx_power);

/** @brief Encode the ACI message for get device address
 *
 *  @param[in,out]  buffer            Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_get_address(uint8_t *buffer);

/** @brief Encode the ACI message for send data
 *
 *  @param[in,out]  buffer                        Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_send_data_t  Pointer to the data parameters in ::aci_cmd_params_send_data_t
 *  @param[in]      data_size                     Size of data message
 *
 *  @return         None
 */
void acil_encode_cmd_send_data(uint8_t *buffer, aci_cmd_params_send_data_t *p_aci_cmd_params_send_data_t, uint8_t data_size);

/** @brief Encode the ACI message for request data
 *
 *  @param[in,out]  buffer                          Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_request_data   Pointer to the request data parameters in ::aci_cmd_params_request_data_t
 *
 *  @return         None
 */
void acil_encode_cmd_request_data(uint8_t *buffer, aci_cmd_params_request_data_t *p_aci_cmd_params_request_data);

/** @brief Encode the ACI message for open remote pipe
 *
 *  @param[in,out]  buffer                              Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_open_remote_pipe   Pointer to the dynamic data parameters in ::aci_cmd_params_open_remote_pipe_t
 *
 *  @return         None
 */
void acil_encode_cmd_open_remote_pipe(uint8_t *buffer, aci_cmd_params_open_remote_pipe_t *p_aci_cmd_params_open_remote_pipe);

/** @brief Encode the ACI message for close remote pipe
 *
 *  @param[in,out]  buffer                              Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_close_remote_pipe   Pointer to the dynamic data parameters in ::aci_cmd_params_close_remote_pipe_t
 *
 *  @return         None
 */
void acil_encode_cmd_close_remote_pipe(uint8_t *buffer, aci_cmd_params_close_remote_pipe_t *p_aci_cmd_params_close_remote_pipe);

/** @brief Encode the ACI message for echo message
 *
 *  @param[in,out]  buffer             Pointer to ACI message buffer
 *  @param[in]      p_cmd_params_echo  Pointer to the dynamic data parameters in ::aci_cmd_params_echo_t
 *  @param[in]      msg_size           Size of the message
 *
 *  @return         None
 */
void acil_encode_cmd_echo_msg(uint8_t *buffer, aci_cmd_params_echo_t *p_cmd_params_echo, uint8_t msg_size);

/** @brief Encode the ACI message to battery level
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_battery_level(uint8_t *buffer);

/** @brief Encode the ACI message to temparature
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_temparature(uint8_t *buffer);

/** @brief Encode the ACI message to read dynamic data
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_read_dynamic_data(uint8_t *buffer);

/** @brief Encode the ACI message to change timing request
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_change_timing  Pointer to the change timing parameters in ::aci_cmd_params_change_timing_t
 *
 *  @return         None
 */
void acil_encode_cmd_change_timing_req(uint8_t *buffer, aci_cmd_params_change_timing_t *p_aci_cmd_params_change_timing);

/** @brief Encode the ACI message to change timing request using the timing parameters from GAP PPCP
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_change_timing  Pointer to the change timing parameters in ::aci_cmd_params_change_timing_t
 *
 *  @return         None
 */
void acil_encode_cmd_change_timing_req_GAP_PPCP(uint8_t *buffer);


/** @brief Encode the ACI message for write dynamic data
 *
 *  @param[in,out]  buffer                          Pointer to ACI message buffer
 *  @param[in]      seq_no                          Sequence number of the dynamic data (as received in the response to @c Read Dynamic Data)
 *  @param[in]      dynamic_data                    Pointer to the dynamic data
 *  @param[in]      dynamic_data_size               Size of dynamic data
 *
 *  @return         None
 */
void acil_encode_cmd_write_dynamic_data(uint8_t *buffer, uint8_t seq_no, uint8_t* dynamic_data, uint8_t dynamic_data_size);

/** @brief Encode the ACI message to send data acknowledgement
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *  @param[in]      pipe_number Pipe number for which the ack is to be sent
 *
 *  @return         None
 */
void acil_encode_cmd_send_data_ack(uint8_t *buffer, const uint8_t pipe_number);

/** @brief Encode the ACI message to send negative acknowledgement
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *  @param[in]    pipe_number Pipe number for which the nack is to be sent
 *  @param[in]    error_code Error code that has to be sent in the NACK
 *
 *  @return         None
 */
void acil_encode_cmd_send_data_nack(uint8_t *buffer, const uint8_t pipe_number,const uint8_t error_code);

/** @brief Encode the ACI message to set the application latency
 *
 *  @param[in,out]  buffer  Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd_params_set_app_latency  Pointer to the set_application_latency command parameters in ::aci_cmd_params_dtm_cmd_t
 *
 *  @return         None
 */
void acil_encode_cmd_set_app_latency(uint8_t *buffer, aci_cmd_params_set_app_latency_t *p_aci_cmd_params_set_app_latency);

/** @brief Encode the ACI message for setup
 *
 *  @param[in,out]  buffer                          Pointer to ACI message buffer
 *  @param[in]      p_cmd_params_set_run_behaviour  Pointer to the setup data in ::aci_cmd_params_setup_t
 *  @param[in]      setup_data_size                 Size of setup message
 *
 *  @return         None
 */
void acil_encode_cmd_setup(uint8_t *buffer, aci_cmd_params_setup_t *p_aci_cmd_params_setup, uint8_t setup_data_size);

/** @brief Encode the ACI message for DTM command
 *
 *  @param[in,out]  buffer                          Pointer to ACI message buffer
 *  @param[in]      p_cmd_params_set_run_behaviour  Pointer to the DTM command parameters in ::aci_cmd_params_dtm_cmd_t
 *
 *  @return         None
 */
void acil_encode_cmd_dtm_cmd(uint8_t *buffer, aci_cmd_params_dtm_cmd_t *p_aci_cmd_params_dtm_cmd);

/** @brief Encode the ACI message for Set Key Request command
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_set_key(uint8_t *buffer, aci_cmd_params_set_key_t *p_aci_cmd_params_set_key);

/** @brief Encode the ACI message for Bond Security Request command
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *
 *  @return         None
 */
void acil_encode_cmd_bond_security_request(uint8_t *buffer);

/** @brief Encode the ACI message
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd   Pointer to ACI command data in ::aci_cmd_t
 *  @param[in]      bool
 *
 *  @return         bool         true, if succesful, else returns false
 */
bool acil_encode_cmd(uint8_t *buffer, aci_cmd_t *p_aci_cmd);

/** @brief Encode the ACI message for Broadcast command
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd   Pointer to ACI command data in ::aci_cmd_params_broadcast_t
 *
 *  @return         None
 */
void acil_encode_cmd_broadcast(uint8_t *buffer,  aci_cmd_params_broadcast_t * p_aci_cmd_params_broadcast);

/** @brief Encode the ACI message for Open Adv Pipes
 *
 *  @param[in,out]  buffer      Pointer to ACI message buffer
 *  @param[in]      p_aci_cmd   Pointer to ACI command data in ::aci_cmd_params_open_adv_pipe_t
 *
 *  @return         None
 */
void acil_encode_cmd_open_adv_pipes(uint8_t *buffer, aci_cmd_params_open_adv_pipe_t * p_aci_cmd_params_set_adv_svc_data);

/** @brief Decode the ACI event command response
 *
 *  @param[in]      buffer_in  Pointer to message received
 *  @param[in,out]  buffer     Pointer to the decoded message in ::aci_evt_params_cmd_rsp_t
 *
 *  @return         None
 */
void acil_decode_evt_command_response(uint8_t *buffer_in, aci_evt_params_cmd_rsp_t *p_evt_params_cmd_rsp);

/** @brief Decode the ACI event device started
 *
 *  @param[in]      buffer_in  Pointer to message received
 *  @param[in,out]  p_aci_evt  Pointer to the decoded message in ::aci_evt_params_device_started_t
 *
 *  @return         None
 */
void acil_decode_evt_device_started(uint8_t *buffer_in, aci_evt_params_device_started_t *p_evt_params_device_started);

/** @brief Decode the ACI event pipe status
 *
 *  @param[in]      buffer_in  Pointer to message received
 *  @param[in,out]  p_aci_evt_params_pipe_status  Pointer to the decoded message in ::aci_evt_params_pipe_status_t
 *
 *  @return         None
 */
void acil_decode_evt_pipe_status(uint8_t *buffer_in, aci_evt_params_pipe_status_t *p_aci_evt_params_pipe_status);

/** @brief Decode the ACI event for disconnected
 *
 *  @param[in]      buffer_in                      Pointer to message received
 *  @param[in,out]  p_aci_evt_params_disconnected  Pointer to the decoded message in ::aci_evt_params_disconnected_t
 *
 *  @return         None
 */
void acil_decode_evt_disconnected(uint8_t *buffer_in, aci_evt_params_disconnected_t *p_aci_evt_params_disconnected);

/** @brief Decode the ACI event for bond status
 *
 *  @param[in]      buffer_in                     Pointer to message received
 *  @param[in,out]  p_aci_evt_params_bond_status  Pointer to the decoded message in ::aci_evt_params_bond_status_t
 *
 *  @return         None
 */
void acil_decode_evt_bond_status(uint8_t *buffer_in, aci_evt_params_bond_status_t *p_aci_evt_params_bond_status);

/** @brief Decode the ACI event for data received
 *
 *  @param[in]      buffer_in                   Pointer to message received
 *  @param[in,out]  p_evt_params_data_received  Pointer to the decoded message in ::aci_evt_params_data_received_t
 *
 *  @return         size                        Received data size
 */
uint8_t acil_decode_evt_data_received(uint8_t *buffer_in, aci_evt_params_data_received_t *p_evt_params_data_received);

/** @brief Decode the ACI event data acknowledgement
 *
 *  @param[in]      buffer_in               Pointer to message received
 *  @param[in,out]  p_evt_params_data_ack   Pointer to the decoded message in ::aci_evt_params_data_ack_t
 *
 *  @return         None
 */
void acil_decode_evt_data_ack(uint8_t *buffer_in, aci_evt_params_data_ack_t *p_evt_params_data_ack);

/** @brief Decode the ACI event for hardware error
 *
 *  @param[in]      buffer_in                  Pointer to message received
 *  @param[in,out]  p_aci_evt_params_hw_error  Pointer to the decoded message in ::aci_evt_params_hw_error_t
 *
 *  @return         size                     Size of debug information
 */
uint8_t acil_decode_evt_hw_error(uint8_t *buffer_in, aci_evt_params_hw_error_t *p_aci_evt_params_hw_error);

/** @brief Decode the ACI event data credit
 *
 *  @param[in]      buffer_in                 Pointer to message received
 *  @param[in,out]  p_evt_params_data_credit  Pointer to the decoded message in ::aci_evt_params_data_credit_t
 *
 *  @return         None
 */
void acil_decode_evt_credit(uint8_t *buffer_in, aci_evt_params_data_credit_t *p_evt_params_data_credit);

/** @brief Decode the ACI event for connected
 *
 *  @param[in]      buffer_in                   Pointer to message received
 *  @param[in,out]  p_aci_evt_params_connected  Pointer to the decoded message in ::aci_evt_params_connected_t
 *
 *  @return         None
 */
void acil_decode_evt_connected(uint8_t *buffer_in, aci_evt_params_connected_t *p_aci_evt_params_connected);

/** @brief Decode the ACI event for timing
 *
 *  @param[in]      buffer_in             Pointer to message received
 *  @param[in,out]  p_evt_params_timing   Pointer to the decoded message in ::aci_evt_params_timing_t
 *
 *  @return         None
 */
void acil_decode_evt_timing(uint8_t *buffer_in, aci_evt_params_timing_t *p_evt_params_timing);

/** @brief Decode the ACI event for pipe error
 *
 *  @param[in]      buffer_in                 Pointer to message received
 *  @param[in,out]  p_evt_params_pipe_error   Pointer to the decoded message in ::aci_evt_params_pipe_error_t
 *
 */
void acil_decode_evt_pipe_error(uint8_t *buffer_in, aci_evt_params_pipe_error_t *p_evt_params_pipe_error);

/** @brief Decode the ACI event for key request
 *
 *  @param[in]      buffer_in               Pointer to message received
 *  @param[in,out]  p_evt_params_key_type   Pointer to the decoded message in ::aci_evt_params_key_type_t
 *
 *  @return         None
 */
void acil_decode_evt_key_request(uint8_t *buffer_in, aci_evt_params_key_request_t *p_evt_params_key_request);

/** @brief Decode the ACI event for echo
 *
 *  @param[in]      buffer_in    Pointer to message received
 *  @param[in,out]  buffer_out   Pointer to the echo message (max size of buffer ::ACI_ECHO_DATA_MAX_LEN)
 *
 *  @return         size         Received echo message size
 */
uint8_t acil_decode_evt_echo(uint8_t *buffer_in, aci_evt_params_echo_t *buffer_out);

/** @brief Decode the ACI event
 *
 *  @param[in]      buffer_in   Pointer to message received
 *  @param[in,out]  p_aci_evt   Pointer to the decoded message in ::aci_evt_t
 *
 *  @return         bool         true, if succesful, else returns false
 */
bool acil_decode_evt(uint8_t *buffer_in, aci_evt_t *p_aci_evt);

/** @brief Decode the Display Key Event
 *
 *  @param[in]      buffer_in   Pointer to message received
 *  @param[in,out]  p_aci_evt   Pointer to the decoded message in ::aci_evt_params_display_passkey_t
 *
 *  @return         None
 */
void acil_decode_evt_display_passkey(uint8_t *buffer_in, aci_evt_params_display_passkey_t *p_aci_evt_params_display_passkey);

#endif /* _acilib_IF_H_ */
