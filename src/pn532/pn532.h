/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This code is heavily based on the Adafruit-PN532 library at
 * https://github.com/adafruit/Adafruit-PN532, which is licensed under
 * the BSD license. See upm/src/pn532/license.txt
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

#include <string.h>
#include <string>
#include <mraa/common.hpp>
#include <mraa/i2c.hpp>

#include <mraa/gpio.hpp>

#define PN532_I2C_BUS 0
#define PN532_DEFAULT_I2C_ADDR (0x48 >> 1)

#define PN532_PREAMBLE                      (0x00)
#define PN532_STARTCODE1                    (0x00)
#define PN532_STARTCODE2                    (0xFF)
#define PN532_POSTAMBLE                     (0x00)

#define PN532_HOSTTOPN532                   (0xD4)
#define PN532_PN532TOHOST                   (0xD5)

namespace upm {
  
  /**
   * @brief PN532 NFC/RFID reader/writer
   * @defgroup pn532 libupm-pn532
   * @ingroup adafruit i2c rfid
   */

  /**
   * @library pn532
   * @sensor pn532
   * @comname PN532 NFC/RFID reader/writer
   * @type rfid
   * @man adafruit
   * @web http://www.adafruit.com/products/364
   * @con i2c
   *
   * @brief API for the PN532 based NFC/RFID reader/writer
   *
   * Identify a card and print out basic info
   * @snippet pn532.cxx Interesting
   * Add a URI to an already NDEF formatted ultralight or NTAG2XX tag
   * @snippet pn532-writeurl.cxx Interesting
   */
  class PN532 {
  public:

    /**
     * PN532 commands
     */
    typedef enum {
      CMD_DIAGNOSE              = 0x00,
      CMD_GETFIRMWAREVERSION    = 0x02,
      CMD_GETGENERALSTATUS      = 0x04,
      CMD_READREGISTER          = 0x06,
      CMD_WRITEREGISTER         = 0x08,
      CMD_READGPIO              = 0x0C,
      CMD_WRITEGPIO             = 0x0E,
      CMD_SETSERIALBAUDRATE     = 0x10,
      CMD_SETPARAMETERS         = 0x12,
      CMD_SAMCONFIGURATION      = 0x14,
      CMD_POWERDOWN             = 0x16,
      CMD_RFCONFIGURATION       = 0x32,
      CMD_RFREGULATIONTEST      = 0x58,
      CMD_INJUMPFORDEP          = 0x56,
      CMD_INJUMPFORPSL          = 0x46,
      CMD_INLISTPASSIVETARGET   = 0x4A,
      CMD_INATR                 = 0x50,
      CMD_INPSL                 = 0x4E,
      CMD_INDATAEXCHANGE        = 0x40,
      CMD_INCOMMUNICATETHRU     = 0x42,
      CMD_INDESELECT            = 0x44,
      CMD_INRELEASE             = 0x52,
      CMD_INSELECT              = 0x54,
      CMD_INAUTOPOLL            = 0x60,
      CMD_TGINITASTARGET        = 0x8C,
      CMD_TGSETGENERALBYTES     = 0x92,
      CMD_TGGETDATA             = 0x86,
      CMD_TGSETDATA             = 0x8E,
      CMD_TGSETMETADATA         = 0x94,
      CMD_TGGETINITIATORCOMMAND = 0x88,
      CMD_TGRESPONSETOINITIATOR = 0x90,
      CMD_TGGETTARGETSTATUS     = 0x8A
    } PN532_CMD_T;
    
    /**
     * Response bytes
     */
    typedef enum {
      RSP_INDATAEXCHANGE        = 0x41,
      RSP_INLISTPASSIVETARGET   = 0x4B
    } PN532_RSP_T;

    /**
     * MIFARE commands
     */
    typedef enum {
      MIFARE_CMD_AUTH_A                   = 0x60,
      MIFARE_CMD_AUTH_B                   = 0x61,
      MIFARE_CMD_READ                     = 0x30,
      MIFARE_CMD_WRITE                    = 0xA0,
      MIFARE_CMD_TRANSFER                 = 0xB0,
      MIFARE_CMD_DECREMENT                = 0xC0,
      MIFARE_CMD_INCREMENT                = 0xC1,
      MIFARE_CMD_STORE                    = 0xC2,
      MIFARE_ULTRALIGHT_CMD_WRITE         = 0xA2
    } MIFARE_CMD_T;

    /**
     * NDEF prefixes
     */
    typedef enum {
      NDEF_URIPREFIX_NONE                 = 0x00,
      NDEF_URIPREFIX_HTTP_WWWDOT          = 0x01,
      NDEF_URIPREFIX_HTTPS_WWWDOT         = 0x02,
      NDEF_URIPREFIX_HTTP                 = 0x03,
      NDEF_URIPREFIX_HTTPS                = 0x04,
      NDEF_URIPREFIX_TEL                  = 0x05,
      NDEF_URIPREFIX_MAILTO               = 0x06,
      NDEF_URIPREFIX_FTP_ANONAT           = 0x07,
      NDEF_URIPREFIX_FTP_FTPDOT           = 0x08,
      NDEF_URIPREFIX_FTPS                 = 0x09,
      NDEF_URIPREFIX_SFTP                 = 0x0A,
      NDEF_URIPREFIX_SMB                  = 0x0B,
      NDEF_URIPREFIX_NFS                  = 0x0C,
      NDEF_URIPREFIX_FTP                  = 0x0D,
      NDEF_URIPREFIX_DAV                  = 0x0E,
      NDEF_URIPREFIX_NEWS                 = 0x0F,
      NDEF_URIPREFIX_TELNET               = 0x10,
      NDEF_URIPREFIX_IMAP                 = 0x11,
      NDEF_URIPREFIX_RTSP                 = 0x12,
      NDEF_URIPREFIX_URN                  = 0x13,
      NDEF_URIPREFIX_POP                  = 0x14,
      NDEF_URIPREFIX_SIP                  = 0x15,
      NDEF_URIPREFIX_SIPS                 = 0x16,
      NDEF_URIPREFIX_TFTP                 = 0x17,
      NDEF_URIPREFIX_BTSPP                = 0x18,
      NDEF_URIPREFIX_BTL2CAP              = 0x19,
      NDEF_URIPREFIX_BTGOEP               = 0x1A,
      NDEF_URIPREFIX_TCPOBEX              = 0x1B,
      NDEF_URIPREFIX_IRDAOBEX             = 0x1C,
      NDEF_URIPREFIX_FILE                 = 0x1D,
      NDEF_URIPREFIX_URN_EPC_ID           = 0x1E,
      NDEF_URIPREFIX_URN_EPC_TAG          = 0x1F,
      NDEF_URIPREFIX_URN_EPC_PAT          = 0x20,
      NDEF_URIPREFIX_URN_EPC_RAW          = 0x21,
      NDEF_URIPREFIX_URN_EPC              = 0x22,
      NDEF_URIPREFIX_URN_NFC              = 0x23
    } NDEF_URI_T;

    /**
     * Card baud rates
     */
    typedef enum {
      BAUD_MIFARE_ISO14443A               = 0x00 // 106 Kbit/s
    } BAUD_T;


    /**
     * Tag types
     */
    typedef enum {
      TAG_TYPE_UNKNOWN                    = 0,
      TAG_TYPE_MIFARE_CLASSIC             = 1,
      TAG_TYPE_NFC2                       = 2 /* ultralight or NTAG2XX */
    } TAG_TYPE_T;

    /**
     * pn532 constructor
     *
     * @param irq pin to use for IRQ
     * @param reset reset pin
     * @param bus i2c bus to use
     * @param address the address for this device
     */
    PN532(int irq, int reset, int bus=PN532_I2C_BUS, 
          uint8_t address=PN532_DEFAULT_I2C_ADDR);

    /**
     * PN532 Destructor
     */
    ~PN532();
    
    /**
     * set up initial values and start operation
     *
     * @return true if successful
     */
    bool init();

    /**
     * Checks the firmware version of the PN5xx chip
     * 
     * @return the chip's firmware version and ID
    */
    uint32_t getFirmwareVersion();

    /**
     * sends a command and waits a specified period for the ACK
     * 
     * @param  cmd       Pointer to the command buffer
     * @param  cmdlen    the size of the command in bytes 
     * @param  timeout   timeout before giving up (in ms)
     * 
     * @return true if everything is OK, false if timeout occured
     * before an ACK was recieved
     */
    bool sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen, 
                             uint16_t timeout=1000);

    /**
     * configures the SAM (Secure Access Module)
     *
     * @return true if successfully configured
     */
    bool SAMConfig(void);

    /**
     * sets the MxRtyPassiveActivation byte of the RFConfiguration
     * register.  By default the pn532 will retry indefinitely.
     * 
     * @param  maxRetries 0xFF to wait forever, 0x00..0xFE to timeout
     * after maxRetries.  0x00 means try once, with no retries on failure.
     * 
     * @return true if everything executed properly, false for an error
     */
    bool setPassiveActivationRetries(uint8_t maxRetries);
 
    /**
     *  waits for an ISO14443A target to enter the field
     *
     * @param  cardbaudbate  baud rate of the card, one of the BAUD_T values
     * @param  uid Pointer to the array that will be populated with the
     * cards UID, up to 7 bytes
     * @param  uidLength Pointer to the variable that will hold the
     * length of the card's UID.
     * @param timeout the number of milliseconds to wait
     *     
     * @return true if everything executed properly, false for an error
     */
    bool readPassiveTargetID(BAUD_T cardbaudrate, uint8_t * uid, 
                             uint8_t * uidLength, uint16_t timeout);
    
    /**
     * exchanges an APDU (Application Protocol Data Unit) with the
     * currently inlisted peer
     *
     * @param  send            Pointer to data to send
     * @param  sendLength      Length of the data to send
     * @param  response        Pointer to response data
     * @param  responseLength  Pointer to the response data length
     *     
     * @return true if everything executed properly, false for an error
     */
    bool inDataExchange(uint8_t * send, uint8_t sendLength,
                        uint8_t * response, uint8_t * responseLength);

    /**
     * 'InLists' a passive target. PN532 acting as reader/initiator,
     * peer acting as card/responder.
     *     
     * @return true if everything executed properly, false for an error
     */
    bool inListPassiveTarget();

    /**
     *  Indicates whether the specified block number is the first block
     *  in the sector (block 0 relative to the current sector)
     *     
     * @return true if it's the first block, false otherwise
     */
    bool mifareclassic_IsFirstBlock (uint32_t uiBlock);

    /**
     * indicates whether the specified block number is the sector trailer
     *     
     * @return true if it's the trailer block, false otherwise
     */
    bool mifareclassic_IsTrailerBlock (uint32_t uiBlock);

    /**
     * tries to authenticate a block of memory on a MIFARE card using the
     * INDATAEXCHANGE command.  See section 7.3.8 of the PN532 User Manual
     * for more information on sending MIFARE and other commands.
     *
     * @param  uid           Pointer to a byte array containing the card UID
     * @param  uidLen        The length (in bytes) of the card's UID (Should
     * be 4 for MIFARE Classic)
     * @param  blockNumber   The block number to authenticate.  (0..63 for
     * 1KB cards, and 0..255 for 4KB cards).
     * @param  keyNumber     Which key type to use during authentication
     * (0 = MIFARE_CMD_AUTH_A, 1 = MIFARE_CMD_AUTH_B)
     * @param  keyData       Pointer to a byte array containing the 6 byte
     * key value
     *   
     * @return true if everything executed properly, false for an error
     */
    bool mifareclassic_AuthenticateBlock (uint8_t * uid, uint8_t uidLen,
                                          uint32_t blockNumber,
                                          uint8_t keyNumber,
                                          uint8_t * keyData);

    /**
     *  tries to read an entire 16-byte data block at the specified block
     *  address.
     *
     *  @param  blockNumber   The block number to read  (0..63 for
     *  1KB cards, and 0..255 for 4KB cards).
     *  @param  data          Pointer to the byte array that will hold the
     *  retrieved data (if any)
     *    
     *  @return true if everything executed properly, false for an error
     */
    bool mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t * data);

    /**
     *  tries to write an entire 16-byte data block at the specified block
     *  address.
     *
     *  @param  blockNumber   The block number to write.  (0..63 for
     *  1KB cards, and 0..255 for 4KB cards).
     *  @param  data          The byte array that contains the data to write.
     *    
     *  @returns true if everything executed properly, false for an error
     */
    bool mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t * data);

    /**
     *  formats a Mifare Classic card to store NDEF Records 
     *    
     *  @return true if everything executed properly, false for an error
     */
    bool mifareclassic_FormatNDEF (void);

    /**
     *  writes an NDEF URI Record to the specified sector (1..15)
     *    
     *  Note that this function assumes that the Mifare Classic card is
     *  already formatted to work as an "NFC Forum Tag" and uses a MAD1
     *  file system.  You can use the NXP TagWriter app on Android to
     *  properly format cards for this.
     *
     * @param  sectorNumber  The sector that the URI record should be written
     * to (can be 1..15 for a 1K card)
     * @param  uriIdentifier The uri identifier code (one of the NDEF_URI_T 
     * values
     * @param  url the uri text to write (max 38 characters).
     *    
     * @return true if everything executed properly, false for an error
     */
    bool mifareclassic_WriteNDEFURI (uint8_t sectorNumber, 
                                     NDEF_URI_T uriIdentifier,
                                     const char * url);

    /**
     * read an entire 4-byte page at the specified address
     *
     * @param  page        The page number (0..63 in most cases)
     * @param  buffer      Pointer to the byte array that will hold the
     * retrieved data (if any)
     *    
     * @return true if everything executed properly, false for an error
     */
    bool ntag2xx_ReadPage (uint8_t page, uint8_t * buffer);

    /**
     *  write an entire 4-byte page at the specified block address
     *
     * @param  page          The page number to write.  (0..63 for most cases)
     * @param  data          The byte array that contains the data to write.
     * Should be exactly 4 bytes long.
     *    
     * @return true if everything executed properly, false for an error
     */
    bool ntag2xx_WritePage (uint8_t page, uint8_t * data);

    /**
     * writes an NDEF URI Record starting at the specified page (4..nn)
     *      
     * Note that this function assumes that the NTAG2xx card is
     * already formatted to work as an "NFC Forum Tag".
     *
     * @param  uriIdentifier The uri identifier code (one of the NDEF_URI_T 
     * values
     * @param  url           The uri text to write (null-terminated string).
     * @param  dataLen       The size of the data area for overflow checks.
     *    
     * @return true if everything executed properly, false for an error
     */
    bool ntag2xx_WriteNDEFURI (NDEF_URI_T uriIdentifier, char * url, 
                               uint8_t dataLen);

    /**
     * return the ATQA (Answer to Request Acknowlege) value.  This
     * value is only valid after a successfull call to
     * readPassiveTargetID()
     * 
     * @return ATQA value
     */
    uint16_t getATQA() { return m_ATQA; };

    /**
     * return the SAK (Select Acknowlege) value.  This
     * value is only valid after a successfull call to
     * readPassiveTargetID()
     * 
     * @return SAK value
     */
    uint8_t getSAK() { return m_SAK; };

    /**
     * provide public access to the class's MRAA i2C context for
     * direct user access
     *
     * @return a reference to the class i2c context
     */
    mraa::I2c& i2cContext() { return m_i2c; };

    /**
     * enable or disable debugging output for pn532 related operations
     *
     * @param enable true to enabloe debug output, false to disable
     */
    void pn532Debug(bool enable) { m_pn532Debug = enable; };

    /**
     * enable or disable debugging output for mifare related operations
     *
     * @param enable true to enabloe debug output, false to disable
     */

    void mifareDebug(bool enable) { m_mifareDebug = enable; };

    /**
     * try to determine the tag type
     *
     * @return one of the TAG_TYPE_T values
     */
    TAG_TYPE_T tagType();

  protected:
    mraa::I2c m_i2c;
    mraa::Gpio m_gpioIRQ;
    mraa::Gpio m_gpioReset;

    bool readAck();
    bool isReady();
    bool waitForReady(uint16_t timeout);
    void readData(uint8_t* buff, uint8_t n);
    void writeCommand(uint8_t* cmd, uint8_t cmdlen);

  private:
    static void dataReadyISR(void *ctx);
    bool m_isrInstalled;
    volatile bool m_irqRcvd;

    uint8_t m_addr;

    uint8_t m_uid[7];       // ISO14443A uid
    uint8_t m_uidLen;       // uid len
    uint8_t m_key[6];       // Mifare Classic key
    uint8_t m_inListedTag;  // Tg number of inlisted tag.

    uint16_t m_ATQA;        // ATQA (Answer to Request Acknowlege - ISO14443)
                            // for currently inlisted card
    uint8_t m_SAK;          // SAK (Select Acknowlege) 
                            // for currently inlisted card

    // debugables
    bool m_pn532Debug;
    bool m_mifareDebug;
  };
}


