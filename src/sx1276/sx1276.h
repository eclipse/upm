/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Semtech for their example code at:
 * https://github.com/Lora-net/LoRaMac-node
 * released under a modified BSD license, for many clues as to how to
 * initialize and operate this radio properly.
 * See src/sx1276/LICENSE.txt
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <string>

#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include <mraa/common.hpp>
#include <mraa/spi.hpp>
#include <mraa/gpio.hpp>

// Our crystal oscillator frequency (32Mhz)
#define FXOSC_FREQ 32000000.0

// Our freq stepping resolution (in Hz) if FXOSC_FREQ is 32Mhz
// (FXOSC_FREQ / 2^19) =
#define FXOSC_STEP 61.03515625

namespace upm {
  
  /**
   * @brief SX1276 LoRa/FSK modem
   * @defgroup sx1276 libupm-sx1276
   * @ingroup spi gpio wifi
   */

  /**
   * @library sx1276
   * @sensor sx1276
   * @comname SX1276 LoRa/FSK modem
   * @altname SX1277 SX1278 SX1279
   * @type wifi
   * @man semtech
   * @con spi gpio
   * @web http://www.digikey.com/product-search/en?vendor=0&keywords=SX1276MB1LAS
   *
   * @brief API for the SX1276 LoRa/FSK modem
   *
   * The SX1276 is a FSK/OOK/LoRa modem capable of both Low Frequency
   * and High Frequency communication.
   *
   * It requires a 3.3v power supply, do not use 5v.
   *
   * Frequency Hopping Spread Spectrum (FHSS) is not currently supported.
   *
   * While not all of the functionality of this device is supported
   * initially, methods and register definitions are provided that
   * should allow an end user to implement whatever features are
   * required.
   *
   * FSK send/receive example
   * @snippet sx1276-fsk.cxx Interesting
   * LORA send/receive example
   * @snippet sx1276-lora.cxx Interesting
   */

  class SX1276 {
  public:

    // The default chip revision
    static const uint8_t chipRevision = 0x12;


    // total FIFO size
    static const int FIFO_SIZE = 256;

    // differentiator between high and low bands
    static const int RF_MID_BAND_THRESH = 525000000;

    // LoRa RSSI offsets depending on LF or HF bands
    static const int LOR_RSSI_OFFSET_HF = -157;
    static const int LOR_RSSI_OFFSET_LF = -164;

    /**
     * What modem we are configured for
     */
    typedef enum {
      MODEM_LORA                         = 0,
      MODEM_FSK
    } RADIO_MODEM_T;

    /**
     * Events that can occurr during a TX or RX operation.
     *
     * When sending or receiving a packet (calling setTx()/send() or
     * setRx()), the state will be initialized to ESTATE_EXEC to
     * indicate the operation is in progress.  Once an event has
     * occurred, this state will be updated accordingly.
     *
     * For receiving, if RX_DONE is set, then it is safe to retrieve
     * the rx buffer (via getRxBuffer()/getRxBufferStr()) as well as
     * query the RSSI and SNR.  On RX_ERROR, these values will be the
     * same as they were last set during the last RX_DONE event.
     */
    typedef enum {
      REVENT_DONE                       = 0, // operation completed successfully
      REVENT_EXEC,                           // runninsg something
      REVENT_ERROR,                          // failed, crc error, sync timeout
      REVENT_TIMEOUT                         // timed out
    } RADIO_EVENT_T;

    /**
     * SX1276 registers
     *
     * NOTE: reserved registers must not be written into or read from.
     * Reserved bitfields must always be 0.  
     *
     * This device has a set of "common" registers, as well as
     * registers that represent different things depending on whether
     * the device in in LoRa mode or FSK/OOK mode.  So here, we will
     * prefix the register names with COM (common), LOR (LoRa mode),
     * and FSK (FSK/OOK mode) accordingly.
     */
    typedef enum {
      COM_RegFifo                      = 0x00, // FIFO r/w access
      COM_RegOpMode                    = 0x01, // LoRa/FSK

      FSK_RegBitrateMsb                = 0x02,
      LOR_Reserved02                   = 0x02, // reserved

      FSK_RegBitrateLsb                = 0x03,
      LOR_Reserved03                   = 0x03, // reserved

      FSK_RegFdevMsb                   = 0x04, // freq deviation
      LOR_Reserved04                   = 0x04, // reserved

      FSK_RegFdevLsb                   = 0x05,
      LOR_Reserved05                   = 0x05, // reserved
      
      COM_RegFrfMsb                    = 0x06, // carrier freq
      COM_RegFrfMid                    = 0x07,
      COM_RegFrfLsb                    = 0x08,
      COM_RegPaConfig                  = 0x09,
      COM_RegPaRamp                    = 0x0a,

      COM_RegOcp                       = 0x0b, // overcurrent protection
      COM_RegLna                       = 0x0c,

      FSK_RegRxConfig                  = 0x0d,
      LOR_RegFifoAddrPtr               = 0x0d,

      FSK_RegRssiConfg                 = 0x0e,
      LOR_RegFifoTxBaseAddr            = 0x0e,

      FSK_RegRssiCollision             = 0x0f,
      LOR_RegFifoRxBaseAddr            = 0x0f,

      FSK_RegRssiThresh                = 0x10,
      LOR_RegFifoRxCurrentAddr         = 0x10,

      FSK_RegRssiValue                 = 0x11,
      LOR_RegIrqFlagsMask              = 0x11,

      FSK_RegRxBw                      = 0x12,
      LOR_RegIrqFlags                  = 0x12,

      FSK_RegAfcBw                     = 0x13, // automatic freq cntrl
      LOR_RegRxNbBytes                 = 0x13, // received pkt len

      FSK_RegOokPeak                   = 0x14,
      LOR_RegRxHeaderCntValueMsb       = 0x14,

      FSK_RegOokFix                    = 0x15,
      LOR_RegRxHeaderCntValueLsb       = 0x15,

      FSK_RegOokAvg                    = 0x16,
      LOR_RegRxPacketCntValueMsb       = 0x16,

      FSK_Reserved17                   = 0x17, // reserved
      LOR_RegRxPacketCntValueLsb       = 0x17,

      FSK_Reserved18                   = 0x18, // reserved
      LOR_RegModemStat                 = 0x18,

      FSK_Reserved19                   = 0x19, // reserved
      LOR_RegPktSnrValue               = 0x19,

      FSK_RegAfcFei                    = 0x1a,
      LOR_RegPktRssiValue              = 0x1a,

      FSK_RegAfcMsb                    = 0x1b,
      LOR_RegRssiValue                 = 0x1b,

      FSK_RegAfcLsb                    = 0x1c,
      LOR_RegHopChannel                = 0x1c, // fhss starting channel

      FSK_RegFeiMsb                    = 0x1d,
      LOR_RegModemConfig1              = 0x1d,

      FSK_RegFeiLsb                    = 0x1e,
      LOR_RegModemConfig2              = 0x1e,

      FSK_RegPreambleDetect            = 0x1f,
      LOR_RegSymbTimeoutLsb            = 0x1f,

      FSK_RegRxTimeout1                = 0x20,
      LOR_RegPreambleMsb               = 0x20,

      FSK_RegRxTimeout2                = 0x21,
      LOR_RegPreambleLsb               = 0x21,

      FSK_RegRxTimeout3                = 0x22,
      LOR_RegPayloadLength             = 0x22,

      FSK_RegRxDelay                   = 0x23,
      LOR_RegMaxPayloadLength          = 0x23,

      FSK_RegOsc                       = 0x24,
      LOR_RegHopPeriod                 = 0x24,

      FSK_RegPreambleMsb               = 0x25,
      LOR_RegFifoRxByteAddr            = 0x25,

      FSK_RegPreambleLsb               = 0x26,
      LOR_RegModemConfig3              = 0x26,

      FSK_RegSyncConfig                = 0x27,
      LOR_Reserved27                   = 0x27, // reserved

      FSK_RegSyncValue1                = 0x28,
      LOR_RegFeiMsb                    = 0x28,
      
      FSK_RegSyncValue2                = 0x29,
      LOR_RegFeiMid                    = 0x29,

      FSK_RegSyncValue3                = 0x2a,
      LOR_RegFeiLsb                    = 0x2a,

      FSK_RegSyncValue4                = 0x2b,
      LOR_Reserved2b                   = 0x2b, // reserved

      FSK_RegSyncValue5                = 0x2c,
      LOR_RegRssiWideband              = 0x2c,

      FSK_RegSyncValue6                = 0x2d,
      LOR_Reserved2d                   = 0x2d, // reserved
      
      FSK_RegSyncValue7                = 0x2e,
      LOR_Reserved2e                   = 0x2e, // reserved
      
      FSK_RegSyncValue8                = 0x2f,
      LOR_Reserved2f                   = 0x2f, // reserved

      FSK_RegPacketConfig1             = 0x30,
      LOR_Reserved30                   = 0x30, // reserved

      FSK_RegPacketConfig2             = 0x31,
      LOR_RegDetectOptimize            = 0x31,

      FSK_RegPayloadLength             = 0x32,
      LOR_Reserved32                   = 0x32, // reserved

      FSK_RegNodeAddr                  = 0x33,
      LOR_RegInvertIQ                  = 0x33,

      FSK_RegBroadcastAddr             = 0x34,
      LOR_Reserved34                   = 0x34, // reserved
      
      FSK_RegFifoThresh                = 0x35,
      LOR_Reserved35                   = 0x35, // reserved
      
      FSK_RegSeqConfig1                = 0x36,
      LOR_Reserved36                   = 0x36, // reserved

      FSK_RegSeqConfig2                = 0x37,
      LOR_RegDetectionThreshold        = 0x37,

      FSK_RegTimerResol                = 0x38,
      LOR_Reserved38                   = 0x38, // reserved

      FSK_RegTimer1Coeff               = 0x39,
      LOR_RegSyncWord                  = 0x39,

      FSK_RegTimer2Coeff               = 0x3a,
      LOR_Reserved3a                   = 0x3a, // reserved

      FSK_RegImageCal                  = 0x3b,
      LOR_Reserved3b                   = 0x3b, // reserved (in datasheet)?
      LOR_RegInvertIQ2                 = 0x3b, // does not exist in datasheet
                                               // but used in Semtech code.
                                               // UNDOCUMENTED

      FSK_RegTemp                      = 0x3c,
      LOR_Reserved3c                   = 0x3c, // reserved

      FSK_RegLowBat                    = 0x3d,
      LOR_Reserved3d                   = 0x3d, // reserved

      FSK_RegIrqFlags1                 = 0x3e,
      LOR_Reserved3e                   = 0x3e, // reserved

      FSK_RegIrqFlags2                 = 0x3f,
      LOR_Reserved3f                   = 0x3f, // reserved

      COM_RegDioMapping1               = 0x40, // DIO0-DIO3
      COM_RegDioMapping2               = 0x41, // DIO4-DIO5, clk out freq

      COM_RegVersion                   = 0x42, // Semtech ID (silicon revision)

      // 0x43 reserved

      // The data sheet says this is FSK only, but the semtech code
      // implies this is only valid for LoRa.  So for now, assume the
      // datasheet is wrong.
      //
      // FSK_RegPllHop                    = 0x44, 
      // LOR_Reserved44                   = 0x44, // reserved 

      FSK_Reserved44                   = 0x44,
      LOR_RegPllHop                    = 0x44,

      // 0x45-0x4a reserved

      COM_RegTcxo                      = 0x4b,

      // 0x4c reserved

      COM_RegPaDac                     = 0x4d,

      // 0x4e-0x5a reserved

      COM_RegFormerTemp                = 0x5b,

      // 0x5c reserved

      FSK_RegBitRateFrac               = 0x5d,
      LOR_Reserved5d                   = 0x5d, // reserved

      // 0x5e-0x60 reserved

      COM_RegAgcRef                    = 0x61,
      COM_RegAgcThresh1                = 0x62,
      COM_RegAgcThresh2                = 0x63,
      COM_RegAgcThresh3                = 0x64,

      // 0x65-0x6f reserved

      COM_RegPll                       = 0x70

      // 0x71-0xff reserved
    } SX1276_REGS_T;

    /**
     * OpMode register (differing bitfields depending on mode)
     */
    typedef enum {
      OPMODE_Mode0                     = 0x01, // operating modes (sleep, etc)
      OPMODE_Mode1                     = 0x02,
      OPMODE_Mode2                     = 0x04,
      _OPMODE_Mode_MASK                = 7,
      _OPMODE_Mode_SHIFT               = 0,

      OPMODE_LowFrequencyModeOn        = 0x08,

      // 0x10 reserved

      OPMODE_FSK_ModulationType0       = 0x20,
      OPMODE_FSK_ModulationType1       = 0x40,
      _OPMODE_FSK_ModulationType_MASK  = 3,
      _OPMODE_FSK_ModulationType_SHIFT = 5,

      OPMODE_LOR_Reserved0x20          = 0x20,

      OPMODE_LOR_AccessSharedReg       = 0x40, // tmp sw to FSK regs

      OPMODE_LongRangeMode             = 0x80 // LoRa mode enable(1), else FSK
    } OPMODE_BITS_T;


    /**
     * Mode values
     */
    typedef enum {
      MODE_Sleep                        = 0,
      MODE_Standby                      = 1,
      MODE_FSTX                         = 2, // freq synth
      MODE_TxMode                       = 3,
      MODE_FSRX                         = 4, // freq synth

      MODE_FSK_RxMode                   = 5,
      MODE_LOR_RxContinuous             = 5, // continuous rx mode

      MODE_FSK_Reserved6                = 6,
      MODE_LOR_RxSingle                 = 6, // single packet rx mode

      MODE_FSK_Reserved7                = 7,
      MODE_LOR_CAD                      = 7 // channel activity detection
    } MODE_T;

    /**
     * FSK_ModulationType values
     */
    typedef enum {
      MODULATION_FSK                    = 0, // freq shift keying
      MODULATION_OOK                    = 1, // on/off keying
      // 2-3 reserved
    } FSK_MODULATION_TYPE_T;

    /**
     * RegPaConfig register
     */
    typedef enum {
      PACONFIG_OutputPower0              = 0x01,
      PACONFIG_OutputPower1              = 0x02,
      PACONFIG_OutputPower2              = 0x04,
      PACONFIG_OutputPower3              = 0x08,
      _PACONFIG_OutputPower_MASK         = 15,
      _PACONFIG_OutputPower_SHIFT        = 0,

      PACONFIG_MaxPower0                 = 0x10,
      PACONFIG_MaxPower1                 = 0x20,
      PACONFIG_MaxPower2                 = 0x40,
      _PACONFIG_MaxPower_MASK            = 7,
      _PACONFIG_MaxPower_SHIFT           = 4,

      PACONFIG_PaSelect                  = 0x80 // PA output pin,
                                                //  0 = 14dBm, 1 = 20dBm
    } PACONFIG_BITS_T;

    /**
     * RegPaRamp register
     */
    typedef enum {
      PARAMP_PaRamp0                     = 0x01, // rise/fall of ramp up/down
      PARAMP_PaRamp1                     = 0x02,
      PARAMP_PaRamp2                     = 0x04,
      PARAMP_PaRamp3                     = 0x08,
      _PARAMP_PaRamp_MASK                = 15,
      _PARAMP_PaRamp_SHIFT               = 0,

      // 0x10 reserved

      // LORA 0x20-0x40 reserved

      PARAMP_FSK_ModulationShaping0      = 0x20,
      PARAMP_FSK_ModulationShaping1      = 0x40,
      _PARAMP_FSK_ModulationShaping_MASK = 3,
      _PARAMP_FSK_ModulationShaping_SHIFT = 5

      // 0x80 reserved
    } PARAMP_BITS_T;

    /**
     * PARAMP_PaRamp values
     */
    typedef enum {
      PARAMP_3_4MS                       = 0, // 3.4ms
      PARAMP_2MS                         = 1,
      PARAMP_1MS                         = 2,
      PARAMP_500US                       = 3, // 500us
      PARAMP_250US                       = 4,
      PARAMP_125US                       = 5,
      PARAMP_100US                       = 6,
      PARAMP_62US                        = 7,
      PARAMP_50US                        = 8,
      PARAMP_40US                        = 9,
      PARAMP_31US                        = 10,
      PARAMP_25US                        = 11,
      PARAMP_20US                        = 12,
      PARAMP_15US                        = 13,
      PARAMP_12US                        = 14,
      PARAMP_10US                        = 15
    } PARAMP_T;

    /**
     * PARAMP_ModulationShaping values.  Note, these mean different
     * things depending on whether you are using FSK or OOK.  Hence
     * the FSK/OOK dups.  We will also name these as 'MODSHAPING_', rather
     * than the lengthy 'MODULATIONSHAPING... '
     */
    typedef enum {
      MODSHAPING_NOSHAPING              = 0,

      // FSK
      MODSHAPING_FSK_GaussianFilterBT1  = 1, // BT = 1.0
      MODSHAPING_FSK_GaussianFilterBT05 = 2, // BT = 0.5
      MODSHAPING_FSK_GaussianFilterBT03 = 3, // BT = 0.3

      // OOK
      MODSHAPING_OOK_FCutoffBitRate     = 1, // Fcutoff = BitRate
      MODSHAPING_OOK_FCutoffBitRate2    = 2  // Fcutoff = 2*BitRate

      // for OOK, 3 is reserved
    } MODSHAPING_T;

    /**
     * RegOcp register (see datasheet for OcpTrim values)
     */
    typedef enum {
      OCP_OcpTrim0                      = 0x01,
      OCP_OcpTrim1                      = 0x02,
      OCP_OcpTrim2                      = 0x04,
      OCP_OcpTrim3                      = 0x08,
      _OCP_OcpTrim_MASK                 = 15,
      _OCP_OcpTrim_SHIFT                = 0,

      OCP_OcpOn                         = 0x10

      // 0x20-0x80 reserved
    } OCP_BITS_T;

    /**
     * Lna register
     */
    typedef enum {
      LNA_LnaBoostHf0                    = 0x01,
      LNA_LnaBoostHf1                    = 0x02,
      _LNA_LnaBoostHf_MASK               = 3,
      _LNA_LnaBoostHf_SHIFT              = 0,

      // 0x04 reserved

      LNA_LnaBoostLf0                    = 0x08,
      LNA_LnaBoostLf1                    = 0x10,
      _LNA_LnaBoostLf_MASK               = 3,
      _LNA_LnaBoostLf_SHIFT              = 3,

      LNA_LnaGain0                       = 0x20,
      LNA_LnaGain1                       = 0x40,
      LNA_LnaGain2                       = 0x80,
      _LNA_LnaGain_MASK                  = 7,
      _LNA_LnaGain_SHIFT                 = 5
    } LNA_BITS_T;

    /**
     * LnaBoostHf values
     */
    typedef enum {
      LNABOOSTHF_Default                = 0,
      // 1-2 reserved
      LNABOOSTHF_BoostOn                = 3, // 150% LNA current
    } LNABOOSTHF_T;

    /**
     * LnaBoostLf values
     */
    typedef enum {
      LNABOOSTLF_Default                = 0
      // 1-3 reserved
    } LNABOOSTLF_T;

    /**
     * LnaGain values
     */
    typedef enum {
      // 0 reserved
      LNAGAIN_G1                        = 1, // max gain
      LNAGAIN_G2                        = 2,
      LNAGAIN_G3                        = 3,
      LNAGAIN_G4                        = 4,
      LNAGAIN_G5                        = 5,
      LNAGAIN_G6                        = 6  // minimum gain
      // 7 reserved
    } LNAGAIN_T;

    /**
     * FSK_RxConfig register.  See Table 24 in the data sheet for
     * the meanings of the RxTrigger values.
     */
    typedef enum {
      RXCONFIG_RxTrigger0               = 0x01,
      RXCONFIG_RxTrigger1               = 0x02,
      RXCONFIG_RxTrigger2               = 0x04,
      _RXCONFIG_RxTrigger_MASK          = 7,
      _RXCONFIG_RxTrigger_SHIFT         = 0,

      RXCONFIG_AgcAutoOn                = 0x08,
      RXCONFIG_AfcAutoOn                = 0x10,
      RXCONFIG_RestartRxWithPllLock     = 0x20,
      RXCONFIG_RestartRxWithoutPllLock  = 0x40,
      RXCONFIG_RestartRxOnCollision     = 0x80
    } RXCONFIG_BITS_T;

    /**
     * FSK_RssiConfig register
     */
    typedef enum {
      RSSICONFIG_RssiSmoothing0         = 0x01, // RSSI sampling/averaging
      RSSICONFIG_RssiSmoothing1         = 0x02,
      RSSICONFIG_RssiSmoothing2         = 0x04,
      _RSSICONFIG_RssiSmoothing_MASK    = 7,
      _RSSICONFIG_RssiSmoothing_SHIFT   = 0,

      RSSICONFIG_RssiOffset0            = 0x08, // 2's complement offset
      RSSICONFIG_RssiOffset1            = 0x10,
      RSSICONFIG_RssiOffset2            = 0x20,
      RSSICONFIG_RssiOffset3            = 0x40,
      RSSICONFIG_RssiOffset4            = 0x80,
      _RSSICONFIG_RssiOffset_MASK       = 31,
      _RSSICONFIG_RssiOffset_SHIFT      = 3
    } RSSICONFIG_BITS_T;

    /**
     * RssiSmoothing values
     */
    typedef enum {
      RSSISMOOTHING_2                   = 0, // 2 samples used
      RSSISMOOTHING_4                   = 1,
      RSSISMOOTHING_8                   = 2,
      RSSISMOOTHING_16                  = 3,
      RSSISMOOTHING_32                  = 4,
      RSSISMOOTHING_64                  = 5,
      RSSISMOOTHING_128                 = 6,
      RSSISMOOTHING_256                 = 7
    } RSSISMOOTHING_T;

    /**
     * LOR_RegIrqFlagsMask and LOR_RegIrqFlags registers
     */
    typedef enum {
      LOR_IRQFLAG_CadDetected            = 0x01,
      LOR_IRQFLAG_FhssChangeChannel      = 0x02,
      LOR_IRQFLAG_CadDone                = 0x04,
      LOR_IRQFLAG_TxDone                 = 0x08,

      LOR_IRQFLAG_ValidHeader            = 0x10,
      LOR_IRQFLAG_PayloadCrcError        = 0x20,
      LOR_IRQFLAG_RxDone                 = 0x40,
      LOR_IRQFLAG_RxTimeout              = 0x80
    } LOR_IRQFLAG_BITS_T;
    
    /**
     * FSK_RxBw register and FSK_RegAfcBw registers
     */
    typedef enum {
      RXBW_RxBwExp0                      = 0x01,
      RXBW_RxBwExp1                      = 0x02,
      RXBW_RxBwExp2                      = 0x04,
      _RXBW_RxBwExp_MASK                 = 7,
      _RXBW_RxBwExp_SHIFT                = 0,

      RXBW_RxBwMant0                     = 0x08,
      RXBW_RxBwMant1                     = 0x10,
      _RXBW_RxBwMant_MASK                = 3,
      _RXBW_RxBwMant_SHIFT               = 3,
      // 0x20-0x80 reserved
    } RXBW_BITS_T;

    /**
     * RXBW_RxBwMant values
     */
    typedef enum {
      RXBWMANT_0                         = 0,
      RXBWMANT_1                         = 1,
      RXBWMANT_2                         = 2
      // 3 reserved
    } RXBWMANT_T;

    /**
     * RXBW_RxBwExp values
     */
    typedef enum {
      RXBWEXP_1                          = 1,
      RXBWEXP_2                          = 2,
      RXBWEXP_3                          = 3,
      RXBWEXP_4                          = 4,
      RXBWEXP_5                          = 5,
      RXBWEXP_6                          = 6,
      RXBWEXP_7                          = 7
      // other values reserved
    } RXBWEXP_T;

    /**
     * FSK_OokPeak register
     */
    typedef enum {
      OOKPEAK_OokPeakThreshStep0         = 0x01,
      OOKPEAK_OokPeakThreshStep1         = 0x02,
      OOKPEAK_OokPeakThreshStep2         = 0x04,
      _OOKPEAK_OokPeakThreshStep_MASK    = 7,
      _OOKPEAK_OokPeakThreshStep_SHIFT   = 0,

      OOKPEAK_OokThreshType0             = 0x08,
      OOKPEAK_OokThreshType1             = 0x10,
      _OOKPEAK_OokThreshType_MASK        = 3,
      _OOKPEAK_OokThreshType_SHIFT       = 3,

      OOKPEAK_BitSyncOn                  = 0x20,

      // 0x40-0x80 reserved
    } OOKPEAK_BITS_T;

    /**
     * OokPeakThreshStep values
     */
    typedef enum {
      OOKPEAKTHRESHSTEP_05dB             = 0, // dec of RSSI threshold 0.5dB
      OOKPEAKTHRESHSTEP_1dB              = 1, // 1 dB
      OOKPEAKTHRESHSTEP_15dB             = 2, // 1.5 dB
      OOKPEAKTHRESHSTEP_2dB              = 3, // 2 dB
      OOKPEAKTHRESHSTEP_3dB              = 4,
      OOKPEAKTHRESHSTEP_4dB              = 5,
      OOKPEAKTHRESHSTEP_5dB              = 6,
      OOKPEAKTHRESHSTEP_6dB              = 7
    } OOKPEAKTHRESHSTEP_T;

    /**
     * OokPeakThreshType values
     */
    typedef enum {
      OOKTHRESHTYPE_FIXED                = 0,
      OOKTHRESHTYPE_PEAK                 = 1,
      OOKTHRESHTYPE_AVERAGE              = 2
      // 3 reserved
    } OOKTHRESHTYPE_T;

    /**
     * FSK_OokAvg register
     */
    typedef enum {
      OOKAVG_OokAvgThreshFilt0           = 0x01,
      OOKAVG_OokAvgThreshFilt1           = 0x02,
      _OOKAVG_OokAvgThreshFilt_MASK      = 3,
      _OOKAVG_OokAvgThreshFilt_SHIFT     = 0,
    
      OOKAVG_OokAvgOffset0               = 0x04,
      OOKAVG_OokAvgOffset1               = 0x08,
      _OOKAVG_OokAvgOffset_MASK          = 3,
      _OOKAVG_OokAvgOffset_SHIFT         = 2,

      // 0x10 reserved

      OOKAVG_OokPeakThreshDec0           = 0x20,
      OOKAVG_OokPeakThreshDec1           = 0x40,
      OOKAVG_OokPeakThreshDec2           = 0x80,
      _OOKAVG_OokPeakThreshDec_MASK      = 7,
      _OOKAVG_OokPeakThreshDec_SHIFT     = 5
    } OOKAVG_BITS_T;

    /**
     * OokAvgThreshFilt values
     */
    typedef enum {
      OOKAVGTHRESHFILT_32                = 0, // filter coedd in avg mode
      OOKAVGTHRESHFILT_8                 = 1,
      OOKAVGTHRESHFILT_4                 = 2,
      OOKAVGTHRESHFILT_2                 = 3
    } OOKAVGTHRESHFILT_T;

    /**
     * OokAvgOffset values
     */
    typedef enum {
      OOKAVGOFFSET_0                     = 0, // 0.0dB
      OOKAVGOFFSET_2                     = 1,
      OOKAVGOFFSET_4                     = 2,
      OOKAVGOFFSET_6                     = 3
    } OOKAVGOFFSET_T;

    /**
     * OokPeakThreshDec values
     */
    typedef enum {
      OOKPEAKTHRESHDEC_1_1               = 0, // once per chip
      OOKPEAKTHRESHDEC_1_2               = 1, // once every 2 chips...
      OOKPEAKTHRESHDEC_1_4               = 2,
      OOKPEAKTHRESHDEC_1_8               = 3,
      OOKPEAKTHRESHDEC_2_1               = 4, // twice per chip
      OOKPEAKTHRESHDEC_4_1               = 5, // 4 times every chip...
      OOKPEAKTHRESHDEC_8_1               = 6,
      OOKPEAKTHRESHDEC_16_1              = 7
    } OOKPEAKTHRESHDEC_T;

    /**
     * LOR_ModemStat register
     */
    typedef enum {
      MODEMSTAT_SignalDetected           = 0x01,
      MODEMSTAT_SignalSynchronized       = 0x02,
      MODEMSTAT_RxOngoing                = 0x04,
      MODEMSTAT_HeaderInfoValid          = 0x08,
      MODEMSTAT_ModemClear               = 0x10,
      
      MODEMSTAT_RxCodingRate0            = 0x20,
      MODEMSTAT_RxCodingRate1            = 0x40,
      MODEMSTAT_RxCodingRate2            = 0x80,
      _MODEMSTAT_RxCodingRate_MASK       = 7,
      _MODEMSTAT_RxCodingRate_SHIFT      = 5
    } MODEMSTAT_BITS_T;

    /**
     * FSK_RegAfcFei register
     */
    typedef enum {
      AFCFEI_AfcAutoClearOn              = 0x01,
      AFCFEI_AfcClear                    = 0x02,

      // 0x04-0x08 reserved

      AFCFEI_AgcStart                    = 0x10

      // 0x20-0x80 reserved
    } AFCFEI_BITS_T;

    /**
     * LOR_HopChannel register
     */
    typedef enum {
      HOPCHANNEL_FhssPresentChannel0     = 0x01, // current freq hopping channel
      HOPCHANNEL_FhssPresentChannel1     = 0x02,
      HOPCHANNEL_FhssPresentChannel2     = 0x04,
      HOPCHANNEL_FhssPresentChannel3     = 0x08,
      HOPCHANNEL_FhssPresentChannel4     = 0x10,
      HOPCHANNEL_FhssPresentChannel5     = 0x20,
      _HOPCHANNEL_FhssPresentChannel_MASK = 63,
      _HOPCHANNEL_FhssPresentChannel_SHIFT = 0,

      HOPCHANNEL_CrcOnPayload            = 0x40,
      HOPCHANNEL_PllTimeout              = 0x80
    } HOPCHANNEL_BITS_T;

    /**
     * LOR_ModemConfig1 register
     */
    typedef enum {
      MODEMCONFIG1_ImplicitHeaderModeOn  = 0x01,

      MODEMCONFIG1_CodingRate0           = 0x02,
      MODEMCONFIG1_CodingRate1           = 0x04,
      MODEMCONFIG1_CodingRate2           = 0x08,
      _MODEMCONFIG1_CodingRate_MASK      = 7,
      _MODEMCONFIG1_CodingRate_SHIFT     = 0,

      MODEMCONFIG1_Bw0                   = 0x10,
      MODEMCONFIG1_Bw1                   = 0x20,
      MODEMCONFIG1_Bw2                   = 0x40,
      MODEMCONFIG1_Bw3                   = 0x80,
      _MODEMCONFIG1_Bw_MASK              = 15,
      _MODEMCONFIG1_Bw_SHIFT             = 4
    } MODEMCONFIG1_BITS_T;

    /**
     * CodingRate values
     */
    typedef enum {
      CODINGRATE_4_5                     = 1, // Error coding rate 4/5
      CODINGRATE_4_6                     = 2,
      CODINGRATE_4_7                     = 3,
      CODINGRATE_4_8                     = 4
    } CODINGRATE_T;

    /**
     * Bw values
     */
    typedef enum {
      BW_7_8                             = 0, // 7.8Khz
      BW_10_4                            = 1,
      BW_15_6                            = 2,
      BW_20_8                            = 3,
      BW_31_25                           = 4,
      BW_41_7                            = 5,
      BW_62_5                            = 6,
      BW_125                             = 7,
      BW_250                             = 8,
      BW_500                             = 9

      // BW250 and BW500 not supported in lower band (169Mhz)
    } BW_T;

    /**
     * LOR_ModemConfig2 register
     */
    typedef enum {
      MODEMCONFIG2_SymbTimeoutMsb0       = 0x01,
      MODEMCONFIG2_SymbTimeoutMsb1       = 0x02,
      _MODEMCONFIG2_SymbTimeoutMsb_MASK  = 3,
      _MODEMCONFIG2_SymbTimeoutMsb_SHIFT = 0,

      MODEMCONFIG2_RxPayloadCrcOn        = 0x04,

      MODEMCONFIG2_TxContinuousMode      = 0x08,

      MODEMCONFIG2_SpreadingFactor0      = 0x10,
      MODEMCONFIG2_SpreadingFactor1      = 0x20,
      MODEMCONFIG2_SpreadingFactor2      = 0x40,
      MODEMCONFIG2_SpreadingFactor3      = 0x80,
      _MODEMCONFIG2_SpreadingFactor_MASK = 15,
      _MODEMCONFIG2_SpreadingFactor_SHIFT = 4,
    } MODEMCONFIG2_BITS_T;

    /**
     * SpreadingFactor values (expressed as a base-2 logarithm)
     */
    typedef enum {
      SPREADINGFACTOR_64                 = 6, // 64 chips/symbol
      SPREADINGFACTOR_128                = 7,
      SPREADINGFACTOR_256                = 8,
      SPREADINGFACTOR_512                = 9,
      SPREADINGFACTOR_1024               = 10,
      SPREADINGFACTOR_2048               = 11,
      SPREADINGFACTOR_4096               = 12

      // other values reserved
    } SPREADINGFACTOR_T;

    /**
     * FSK_PreableDetect register
     */
    typedef enum {
      PREABLEDETECT_PreambleDetectorTol0 = 0x01,
      PREABLEDETECT_PreambleDetectorTol1 = 0x02,
      PREABLEDETECT_PreambleDetectorTol2 = 0x04,
      PREABLEDETECT_PreambleDetectorTol3 = 0x08,
      PREABLEDETECT_PreambleDetectorTol4 = 0x10,
      _PREABLEDETECT_PreambleDetectorTol4_MASK = 31,
      _PREABLEDETECT_PreambleDetectorTol4_SHIFT = 0,

      PREABLEDETECT_PreambleDetectorSize0 = 0x20,
      PREABLEDETECT_PreambleDetectorSize1 = 0x40,
      _PREABLEDETECT_PreambleDetectorSize_MASK = 3,
      _PREABLEDETECT_PreambleDetectorSize_SHIFT = 5,

      PREABLEDETECT_PreambleDetectorOn   = 0x80
    } PREAMBLEDETECT_BITS_T;

    /**
     * PreambleDetectorSize values
     */
    typedef enum {
      PREAMBLEDETECTORSIZE_1             = 0, // 1 byte
      PREAMBLEDETECTORSIZE_2             = 1,
      PREAMBLEDETECTORSIZE_3             = 2

      // other values reserved
    } PREAMBLEDETECTORSIZE_T;

    /**
     * FSK_Osc register
     */
    typedef enum {
      OSC_ClkOut0                        = 0x01, // clk output freq
      OSC_ClkOut1                        = 0x02,
      OSC_ClkOut2                        = 0x04,
      _OSC_ClkOut_MASK                   = 7,
      _OSC_ClkOut_SHIFT                  = 0,

      OSC_RcCalStart                     = 0x08

      // other bits reserved
    } OSC_BITS_T;

    /**
     * ClkOut values
     */
    typedef enum {
      CLKOUT_1                           = 0, // FXOSC
      CLKOUT_2                           = 1, // FXOSC / 2 ...
      CLKOUT_4                           = 2,
      CLKOUT_8                           = 3,
      CLKOUT_16                          = 4,
      CLKOUT_32                          = 5,
      CLKOUT_RC                          = 6, // RC, (automatically enabled)
      CLKOUT_OFF                         = 7  // clkout off
    } CLKOUT_T;

    /**
     * LOR_ModemConfig3 register
     */
    typedef enum {
      // 0x01-0x02 reserved

      MODEMCONFIG3_AgcAutoOn             = 0x04,
      MODEMCONFIG3_LowDataRateOptimize   = 0x08  // req. for SF11 and SF12 and
                                                 // BW125

      // 0x10-0x80 reserved
    } MODEMCONFIG3_BITS_T;

    /**
     * FSK_SyncConfig register
     */
    typedef enum {
      SYNCCONFIG_SyncSize0               = 0x01,
      SYNCCONFIG_SyncSize1               = 0x02,
      SYNCCONFIG_SyncSize2               = 0x04,
      _SYNCCONFIG_SyncSize_MASK          = 7,
      _SYNCCONFIG_SyncSize_SHIFT         = 0,

      // 0x08 reserved

      SYNCCONFIG_SyncOn                  = 0x10,
      SYNCCONFIG_PreamblePolarity        = 0x20,

      SYNCCONFIG_AutoRestartMode0        = 0x40,
      SYNCCONFIG_AutoRestartMode1        = 0x80,
      _SYNCCONFIG_AutoRestartMode_MASK   = 3,
      _SYNCCONFIG_AutoRestartMode_SHIFT  = 6,
    } SYNCCONFIG_BITS_T;

    /**
     * AutoRestartMode values
     */
    typedef enum {
      AUTORESTARTMODE_OFF                = 0,
      AUTORESTARTMODE_ON_NOPLL           = 1, // don't wait for PLL resync
      AUTORESTARTMODE_ON_PLL             = 2  // wait for PLL resync
      // other values reserved
    } AUTORESTARTMODE_T;

    /**
     * LOR_FeiMsb register (4 bit MSB of Fei value)
     */
    typedef enum {
      FEIMSB_FreqError0                  = 0x01,
      FEIMSB_FreqError1                  = 0x02,
      FEIMSB_FreqError2                  = 0x04,
      FEIMSB_FreqError3                  = 0x08,
      _FEIMSB_FreqError_MASK             = 15,
      _FEIMSB_FreqError_SHIFT            = 0

      // 0x10-0x80 reserved
    } FEIMSB_BITS_T;

    /**
     * FSK_PacketConfig1 register
     */
    typedef enum {
      PACKETCONFIG1_CrcWhiteningType     = 0x01,

      PACKETCONFIG1_AddressFiltering0    = 0x02,
      PACKETCONFIG1_AddressFiltering1    = 0x04,
      _PACKETCONFIG1_AddressFiltering_MASK = 3,
      _PACKETCONFIG1_AddressFiltering_SHIFT = 1,

      PACKETCONFIG1_CrcAutoClearOff      = 0x08,
      PACKETCONFIG1_CrcOn                = 0x10,

      PACKETCONFIG1_DcFree0              = 0x20,
      PACKETCONFIG1_DcFree1              = 0x40,
      _PACKETCONFIG1_DcFree_MASK         = 3,
      _PACKETCONFIG1_DcFree_SHIFT        = 5,

      PACKETCONFIG1_PacketFormat         = 0x80 // fixed(0) or variable(1)
    } PACKETCONFIG1_BITS_T;

    /**
     * AddressFiltering values
     */
    typedef enum {
      ADDRESSFILTERING_NONE              = 0,
      ADDRESSFILTERING_NODE              = 1, // must match node addr
      ADDRESSFILTERING_NODE_BROADCAST    = 2, // match node or broadcast
    } ADDRESSFILTERING_T;

    /**
     * DcFree values (DC-free encoding/decoding schemes)
     */
    typedef enum {
      DCFREE_NONE                        = 0,
      DCFREE_MANCHESTER                  = 1,
      DCFREE_WHITENING                   = 2
      // other values reserved
    } DCFREE_T;

    /**
     * FSK_PacketConfig2 register
     */
    typedef enum {
      PACKETCONFIG2_PayloadLengthMsb0    = 0x01,
      PACKETCONFIG2_PayloadLengthMsb1    = 0x02,
      PACKETCONFIG2_PayloadLengthMsb2    = 0x04,
      _PACKETCONFIG2_PayloadLengthMsb_MASK = 7,
      _PACKETCONFIG2_PayloadLengthMsb_SHIFT = 0,

      PACKETCONFIG2_BeaconOn             = 0x08,
 
      // 0x10 reserved (linked to io-homecontrol compat mode (?))

      PACKETCONFIG2_IoHomeOn             = 0x20,
      PACKETCONFIG2_DataMode             = 0x40, // continuous(0), packet(1)

      // 0x80 reserved
    } PACKETCONFIG2_BITS_T;

    /**
     * LOR_DetectOptimize register
     */
    typedef enum {
      DETECTOPTIMIZE_DetectionOptimize0  = 0x01,
      DETECTOPTIMIZE_DetectionOptimize1  = 0x02,
      DETECTOPTIMIZE_DetectionOptimize2  = 0x04,
      _DETECTOPTIMIZE_DetectionOptimize_MASK = 7,
      _DETECTOPTIMIZE_DetectionOptimize_SHIFT = 0

      // 0x08-0x80 reserved
    } DETECTOPTIMIZE_BITS_T;

    /**
     * DetectionOptimize values
     */
    typedef enum {
      DETECTIONOPTIMIZE_SF7_SF12         = 3,
      DETECTIONOPTIMIZE_SF6              = 5

      // other values reserved
    } DETECTIONOPTIMIZE_T;

    /**
     * LOR_InvertIQ register
     */
    typedef enum {
      INVERTIQ_InvertIQTxOff             = 0x01, // invert LoRa I & Q signals
                                                // UNDOCUMENTED

      // 0x01-0x20 reserved

      INVERTIQ_InvertIQRx                = 0x40 // invert LoRa I & Q signals

      // 0x80 reserved
    } INVERTIQ_BITS_T;

    /**
     * FSK_FifoThresh register
     */
    typedef enum {
      FIFOTHRESH_FifoThreshold0          = 0x01,
      FIFOTHRESH_FifoThreshold1          = 0x02,
      FIFOTHRESH_FifoThreshold2          = 0x04,
      FIFOTHRESH_FifoThreshold3          = 0x08,
      FIFOTHRESH_FifoThreshold4          = 0x10,
      FIFOTHRESH_FifoThreshold5          = 0x20,
      _FIFOTHRESH_FifoThreshold_MASK     = 63,
      _FIFOTHRESH_FifoThreshold_SHIFT    = 0,

      // 0x40 reserved

      FIFOTHRESH_TxStartCondition        = 0x80
    } FIFOTHRESH_BITS_T;

    /**
     * FSK_SeqConfig1 register
     */
    typedef enum {
      SEQCONFIG1_FromTransit             = 0x01,
      SEQCONFIG1_FromIdle                = 0x02,
      SEQCONFIG1_LowPowerSelection       = 0x04,

      SEQCONFIG1_FromStart0              = 0x08,
      SEQCONFIG1_FromStart1              = 0x10,
      _SEQCONFIG1_FromStart_MASK         = 3,
      _SEQCONFIG1_FromStart_SHIFT        = 3,

      SEQCONFIG1_IdleMode                = 0x20,
      SEQCONFIG1_SequencerStop           = 0x40,
      SEQCONFIG1_SequencerStart          = 0x80
    } SEQCONFIG1_BITS_T;

    /**
     * FromStart values
     */
    typedef enum {
      FROMSTART_ToLowPowerSelection        = 0,
      FROMSTART_ToReceiveState             = 1,
      FROMSTART_ToTransmitState            = 2,
      FROMSTART_ToTransmitStateOnFifoLevel = 3
    } FROMSTART_T;

    /**
     * FSK_SeqConfig2 register
     */
    typedef enum {
      SEQCONFIG2_FromPacketReceived0       = 0x01,
      SEQCONFIG2_FromPacketReceived1       = 0x02,
      SEQCONFIG2_FromPacketReceived2       = 0x04,
      _SEQCONFIG2_FromPacketReceived_MASK  = 7,
      _SEQCONFIG2_FromPacketReceived_SHIFT = 0,

      SEQCONFIG2_FromRxTimeout0            = 0x08,
      SEQCONFIG2_FromRxTimeout1            = 0x10,
      _SEQCONFIG2_FromRxTimeout_MASK       = 3,
      _SEQCONFIG2_FromRxTimeout_SHIFT      = 3,
      
      SEQCONFIG2_FromReceive0              = 0x20,
      SEQCONFIG2_FromReceive1              = 0x40,
      SEQCONFIG2_FromReceive2              = 0x80,
      _SEQCONFIG2_FromReceive_MASK         = 3,
      _SEQCONFIG2_FromReceive_SHIFT        = 5
    } SEQCONFIG2_BITS_T;
      
    /**
     * FromPacketReceived values
     */
    typedef enum {
      FROMPACKETRECEIVED_ToSequencerOff    = 0,
      FROMPACKETRECEIVED_ToTransmitStateOnFifoEmpty = 1,
      FROMPACKETRECEIVED_ToLowPowerSelection = 2,
      FROMPACKETRECEIVED_ToReceiveViaFS    = 3, // if freq was changed
      FROMPACKETRECEIVED_ToReceive         = 4  // if freq was not changed

      // other values reserved
    } FROMPACKETRECEIVED_T;

    /**
     * FromRxTimeout values
     */
    typedef enum {
      FROMRXTIMEOUT_ToReceiveViaReceiveStart = 0,
      FROMRXTIMEOUT_ToTransmitState      = 1,
      FROMRXTIMEOUT_ToLowPowerSelection  = 2,
      FROMRXTIMEOUT_ToSequencerOffState  = 3
    } FROMRXTIMEOUT_T;

    /**
     * FromReceive values
     */
    typedef enum {
      FROMRECEIVE_ToPcketReceived        = 1,
      FROMRECEIVE_ToLowPowerSelection    = 2,
      FROMRECEIVE_ToPacketReceived       = 3,
      FROMRECEIVE_ToSequencerOffOnRSSI   = 4, // RSSI interrupt
      FROMRECEIVE_ToSequencerOffOnSync   = 5, // SyncAddr interrupt
      FROMRECEIVE_ToSequencerOffOnPreambleDetect = 6, // PreambleDetect intr
      // other values reserved
    } FROMRECEIVE_T;

    /**
     * FSK_TimerResol register
     */
    typedef enum {
      TIMERRESOL_Timer2Resolution0       = 0x01,
      TIMERRESOL_Timer2Resolution1       = 0x02,
      _TIMERRESOL_Timer2Resolution_MASK  = 3,
      _TIMERRESOL_Timer2Resolution_SHIFT = 0,

      TIMERRESOL_Timer1Resolution0       = 0x04,
      TIMERRESOL_Timer1Resolution1       = 0x08,
      _TIMERRESOL_Timer1Resolution_MASK  = 3,
      _TIMERRESOL_Timer1Resolution_SHIFT = 2

      // 0x10-0x80 reserved
    } TIMERRESOL_BITS_T;

    /**
     * Timer1/Timer2Resolution values
     */
    typedef enum {
      TIMERRESOLUTION_DISABLED           = 0,
      TIMERRESOLUTION_64us               = 1, // 64us
      TIMERRESOLUTION_4_1ms              = 2, // 4.1ms
      TIMERRESOLUTION_262ms              = 3  // 262ms
    } TIMERRESOLUTION_T;

    /**
     * FSK_ImageCal register
     */
    typedef enum {
      IMAGECAL_TempMonitorOff            = 0x01,

      IMAGECAL_TempThreshold0            = 0x02,
      IMAGECAL_TempThreshold1            = 0x04,
      _IMAGECAL_TempThreshold_MASK       = 3,
      _IMAGECAL_TempThreshold_SHIFT      = 1,

      IMAGECAL_TenpChange                = 0x08,

      // 0x10 reserved

      IMAGECAL_ImageCalRunning           = 0x20,
      IMAGECAL_ImageCalStart             = 0x40,
      IMAGECAL_AutoImageCalOn            = 0x80
    } IMAGECAL_BITS_T;

    /**
     * TempThreshold values
     */
    typedef enum {
      TEMPTHRESHOLD_5C                   = 0, // temp change to trigger new I/Q
      TEMPTHRESHOLD_10C                  = 1, //  calibration
      TEMPTHRESHOLD_15C                  = 2,
      TEMPTHRESHOLD_20C                  = 3
    } TEMPTHRESHOLD_T;

    /**
     * FSK_LowBat register
     */
    typedef enum {
      LOWBAT_LowBatTrim0                 = 0x01,
      LOWBAT_LowBatTrim1                 = 0x02,
      LOWBAT_LowBatTrim2                 = 0x04,
      _LOWBAT_LowBatTrim_MASK            = 7,
      _LOWBAT_LowBatTrim_SHIFT           = 0,

      LOWBAT_LowBatOn                    = 0x08

      // 0x10-0z80 reserved
    } LOWBAT_BITS_T;

    /**
     * LowBatTrim values
     */
    typedef enum {
      LOWBATTRIM_1_695                   = 0, // 1.695v
      LOWBATTRIM_1_764                   = 1,
      LOWBATTRIM_1_835                   = 2,
      LOWBATTRIM_1_905                   = 3,
      LOWBATTRIM_1_976                   = 4,
      LOWBATTRIM_2_045                   = 5,
      LOWBATTRIM_2_116                   = 6,
      LOWBATTRIM_2_185                   = 7
    } LOWBATTRIM_T;

    /**
     * FSK_IrqFlags1 register
     */
    typedef enum {
      IRQFLAGS1_SyncAddressMatch         = 0x01,
      IRQFLAGS1_PreambleDetect           = 0x02,
      IRQFLAGS1_Timeout                  = 0x04,
      IRQFLAGS1_Rssi                     = 0x08,
      IRQFLAGS1_PllLock                  = 0x10,
      IRQFLAGS1_TxReady                  = 0x20,
      IRQFLAGS1_RxReady                  = 0x40,
      IRQFLAGS1_ModeReady                = 0x80
    } IRQFLAGS1_BITS_T;

    /**
     * FSK_IrqFlags2 register
     */
    typedef enum {
      IRQFLAGS2_LowBat                   = 0x01,
      IRQFLAGS2_CrcOk                    = 0x02,
      IRQFLAGS2_PayloadReady             = 0x04,
      IRQFLAGS2_PacketSent               = 0x08,
      IRQFLAGS2_FifoOverrun              = 0x10,
      IRQFLAGS2_FifoLevel                = 0x20,
      IRQFLAGS2_FifoEmpty                = 0x40,
      IRQFLAGS2_FifoFull                 = 0x80
    } IRQFLAGS2_BITS_T;

    /**
     * COM_DioMapping1 register. See Tables 18, 29, and 30 in the
     * datasheet for the different mappings depending on mode.
     */
    typedef enum {
      DOIMAPPING1_Dio3Mapping0           = 0x01,
      DOIMAPPING1_Dio3Mapping1           = 0x02,
      DOIMAPPING1_Dio3Mapping_MASK       = 3,
      DOIMAPPING1_Dio3Mapping_SHIFT      = 0,

      DOIMAPPING1_Dio2Mapping0           = 0x04,
      DOIMAPPING1_Dio2Mapping1           = 0x08,
      DOIMAPPING1_Dio2Mapping_MASK       = 3,
      DOIMAPPING1_Dio2Mapping_SHIFT      = 2,

      DOIMAPPING1_Dio1Mapping0           = 0x10,
      DOIMAPPING1_Dio1Mapping1           = 0x20,
      DOIMAPPING1_Dio1Mapping_MASK       = 3,
      DOIMAPPING1_Dio1Mapping_SHIFT      = 4,

      DOIMAPPING1_Dio0Mapping0           = 0x40,
      DOIMAPPING1_Dio0Mapping1           = 0x80,
      DOIMAPPING1_Dio0Mapping_MASK       = 3,
      DOIMAPPING1_Dio0Mapping_SHIFT      = 6,
    } DIOMAPPING1_BITS_T;


    /**
     * COM_DioMapping2 register. See Tables 18, 29, and 30 in the
     * datasheet for the different mappings depending on mode.
     */
    typedef enum {
      DOIMAPPING2_MapPreambleDetect      = 0x01, // rssi intr(0), preambledet(1)

      // 0x02-0x08 reserved

      DOIMAPPING2_Dio5Mapping0           = 0x10,
      DOIMAPPING2_Dio5Mapping1           = 0x20,
      DOIMAPPING2_Dio5Mapping_MASK       = 3,
      DOIMAPPING2_Dio5Mapping_SHIFT      = 4,

      DOIMAPPING2_Dio4Mapping0           = 0x40,
      DOIMAPPING2_Dio4Mapping1           = 0x80,
      DOIMAPPING2_Dio4Mapping_MASK       = 3,
      DOIMAPPING2_Dio4Mapping_SHIFT      = 6,
    } DIOMAPPING2_BITS_T;

    /**
     * DioXMapping values
     *
     * These differ depending on LoRa, FSK packet, and FSK continous
     * modes.  See Tables 29, 30 (FSK), and 18 (LoRa) in the datasheet
     * for details.
     */
    typedef enum {
      DIOMAPPING_00                      = 0, 
      DIOMAPPING_01                      = 1,
      DIOMAPPING_10                      = 2,
      DIOMAPPING_11                      = 3
    } DIOMAPPING_T;


    /**
     * LOR_PllHop (or FSK_PllHop depending on who you believe) register
     */
    typedef enum {
      // 0x01-0x40 reserved

      PLLHOP_FastHopOn                   = 0x80
    } PLLHOP_BITS_T;

    /**
     * COM_Tcxo register
     */
    typedef enum {
      // 0x01-0x08 reserved

      TCXO_TcxoOn                        = 0x10

      // 0x20-0x80 reserved
    } TCXO_BITS_T;

    /**
     * COM_PaDac register
     */
    typedef enum {
      PADAC_PaDac0                       = 0x01,
      PADAC_PaDac1                       = 0x02,
      PADAC_PaDac2                       = 0x04,
      _PADAC_PaDac_MASK                  = 7,
      _PADAC_PaDac_SHIFT                 = 0

      // 0x08-0x80 reserved
    } PADAC_BITS_T;

    /**
     * PaDac values
     */
    typedef enum {
      PADAC_DEFAULT                      = 4, 
      PADAC_BOOST                        = 7 // +20dBm on PA_BOOST when
                                            // OuputPower = 1111
      // other values reserved
    } PADAC_T;

    /**
     * FSK_BitRateFrac register
     */
    typedef enum {
      BITRATEFRAC_BitRateFrac0           = 0x01,
      BITRATEFRAC_BitRateFrac1           = 0x02,
      BITRATEFRAC_BitRateFrac2           = 0x04,
      BITRATEFRAC_BitRateFrac3           = 0x08,
      _BITRATEFRAC_BitRateFrac_MASK      = 15,
      _BITRATEFRAC_BitRateFrac_SHIFT     = 0

      // 0x10-0x80 reserved
    } BITRATEFRAC_BITS_T;

    /**
     * COM_AgcRef register
     *
     * These registers have 2 sets of values depending on whether
     * LowFrequencyModeOn is set or unset.
     */
    typedef enum {
      AGCREF_AgcReferenceLevel0          = 0x01,
      AGCREF_AgcReferenceLevel1          = 0x02,
      AGCREF_AgcReferenceLevel2          = 0x04,
      AGCREF_AgcReferenceLevel3          = 0x08,
      AGCREF_AgcReferenceLevel4          = 0x10,
      AGCREF_AgcReferenceLevel5          = 0x20,
      _AGCREF_AgcReferenceLevel_MASK     = 63,
      _AGCREF_AgcReferenceLevel_SHIFT    = 0

      // 0x40-0x80 reserved
    } ACFREF_BITS_T;

    /**
     * COM_AgcThresh1 register
     *
     * These registers have 2 sets of values depending on whether
     * LowFrequencyModeOn is set or unset.
     */
    typedef enum {
      AGCTHRESH1_AcgStep10               = 0x01,
      AGCTHRESH1_AcgStep11               = 0x02,
      AGCTHRESH1_AcgStep12               = 0x04,
      AGCTHRESH1_AcgStep13               = 0x08,
      _AGCTHRESH1_AcgStep1_MASK          = 15,
      _AGCTHRESH1_AcgStep1_SHIFT         = 0,

      // 0x10-0x80 reserved
    } ACGTHRESH1_BITS_T;

    /**
     * COM_AgcThresh2 register
     *
     * These registers have 2 sets of values depending on whether
     * LowFrequencyModeOn is set or unset.
     */
    typedef enum {
      AGCTHRESH2_AcgStep30               = 0x01,
      AGCTHRESH2_AcgStep31               = 0x02,
      AGCTHRESH2_AcgStep32               = 0x04,
      AGCTHRESH2_AcgStep33               = 0x08,
      _AGCTHRESH2_AcgStep3_MASK          = 15,
      _AGCTHRESH2_AcgStep3_SHIFT         = 0,

      AGCTHRESH2_AcgStep20               = 0x10,
      AGCTHRESH2_AcgStep21               = 0x20,
      AGCTHRESH2_AcgStep22               = 0x40,
      AGCTHRESH2_AcgStep23               = 0x80,
      _AGCTHRESH2_AcgStep2_MASK          = 15,
      _AGCTHRESH2_AcgStep2_SHIFT         = 4
    } ACGTHRESH2_BITS_T;

    /**
     * LOR_RegDetectionThreshold values
     */
    typedef enum {
      LOR_DetectionThreshold_SF7_SF12    = 0x0a,
      LOR_DetectionThreshold_SF6         = 0x0c
    } LOR_DETECTIONTHRESHOLD_T;

    /**
     * COM_AgcThresh3 register
     *
     * These registers have 2 sets of values depending on whether
     * LowFrequencyModeOn is set or unset.
     */
    typedef enum {
      AGCTHRESH3_AcgStep50               = 0x01,
      AGCTHRESH3_AcgStep51               = 0x02,
      AGCTHRESH3_AcgStep52               = 0x04,
      AGCTHRESH3_AcgStep53               = 0x08,
      _AGCTHRESH3_AcgStep5_MASK          = 15,
      _AGCTHRESH3_AcgStep5_SHIFT         = 0,

      AGCTHRESH3_AcgStep40               = 0x10,
      AGCTHRESH3_AcgStep41               = 0x20,
      AGCTHRESH3_AcgStep42               = 0x40,
      AGCTHRESH3_AcgStep43               = 0x80,
      _AGCTHRESH3_AcgStep4_MASK          = 15,
      _AGCTHRESH3_AcgStep4_SHIFT         = 4
    } ACGTHRESH3_BITS_T;


    /**
     * SX1276 constructor
     *
     * Since this is a shield, you will not have much choice as to
     * what pins are used.
     *
     * @param chipRev chip revision, default is 0x12
     * @param bus spi bus to use
     * @param cs GPIO pin to use as SPI Chip Select
     * @param reset GPIO pin to use as reset (A0=GPIO14)
     * @param dio0 GPIO pin to use as reset DIO0 intr
     * @param dio1 GPIO pin to use as reset DIO1 intr
     * @param dio2 GPIO pin to use as reset DIO2 intr
     * @param dio3 GPIO pin to use as reset DIO3 intr
     * @param dio4 GPIO pin to use as reset DIO4 intr
     * @param dio5 GPIO pin to use as reset DIO5 intr
     */
    SX1276(uint8_t chipRev=chipRevision, int bus=1, int cs=10, int resetPin=14,
           int dio0=2, int dio1=3, int dio2=4, int dio3=5, int dio4=17, 
           int dio5=9);

    /**
     * SX1276 Destructor
     */
    ~SX1276();

    /**
     * read a register
     *
     * @param reg the register to read
     * @return the value of the register
     */
    uint8_t readReg(uint8_t reg);

    /**
     * write to a register
     *
     * @param reg the register to write to
     * @param val the value to write
     * @return true if successful, false otherwise
     */
    bool writeReg(uint8_t reg, uint8_t val);

    /**
     * return the chip revision
     *
     * @return the chip revision (usually 0x12)
     */
    uint8_t getChipVersion();

    /**
     * reset the modem
     */
    void reset();

    /**
     * read the FIFO into a buffer
     *
     * @param buffer The buffer to read data into
     * @param len The length of the buffer
     */
    void readFifo(uint8_t *buffer, int len);

    /**
     * write a buffer into the FIFO
     *
     * @param buffer The buffer containing the data to write
     * @param len The length of the buffer
     */
    void writeFifo(uint8_t *buffer, int len);

    /**
     * Set the frequency to transmit and receive on
     *
     * @param freq The frequency to set
     */
    void setChannel(uint32_t freq);

    /**
     * Set the operating mode
     *
     * @param opMode One of the MODE_T values
     */
    void setOpMode(MODE_T opMode);

    /**
     * Set the modem to access.  This can be either the LORA or
     * KSK/OOK modem.
     *
     * @param modem One of the MODEM_T values
     */
    void setModem(RADIO_MODEM_T modem);

    /**
     * Place the SX1276 into sleep mode
     */
    void setSleep();

    /**
     * Place the SX1276 into standby mode
     */
    void setStandby();

    /**
     * Return the current Received Signal Strength Indicator for the
     * given modem
     *
     * @param modem One of the MODEM_T values
     */
    int16_t getRSSI(RADIO_MODEM_T modem);

    /**
     * Check to see if a given channel is free by comparing the RSSI
     * to the supplied threshold.
     *
     * @param modem One of the MODEM_T values
     * @param freq The channel to check
     * @param rssiThreshold The RSSI threshold, over which the channel
     * os considerd in use.
     */
    bool isChannelFree(RADIO_MODEM_T modem, uint32_t freq, int16_t rssiThresh);

    /**
     * Send the supplied string.  This writes the string into the FIFO
     * and places the modem in transmit mode (via setTx()).  This is a
     * wrapper around send().
     *
     * @param buffer The buffer to send
     * @param timeout The timeout in milliseconds
     * @return one of the RADIO_EVENT_T values
     */
    RADIO_EVENT_T sendStr(std::string buffer, int timeout);

    /**
     * Send the supplied buffer.  The writes the buffer into the FIFO
     * and places the modem in transmit mode (via setTx()).
     *
     * @param buffer The buffer to send
     * @param size The size of the buffer
     * @param timeout The timeout in milliseconds
     * @return one of the RADIO_EVENT_T values
     */
    RADIO_EVENT_T send(uint8_t *buffer, uint8_t size, int timeout);

    /**
     * Set the receive configuration for a modem.  It is important
     * that both the receive and transmit configurations match in order
     * for communication to work between two radios.
     *
     * @param modem One of the MODEM_T values

     * @param bandwidth The bandwidth to use.  Valid values are
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: [125 kHz, 250 kHz, 500 kHz] 
     * @param datarate Sets the Datarate
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     * @param coderate Sets the coding rate (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] 
     * @param bandwidthAfc Sets the AFC Bandwidth (FSK only) 
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: N/A ( set to 0 ) 
     * @param preambleLen Sets the Preamble length
     *                          FSK : Number of bytes 
     *                          LoRa: Length in symbols (the hardware adds 
     *                                4 more symbols)
     * @param symbTimeout Sets the RxSingle timeout value (LoRa only) 
     *                          FSK : N/A ( set to 0 ) 
     *                          LoRa: timeout in symbols
     * @param fixLen Fixed length packets [false: variable, true: fixed]
     * @param payloadLen Sets payload length when fixed length is used
     * @param crcOn Enables/Disables the CRC [false: OFF, true: ON]
     * @param FreqHopOn Enables disables the intra-packet frequency hopping
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [false: OFF, true: ON]
     * @param HopPeriod Number of symbols bewteen each hop
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: Number of symbols
     * @param  iqInverted Inverts IQ signals (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [false: not inverted, true: inverted]
     * @param  rxContinuous Sets the reception in continuous mode
     *                          [false: single mode, true: continuous mode]
     */
    void setRxConfig(RADIO_MODEM_T modem, uint32_t bandwidth,
                     uint32_t datarate, uint8_t coderate,
                     uint32_t bandwidthAfc, uint16_t preambleLen,
                     uint16_t symbTimeout, bool fixLen,
                     uint8_t payloadLen,
                     bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                     bool iqInverted, bool rxContinuous);

    /**
     * Set the transmit configuration for a modem.  It is important
     * that both the receive and transmit configurations match in order
     * for communication to work between two radios.
     *
     * @param modem One of the MODEM_T values
     * @param power Sets the output power [dBm]
     * @param fdev Sets the frequency deviation (FSK only)
     *                          FSK : [Hz]
     *                          LoRa: 0
     * @param bandwidth Sets the bandwidth (LoRa only)
     *                          FSK : 0
     *                          LoRa: [125 kHz, 250 kHz,
     *                                 or 500 kHz] 
     * @param datarate Sets the Datarate
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     * @param coderate Sets the coding rate (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] 
     * @param preambleLen Sets the preamble length
     *                          FSK : Number of bytes 
     *                          LoRa: Length in symbols (the hardware adds 
     *                                4 more symbols)
     * @param fixLen Fixed length packets [false: variable, true: fixed]
     * @param crcOn Enables disables the CRC [false: OFF, true: ON]
     * @param FreqHopOn Enables disables the intra-packet frequency hopping
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [false: OFF, true: ON]
     * @param HopPeriod Number of symbols bewteen each hop
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: Number of symbols
     * @param iqInverted Inverts IQ signals (LoRa only)
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [false: not inverted, true: inverted]
     */
    void setTxConfig(RADIO_MODEM_T modem, int8_t power, uint32_t fdev, 
                     uint32_t bandwidth, uint32_t datarate,
                     uint8_t coderate, uint16_t preambleLen,
                     bool fixLen, bool crcOn, bool freqHopOn, 
                     uint8_t hopPeriod, bool iqInverted);

    /**
     * Start a receive operation.  The method will return when
     * completed, either successfully, or in error (crc, or other
     * issue).  If completed successfully, the returned buffer can be
     * read via getRxBuffer() or getRxBufferStr().  In addition,
     * values for RSSI and SNR (Lora only) can be retrieved.
     *
     * @param timeout The timeout in milliseconds
     * @return one of the RADIO_EVENT_T values
     */
    RADIO_EVENT_T setRx(uint32_t timeout);

    /**
     * Upon a successful receive, this method can be used to retrieve
     * the received packet.
     *
     * @return The received buffer in a std::string
     */
    std::string getRxBufferStr()
    {
      std::string rBuffer((char *)m_rxBuffer, getRxLen());
      return rBuffer;
    };

    /**
     * Upon a successful receive, this method can be used to retrieve
     * the received packet.
     *
     * @return a pointer to the received buffer.  You can use
     * getRxLen() to determine the number of valid bytes present.
     */
    uint8_t *getRxBuffer()
    {
      return (uint8_t*)m_rxBuffer;
    };

    /**
     * Upon a successful receive, this method can be used to retrieve
     * the received packet's Received Signal Strength Indicator (RSSI)
     * value.
     *
     * @return RSSI value
     */
    int getRxRSSI()
    {
      return m_rxRSSI;
    };

    /**
     * Upon a successful receive, this method can be used to retrieve
     * the received packet's Signal to Noise (SNR) value.
     *
     * @return SNR value
     */
    int getRxSNR()
    {
      return m_rxSNR;
    };

    /**
     * Upon a successful receive, this method can be used to retrieve
     * the number of bytes received.
     *
     * @return the number of bytes received
     */
    int getRxLen()
    {
      return m_rxLen;
    };


  protected:
    // I/O
    mraa::Spi m_spi;
    mraa::Gpio m_gpioCS;
    mraa::Gpio m_gpioReset;

    mraa::Gpio m_gpioDIO0;
    mraa::Gpio m_gpioDIO1;
    mraa::Gpio m_gpioDIO2;
    mraa::Gpio m_gpioDIO3;
    mraa::Gpio m_gpioDIO4;
    mraa::Gpio m_gpioDIO5;

    // calibration called during init()
    void rxChainCalibration();

    // interrupt handlers
    static void onDio0Irq(void *ctx);
    static void onDio1Irq(void *ctx);
    static void onDio2Irq(void *ctx);
    static void onDio3Irq(void *ctx);
    static void onDio4Irq(void *ctx);
    static void onDio5Irq(void *ctx);

    /**
     * What internal state are we in
     */
    typedef enum {
      STATE_IDLE                         = 0,
      STATE_RX_RUNNING,
      STATE_TX_RUNNING,
      STATE_CAD
    } RADIO_STATES_T;

    // needs to be OR'd onto registers for SPI write
    static const uint8_t m_writeMode = 0x80;

    // initialize the chip
    void init();

    // Start a transmit event (you should use send() or sendStr()
    // rather than call this function directly.
    RADIO_EVENT_T setTx(int timeout);

    void startCAD(); // non-functional/non-tested

    // not really used, maybe it should be
    void setMaxPayloadLength(RADIO_MODEM_T modem, uint8_t max);

    // Chip Select control (active LOW)
    void csOn() 
    {
      m_gpioCS.write(0);
    };

    void csOff() 
    {
      m_gpioCS.write(1);
    };

  private:
    // Thse structs will generate SWIG warnings, as we do not expose
    // this data, they can be ignored.

    // stored settings for the FSK modem
    typedef struct
    {
      int8_t   Power;
      uint32_t Fdev;
      uint32_t Bandwidth;
      uint32_t BandwidthAfc;
      uint32_t Datarate;
      uint16_t PreambleLen;
      bool     FixLen;
      uint8_t  PayloadLen;
      bool     CrcOn;
      bool     IqInverted;
      bool     RxContinuous;
    } radioFskSettings_t;

    // stored settings for the LoRa modem
    typedef struct
    {
      int8_t   Power;
      uint32_t Bandwidth;
      uint32_t Datarate;
      bool     LowDatarateOptimize;
      uint8_t  Coderate;
      uint16_t PreambleLen;
      bool     FixLen;
      uint8_t  PayloadLen;
      bool     CrcOn;
      bool     FreqHopOn;
      uint8_t  HopPeriod;
      bool     IqInverted;
      bool     RxContinuous;
    } radioLoRaSettings_t;

    // FSK packet handler state
    typedef struct
    {
      uint8_t  PreambleDetected;
      uint8_t  SyncWordDetected;
      int8_t   RssiValue;
      int32_t  AfcValue;
      uint8_t  RxGain;
      uint16_t Size;
      uint16_t NbBytes;
      uint8_t  FifoThresh;
      uint8_t  ChunkSize;
    } radioFskPacketHandler_t;

    // LoRa packet handler state
    typedef struct
    {
      int8_t SnrValue;
      int16_t RssiValue;
      uint8_t Size;
    } radioLoRaPacketHandler_t;

    // our radio settings
    struct {
      RADIO_MODEM_T  modem;
      volatile RADIO_STATES_T state;
      uint32_t       channel;

      radioFskSettings_t      fskSettings;
      volatile radioFskPacketHandler_t fskPacketHandler;

      radioLoRaSettings_t      loraSettings;
      volatile radioLoRaPacketHandler_t loraPacketHandler;
    } m_settings;

    uint8_t lookupFSKBandWidth(uint32_t bw);

    // received data (on successfull completion)
    volatile int m_rxRSSI;
    volatile int m_rxSNR;
    volatile int m_rxLen;
    uint8_t m_rxBuffer[FIFO_SIZE];

    // for coordinating interrupt access
    pthread_mutex_t m_intrLock;

    void lockIntrs() { pthread_mutex_lock(&m_intrLock); };
    void unlockIntrs() { pthread_mutex_unlock(&m_intrLock); };

    // current radio event status
    volatile RADIO_EVENT_T m_radioEvent;

    // timer support
    struct timeval m_startTime;
    void initClock();
    uint32_t getMillis();
  };
}


