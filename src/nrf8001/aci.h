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
 * @ingroup aci
 */

/**
 * @defgroup aci aci
 * @{
 * @ingroup aci-lib
 *
 * @brief Definitions for the ACI (Application Control Interface)
 * @remarks
 *
 * Flow control from application mcu to nRF8001
 *
 * Data flow control:
 * The flow control is credit based and the credit is initally given using the "device started" event.
 * A credit of more than 1 is given to the application mcu.
 * These credits are used only after the "ACI Connected Event" is sent to the application mcu.
 *
 * every send_data that is used decrements the credit available by 1. This is to be tracked by the application mcu.
 * When the credit available reaches 0, the application mcu shall not send any more send_data.
 * Credit is returned using the "credit event", this returned credit can then be used to send more send_data.
 * This flow control is not necessary and not available for Broadcast.
 * The entire credit available with the external mcu expires when a "disconnected" event arrives.
 *
 * Command flow control:
 * When a command is sent over the ACI, the next command shall not be sent until after a response
 * for the command sent has arrived.
 *
 */

#ifndef ACI_H__
#define ACI_H__

/**
 * Define an _aci_packed_ macro we can use in structure and enumerated type
 * declarations so that the types are sized consistently across different
 * platforms. In particular Arduino platforms using the GCC compiler and the
 * Nordic processors using the Keil compiler.
 *
 * It's really the GNU compiler platforms that need a special keyword to get
 * tight packing of values. On GNU platforms we can use the keyword:
 *     __attribute__((__packed__))
 * The thing is that while this keyword does the right thing with old and new
 * versions of the gcc (C) compiler it only works right with g++ (C++) compiler
 * versions that are version 4 or newer.
 */
#ifdef __GNUC__
#  if __GNUC__ >= 4
#    define _aci_packed_ __attribute__((__packed__))
#  else
#    error "older g++ versions don't handle packed attribute in typedefs"
#  endif
#else
#  define _aci_packed_
#endif

#include <stdint.h>
#include <cstddef>
#include <string.h>
#include <unistd.h>

/*
 * Define a macro that compares the size of the first parameter to the integer
 * value of the second parameter. If they do not match, a compile time error
 * for negative array size occurs (even gnu chokes on negative array size).
 *
 * This compare is done by creating a typedef for an array. No variables are
 * created and no memory is consumed with this check. The created type is
 * used for checking only and is not for use by any other code. The value
 * of 10 in this macro is arbitrary, it just needs to be a value larger
 * than one to result in a positive number for the array size.
 */
#define ACI_ASSERT_SIZE(x,y) typedef char x ## _assert_size_t[-1+10*(sizeof(x) == (y))]

/**
 * @def ACI_VERSION
 * @brief Current ACI protocol version. 0 means a device that is not yet released.
 * A numer greater than 0 refers to a specific ACI version documented and released.
 * The ACI consists of the ACI commands, ACI events and error codes.
 */
#define ACI_VERSION   (0x02)
/**
 * @def BTLE_DEVICE_ADDRESS_SIZE
 * @brief Size in bytes of a Bluetooth Address
 */
#define BTLE_DEVICE_ADDRESS_SIZE                 (6)
/**
 * @def ACI_PACKET_MAX_LEN
 * @brief Maximum length in bytes of a full ACI packet, including length prefix, opcode and payload
 */
#define ACI_PACKET_MAX_LEN                       (32)
/**
 * @def ACI_ECHO_DATA_MAX_LEN
 * @brief Maximum length in bytes of the echo data portion
 */
#define ACI_ECHO_DATA_MAX_LEN                    (ACI_PACKET_MAX_LEN - 3)
/**
 * @def ACI_DEVICE_MAX_PIPES
 * @brief Maximum number of ACI pipes
 */
#define ACI_DEVICE_MAX_PIPES                       (62)
/**
 * @def ACI_PIPE_TX_DATA_MAX_LEN
 * @brief Maximum length in bytes of a transmission data pipe packet
 */
#define ACI_PIPE_TX_DATA_MAX_LEN                   (20)
/**
 * @def ACI_PIPE_RX_DATA_MAX_LEN
 * @brief Maximum length in bytes of a reception data pipe packet
 */
#define ACI_PIPE_RX_DATA_MAX_LEN                   (22)
/**
 * @def ACI_GAP_DEVNAME_MAX_LEN
 * @brief Maximum length in bytes of the GAP device name
 */
#define ACI_GAP_DEVNAME_MAX_LEN                 (20)
/**
 * @def ACI_AD_PACKET_MAX_LEN
 * @brief Maximum length in bytes of an AD packet
 */
#define ACI_AD_PACKET_MAX_LEN                   (31)
/**
 * @def ACI_AD_PACKET_MAX_USER_LEN
 * @brief Maximum usable length in bytes of an AD packet
 */
#define ACI_AD_PACKET_MAX_USER_LEN              (31 - 3)
/**
 * @def ACI_PIPE_INVALID
 * @brief Invalid pipe number
 */
#define ACI_PIPE_INVALID                        (0xFF)

/**
 * @enum aci_pipe_store_t
 * @brief Storage type identifiers: local and remote
 */
typedef enum
{
  ACI_STORE_INVALID = 0x0,
  ACI_STORE_LOCAL= 0x01,
  ACI_STORE_REMOTE= 0x02
} _aci_packed_ aci_pipe_store_t;

/**
 * @enum aci_pipe_type_t
 * @brief Pipe types
 */
typedef enum
{
  ACI_TX_BROADCAST = 0x0001,
  ACI_TX           = 0x0002,
  ACI_TX_ACK       = 0x0004,
  ACI_RX           = 0x0008,
  ACI_RX_ACK       = 0x0010,
  ACI_TX_REQ       = 0x0020,
  ACI_RX_REQ       = 0x0040,
  ACI_SET          = 0x0080,
  ACI_TX_SIGN      = 0x0100,
  ACI_RX_SIGN      = 0x0200,
  ACI_RX_ACK_AUTO  = 0x0400
} _aci_packed_ aci_pipe_type_t;

ACI_ASSERT_SIZE(aci_pipe_type_t, 2);

/**
 * @enum aci_bd_addr_type_t
 * @brief Bluetooth Address types
 */
typedef enum
{
  ACI_BD_ADDR_TYPE_INVALID  = 0x00,
  ACI_BD_ADDR_TYPE_PUBLIC  = 0x01,
  ACI_BD_ADDR_TYPE_RANDOM_STATIC  = 0x02,
  ACI_BD_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE  = 0x03,
  ACI_BD_ADDR_TYPE_RANDOM_PRIVATE_UNRESOLVABLE  = 0x04
} _aci_packed_ aci_bd_addr_type_t;

/**
 * @enum aci_device_output_power_t
 * @brief Radio output power levels
 */
typedef enum
{
  ACI_DEVICE_OUTPUT_POWER_MINUS_18DBM = 0x00, /**< Output power set to -18dBm */
  ACI_DEVICE_OUTPUT_POWER_MINUS_12DBM = 0x01, /**< Output power set to -12dBm */
  ACI_DEVICE_OUTPUT_POWER_MINUS_6DBM  = 0x02, /**< Output power set to -6dBm  */
  ACI_DEVICE_OUTPUT_POWER_0DBM  = 0x03  /**< Output power set to 0dBm   - DEFAULT*/
} _aci_packed_ aci_device_output_power_t;

/**
 * @enum aci_device_operation_mode_t
 * @brief Device operation modes
 */
typedef enum
{
  ACI_DEVICE_INVALID   =0x00,
  ACI_DEVICE_TEST      =0x01,
  ACI_DEVICE_SETUP     =0x02,
  ACI_DEVICE_STANDBY   =0x03,
  ACI_DEVICE_SLEEP     =0x04
} _aci_packed_ aci_device_operation_mode_t;

/**
 * @enum aci_disconnect_reason_t
 * @brief Reason enumeration for ACI_CMD_DISCONNECT
 */
typedef enum
{
  ACI_REASON_TERMINATE      =0x01, /**< Use this to disconnect (does a terminate request), you need to wait for the "disconnected" event */
  ACI_REASON_BAD_TIMING     =0x02 /*<Use this to disconnect and inform the peer, that the timing on the link is not acceptable for the device, you need to wait for the "disconnected" event */
} _aci_packed_ aci_disconnect_reason_t;

/**
 * @enum aci_test_mode_change_t
 * @brief Device test mode control
 */
typedef enum
{
  ACI_TEST_MODE_DTM_UART    = 0x01,
  ACI_TEST_MODE_DTM_ACI     = 0x02,
  ACI_TEST_MODE_EXIT        = 0xFF

} _aci_packed_ aci_test_mode_change_t;

ACI_ASSERT_SIZE(aci_test_mode_change_t, 1);

/**
 * @enum aci_permissions_t
 * @brief Data store permissions
 */
typedef enum
{
  ACI_PERMISSIONS_NONE               =0x00,
  ACI_PERMISSIONS_LINK_AUTHENTICATED =0x01
} _aci_packed_ aci_permissions_t;

/**
 * @def ACI_VS_UUID_128_MAX_COUNT
 * @brief Maximum number of 128-bit Vendor Specific
 *        UUIDs that can be set
 */
#define ACI_VS_UUID_128_MAX_COUNT  64 /** #0 reserved for invalid, #1 reservered for BT SIG and a maximum of 1024 bytes (16*64) */

/**
 * @struct aci_ll_conn_params_t
 * @brief Link Layer Connection Parameters
 */
typedef struct
{
  uint16_t min_conn_interval;   /**< Minimum connection interval requested from peer */
    #define ACI_PPCP_MIN_CONN_INTVL_NONE  0xFFFF
    #define ACI_PPCP_MIN_CONN_INTVL_MIN   0x0006
    #define ACI_PPCP_MIN_CONN_INTVL_MAX   0x0C80
  uint16_t max_conn_interval;   /**< Maximum connection interval requested from peer */
    #define ACI_PPCP_MAX_CONN_INTVL_NONE  0xFFFF
    #define ACI_PPCP_MAX_CONN_INTVL_MIN   0x0006
    #define ACI_PPCP_MAX_CONN_INTVL_MAX   0x0C80
  uint16_t slave_latency;       /**< Connection interval latency requested from peer */
    #define ACI_PPCP_SLAVE_LATENCY_MAX    0x03E8
  uint16_t timeout_mult;        /**< Link supervisor timeout multiplier requested from peer */
    #define ACI_PPCP_TIMEOUT_MULT_NONE    0xFFFF
    #define ACI_PPCP_TIMEOUT_MULT_MIN     0x000A
    #define ACI_PPCP_TIMEOUT_MULT_MAX     0x0C80
} _aci_packed_ aci_ll_conn_params_t;

/**
 * @def aci_gap_ppcp_t
 * @brief GAP Peripheral Preferred Connection Parameters
 */
#define aci_gap_ppcp_t aci_ll_conn_params_t

/**
 * @def ACI_AD_LOC_SVCUUID_16_MAX_COUNT
 * @brief Maximum number of 16-bit UUIDs that can
 *        be inserted in the Services tag of AD
 */
#define ACI_AD_LOC_SVCUUID_16_MAX_COUNT  5

/**
 * @def ACI_AD_LOC_SVCUUID_128_MAX_COUNT
 * @brief Maximum number of 128-bit UUIDs that can
 *        be inserted in the Services tag of AD
 */
#define ACI_AD_LOC_SVCUUID_128_MAX_COUNT  1

/**
 * @def ACI_AD_SOL_SVCUUID_16_MAX_COUNT
 * @brief Maximum number of UUIDs that can
 *        be inserted in the Solicited Services tag of AD
 */
#define ACI_AD_SOL_SVCUUID_16_MAX_COUNT  5

/**
 * @def ACI_AD_SOL_SVCUUID_128_MAX_COUNT
 * @brief Maximum number of UUIDs that can
 *        be inserted in the Solicited Services tag of AD
 */
#define ACI_AD_SOL_SVCUUID_128_MAX_COUNT  1

/**
 * @def ACI_SEC_ENCKEY_SIZE_MIN
 * @brief Minimum encryption key size
 */
#define ACI_SEC_ENCKEY_SIZE_MIN        7
/**
 * @def ACI_SEC_ENCKEY_SIZE_MAX
 * @brief Maximum encryption key size
 */
#define ACI_SEC_ENCKEY_SIZE_MAX        16
/**
 * @def ACI_CUSTOM_AD_TYPE_MAX_COUNT
 * @brief Maximum number of custom ad types
 */
#define ACI_CUSTOM_AD_TYPE_MAX_COUNT 8
/**
 * @def ACI_CUSTOM_AD_TYPE_MAX_DATA_LENGTH
 * @brief Maximum custom ad type data size
 */
#define ACI_CUSTOM_AD_TYPE_MAX_DATA_LENGTH 20

/**
 * @struct aci_tx_data_t
 * @brief Generic ACI transmit data structure
 */
typedef struct
{
  uint8_t pipe_number;
  uint8_t aci_data[ACI_PIPE_TX_DATA_MAX_LEN];
} _aci_packed_ aci_tx_data_t;

ACI_ASSERT_SIZE(aci_tx_data_t, ACI_PIPE_TX_DATA_MAX_LEN + 1);

/**
 * @struct aci_rx_data_t
 * @brief Generic ACI receive data structure
 */
typedef struct
{
  uint8_t pipe_number;
  uint8_t aci_data[ACI_PIPE_RX_DATA_MAX_LEN];
} _aci_packed_ aci_rx_data_t;

ACI_ASSERT_SIZE(aci_rx_data_t, ACI_PIPE_RX_DATA_MAX_LEN + 1);

/**
 * @enum aci_hw_error_t
 * @brief Hardware Error codes
 */
typedef enum
{
  ACI_HW_ERROR_NONE     = 0x00,
  ACI_HW_ERROR_FATAL    = 0x01
} _aci_packed_ aci_hw_error_t;

/**
 * @enum aci_clock_accuracy_t
 * @brief Bluetooth Low Energy Clock Accuracy
 */
typedef enum
{
  ACI_CLOCK_ACCURACY_500_PPM = 0x00,
  ACI_CLOCK_ACCURACY_250_PPM = 0x01,
  ACI_CLOCK_ACCURACY_150_PPM = 0x02,
  ACI_CLOCK_ACCURACY_100_PPM = 0x03,
  ACI_CLOCK_ACCURACY_75_PPM  = 0x04,
  ACI_CLOCK_ACCURACY_50_PPM  = 0x05,
  ACI_CLOCK_ACCURACY_30_PPM  = 0x06,
  ACI_CLOCK_ACCURACY_20_PPM  = 0x07
} _aci_packed_ aci_clock_accuracy_t;

/**
 * @enum aci_app_latency_mode_t
 * @brief Application latency modes
 */
typedef enum
{
  ACI_APP_LATENCY_DISABLE = 0,
  ACI_APP_LATENCY_ENABLE = 1
} _aci_packed_ aci_app_latency_mode_t;

/**
 * @enum gatt_format_t
 * @brief GATT format definitions
 */
typedef enum
{
  ACI_GATT_FORMAT_NONE        = 0x00, /**< No characteristic format available */
  ACI_GATT_FORMAT_BOOLEAN     = 0x01, /**< Not Supported */
  ACI_GATT_FORMAT_2BIT        = 0x02, /**< Not Supported */
  ACI_GATT_FORMAT_NIBBLE      = 0x03, /**< Not Supported */
  ACI_GATT_FORMAT_UINT8       = 0x04,
  ACI_GATT_FORMAT_UINT12      = 0x05,
  ACI_GATT_FORMAT_UINT16      = 0x06,
  ACI_GATT_FORMAT_UINT24      = 0x07,
  ACI_GATT_FORMAT_UINT32      = 0x08,
  ACI_GATT_FORMAT_UINT48      = 0x09,
  ACI_GATT_FORMAT_UINT64      = 0x0A,
  ACI_GATT_FORMAT_UINT128     = 0x0B,
  ACI_GATT_FORMAT_SINT8       = 0x0C,
  ACI_GATT_FORMAT_SINT12      = 0x0D,
  ACI_GATT_FORMAT_SINT16      = 0x0E,
  ACI_GATT_FORMAT_SINT24      = 0x0F,
  ACI_GATT_FORMAT_SINT32      = 0x10,
  ACI_GATT_FORMAT_SINT48      = 0x11,
  ACI_GATT_FORMAT_SINT64      = 0x12,
  ACI_GATT_FORMAT_SINT128     = 0x13,
  ACI_GATT_FORMAT_FLOAT32     = 0x14,
  ACI_GATT_FORMAT_FLOAT64     = 0x15,
  ACI_GATT_FORMAT_SFLOAT      = 0x16,
  ACI_GATT_FORMAT_FLOAT       = 0x17,
  ACI_GATT_FORMAT_DUINT16     = 0x18,
  ACI_GATT_FORMAT_UTF8S       = 0x19,
  ACI_GATT_FORMAT_UTF16S      = 0x1A,
  ACI_GATT_FORMAT_STRUCT      = 0x1B
} _aci_packed_ aci_gatt_format_t;

/**
 * @brief GATT Bluetooth namespace
 */
typedef enum
{
  ACI_GATT_NAMESPACE_INVALID  = 0x00,
  ACI_GATT_NAMESPACE_BTSIG    = 0x01 /**< Bluetooth SIG */
} _aci_packed_ aci_gatt_namespace_t;

/**
 * @brief Security key types
 */
typedef enum
{
  ACI_KEY_TYPE_INVALID  = 0x00,
  ACI_KEY_TYPE_PASSKEY  = 0x01
} _aci_packed_ aci_key_type_t;

/**
 * @enum aci_bond_status_code_t
 * @brief Bond status code
 */
typedef enum
{
 /**
  * Bonding succeeded
  */
  ACI_BOND_STATUS_SUCCESS                             = 0x00,
 /**
  * Bonding failed
  */
  ACI_BOND_STATUS_FAILED                              = 0x01,
 /**
  * Bonding error: Timeout can occur when link termination is unexpected or did not get connected OR SMP timer expired
  */
  ACI_BOND_STATUS_FAILED_TIMED_OUT                    = 0x02,
 /**
  * Bonding error: Passkey entry failed
  */
  ACI_BOND_STATUS_FAILED_PASSKEY_ENTRY_FAILED        = 0x81,
 /**
  * Bonding error: OOB unavailable
  */
  ACI_BOND_STATUS_FAILED_OOB_UNAVAILABLE             = 0x82,
 /**
  * Bonding error: Authentication request failed
  */
  ACI_BOND_STATUS_FAILED_AUTHENTICATION_REQ          = 0x83,
 /**
  * Bonding error: Confirm value failed
  */
  ACI_BOND_STATUS_FAILED_CONFIRM_VALUE               = 0x84,
 /**
  * Bonding error: Pairing unsupported
  */
  ACI_BOND_STATUS_FAILED_PAIRING_UNSUPPORTED         = 0x85,
 /**
  * Bonding error: Invalid encryption key size
  */
  ACI_BOND_STATUS_FAILED_ENCRYPTION_KEY_SIZE         = 0x86,
 /**
  * Bonding error: Unsupported SMP command
  */
  ACI_BOND_STATUS_FAILED_SMP_CMD_UNSUPPORTED         = 0x87,
 /**
  * Bonding error: Unspecified reason
  */
  ACI_BOND_STATUS_FAILED_UNSPECIFIED_REASON          = 0x88,
 /**
  * Bonding error: Too many attempts
  */
  ACI_BOND_STATUS_FAILED_REPEATED_ATTEMPTS           = 0x89,
 /**
  * Bonding error: Invalid parameters
  */
  ACI_BOND_STATUS_FAILED_INVALID_PARAMETERS          = 0x8A

} _aci_packed_ aci_bond_status_code_t;

ACI_ASSERT_SIZE(aci_bond_status_code_t, 1);

/**
 * @enum aci_bond_status_source_t
 * @brief Source of a bond status code
 */
typedef enum
{
  ACI_BOND_STATUS_SOURCE_INVALID                  = 0x00,
  ACI_BOND_STATUS_SOURCE_LOCAL                    = 0x01,
  ACI_BOND_STATUS_SOURCE_REMOTE                   = 0x02

} _aci_packed_ aci_bond_status_source_t;

/**
 * @enum aci_status_code_t
 * @brief ACI status codes
 */
typedef enum
{
 /**
  * Success
  */
  ACI_STATUS_SUCCESS                                        = 0x00,
 /**
  * Transaction continuation status
  */
  ACI_STATUS_TRANSACTION_CONTINUE                           = 0x01,
 /**
  * Transaction completed
  */
  ACI_STATUS_TRANSACTION_COMPLETE                           = 0x02,
 /**
  * Extended status, further checks needed
  */
  ACI_STATUS_EXTENDED                                       = 0x03,
 /**
  * Unknown error.
  */
  ACI_STATUS_ERROR_UNKNOWN                                  = 0x80,
 /**
  * Internal error.
  */
  ACI_STATUS_ERROR_INTERNAL                                 = 0x81,
 /**
  * Unknown command
  */
  ACI_STATUS_ERROR_CMD_UNKNOWN                              = 0x82,
 /**
  * Command invalid in the current device state
  */
  ACI_STATUS_ERROR_DEVICE_STATE_INVALID                     = 0x83,
 /**
  * Invalid length
  */
  ACI_STATUS_ERROR_INVALID_LENGTH                           = 0x84,
 /**
  * Invalid input parameters
  */
  ACI_STATUS_ERROR_INVALID_PARAMETER                        = 0x85,
 /**
  * Busy
  */
  ACI_STATUS_ERROR_BUSY                                     = 0x86,
 /**
  * Invalid data format or contents
  */
  ACI_STATUS_ERROR_INVALID_DATA                             = 0x87,
 /**
  * CRC mismatch
  */
  ACI_STATUS_ERROR_CRC_MISMATCH                             = 0x88,
 /**
  * Unsupported setup format
  */
  ACI_STATUS_ERROR_UNSUPPORTED_SETUP_FORMAT                 = 0x89,
 /**
  * Invalid sequence number during a write dynamic data sequence
  */
  ACI_STATUS_ERROR_INVALID_SEQ_NO                           = 0x8A,
 /**
  * Setup data is locked and cannot be modified
  */
  ACI_STATUS_ERROR_SETUP_LOCKED                             = 0x8B,
 /**
  * Setup error due to lock verification failure
  */
  ACI_STATUS_ERROR_LOCK_FAILED                              = 0x8C,
 /**
  * Bond required: Local Pipes need bonded/trusted peer
  */
  ACI_STATUS_ERROR_BOND_REQUIRED                            = 0x8D,
 /**
  * Command rejected as a transaction is still pending
  */
  ACI_STATUS_ERROR_REJECTED                                 = 0x8E,
  /**
  * Pipe Error Event : Data size exceeds size specified for pipe : Transmit failed
  */
  ACI_STATUS_ERROR_DATA_SIZE                                = 0x8F,
 /**
  * Pipe Error Event : Invalid pipe
  */
  ACI_STATUS_ERROR_PIPE_INVALID                             = 0x90,
 /**
  * Pipe Error Event : Credit not available
  */
  ACI_STATUS_ERROR_CREDIT_NOT_AVAILABLE                     = 0x91,
 /**
  * Pipe Error Event : Peer device has sent an error on an pipe operation on the remote characteristic
  */
  ACI_STATUS_ERROR_PEER_ATT_ERROR                           = 0x92,
 /**
  * Connection was not established before the BTLE advertising was stopped
  */
  ACI_STATUS_ERROR_ADVT_TIMEOUT                             = 0x93,
 /**
  * Peer has triggered a Security Manager Protocol Error
  */
  ACI_STATUS_ERROR_PEER_SMP_ERROR                           = 0x94,
 /**
  * Pipe Error Event : Pipe type invalid for the selected operation
  */
  ACI_STATUS_ERROR_PIPE_TYPE_INVALID                        = 0x95,
 /**
  * Pipe Error Event : Pipe state invalid for the selected operation
  */
  ACI_STATUS_ERROR_PIPE_STATE_INVALID                       = 0x96,
 /**
  * Invalid key size provided
  */
  ACI_STATUS_ERROR_INVALID_KEY_SIZE                         = 0x97,
 /**
  * Invalid key data provided
  */
  ACI_STATUS_ERROR_INVALID_KEY_DATA                         = 0x98,
 /**
  * Reserved range start
  */
  ACI_STATUS_RESERVED_START                                 = 0xF0,
 /**
  * Reserved range end
  */
  ACI_STATUS_RESERVED_END                                   = 0xFF

} _aci_packed_ aci_status_code_t;

ACI_ASSERT_SIZE(aci_status_code_t, 1);

/**
 * @}
 */

#endif // ACI_H__
