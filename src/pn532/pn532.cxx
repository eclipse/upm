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

#include <unistd.h>
#include <math.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "pn532.h"

using namespace upm;
using namespace std;


#define PN532_PACKBUFFSIZ 64
static uint8_t pn532_packetbuffer[PN532_PACKBUFFSIZ];

static uint8_t pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
static uint32_t pn532_firmwarerev = 0x00320106;

PN532::PN532(int irq, int reset, int bus, uint8_t address):
  m_gpioIRQ(irq), m_gpioReset(reset), m_i2c(bus)
{
  m_addr = address;
  m_uidLen = 0;
  m_inListedTag = 0;
  m_SAK = 0;
  m_ATQA = 0;
  m_isrInstalled = false;
  m_irqRcvd = false;

  memset(m_uid, 0, 7);
  memset(m_key, 0, 6);

  // turn off debugging by default
  pn532Debug(false);
  mifareDebug(false);

  mraa::Result rv;
  if ( (rv = m_i2c.address(m_addr)) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": I2c.address() failed");
      return;
    }

  m_gpioIRQ.dir(mraa::DIR_IN);
  m_gpioReset.dir(mraa::DIR_OUT);
}

PN532::~PN532()
{
  if (m_isrInstalled)
    m_gpioIRQ.isrExit();
}

bool PN532::init()
{
  m_gpioReset.write(1);
  m_gpioReset.write(0);
  usleep(400000);

  // install an interrupt handler
  m_gpioIRQ.isr(mraa::EDGE_FALLING, dataReadyISR, this);
  m_isrInstalled = true;

  m_gpioReset.write(1);

  return true;
}

/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
static void PrintHex(const uint8_t * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
    {
      fprintf(stderr, "0x%02x ", data[szPos] & 0xff);
    }
  fprintf(stderr, "\n");
}

/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters, along with
            the char equivalents in the following format

            00 00 00 00 00 00  ......

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
static void PrintHexChar(const uint8_t * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
    {
      fprintf(stderr, "0x%02x ", data[szPos] & 0xff);
    }
  fprintf(stderr, "  ");
  for (szPos=0; szPos < numBytes; szPos++) 
    {
      if (data[szPos] <= 0x1F)
        fprintf(stderr, ".");
      else
        fprintf(stderr, "%c ", (char)data[szPos]);
    }
  fprintf(stderr, "\n");
}


/**************************************************************************/
/*! 
  @brief  Checks the firmware version of the PN5xx chip

  @returns  The chip's firmware version and ID
*/
/**************************************************************************/
uint32_t PN532::getFirmwareVersion()
{
  uint32_t response = 0;

  pn532_packetbuffer[0] = CMD_GETFIRMWAREVERSION;
  
  if (! sendCommandCheckAck(pn532_packetbuffer, 1))
    return 0;
  
  // read data packet
  readData(pn532_packetbuffer, 12);
  
  int offset = 7;  // Skip the ready byte when using I2C

  response <<= 8;
  response |= pn532_packetbuffer[offset++];
  response <<= 8;
  response |= pn532_packetbuffer[offset++];
  response <<= 8;
  response |= pn532_packetbuffer[offset++];

  if (response != pn532_firmwarerev)
    fprintf(stderr, 
            "Warning: firmware revision 0x%08x does not match expected rev 0x%08x\n",
            response, pn532_firmwarerev);

  return response;
}


/**************************************************************************/
/*! 
  @brief  Sends a command and waits a specified period for the ACK

  @param  cmd       Pointer to the command buffer
  @param  cmdlen    The size of the command in bytes 
  @param  timeout   timeout before giving up
    
  @returns  1 if everything is OK, 0 if timeout occured before an
  ACK was recieved
*/
/**************************************************************************/
// default timeout of one second
bool PN532::sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen, 
                                uint16_t timeout)
{
  uint16_t timer = 0;

  // clear any outstanding irq's
  isReady();
  
  // write the command
  writeCommand(cmd, cmdlen);
  
  // Wait for chip to say its ready!
  if (!waitForReady(timeout)) {
    cerr << __FUNCTION__ << ": Not ready, timeout" << endl;
    return false;
  }

  if (m_pn532Debug)
    cerr << __FUNCTION__ << ": IRQ received" << endl;
  
  // read acknowledgement
  if (!readAck()) {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": No ACK frame received!" << endl;

    return false;
  }

  return true; // ack'd command
}

/**************************************************************************/
/*! 
  @brief  Configures the SAM (Secure Access Module)
*/
/**************************************************************************/
bool PN532::SAMConfig(void)
{
  pn532_packetbuffer[0] = CMD_SAMCONFIGURATION;
  pn532_packetbuffer[1] = 0x01; // normal mode;
  pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
  pn532_packetbuffer[3] = 0x01; // use IRQ pin!
  
  if (! sendCommandCheckAck(pn532_packetbuffer, 4))
    return false;

  // read data packet
  readData(pn532_packetbuffer, 8);
  
  int offset = 6;
  return  (pn532_packetbuffer[offset] == 0x15);
}

/**************************************************************************/
/*! 
  Sets the MxRtyPassiveActivation byte of the RFConfiguration register
    
  @param  maxRetries    0xFF to wait forever, 0x00..0xFE to timeout
  after mxRetries
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::setPassiveActivationRetries(uint8_t maxRetries)
{
  pn532_packetbuffer[0] = CMD_RFCONFIGURATION;
  pn532_packetbuffer[1] = 5;    // Config item 5 (MaxRetries)
  pn532_packetbuffer[2] = 0xFF; // MxRtyATR (default = 0xFF)
  pn532_packetbuffer[3] = 0x01; // MxRtyPSL (default = 0x01)
  pn532_packetbuffer[4] = maxRetries;

  if (m_mifareDebug)
    cerr << __FUNCTION__ << ": Setting MxRtyPassiveActivation to " 
         << (int)maxRetries << endl;
  
  if (! sendCommandCheckAck(pn532_packetbuffer, 5))
    return false;  // no ACK
  
  return true;
}

/***** ISO14443A Commands ******/

/**************************************************************************/
/*! 
  Waits for an ISO14443A target to enter the field
    
  @param  cardBaudRate  Baud rate of the card
  @param  uid           Pointer to the array that will be populated
  with the card's UID (up to 7 bytes)
  @param  uidLength     Pointer to the variable that will hold the
  length of the card's UID.
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::readPassiveTargetID(BAUD_T cardbaudrate, uint8_t * uid, 
                                uint8_t * uidLength, uint16_t timeout)
{
  pn532_packetbuffer[0] = CMD_INLISTPASSIVETARGET;
  pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this
                              // to 2 later)
  pn532_packetbuffer[2] = cardbaudrate;
  
  if (!sendCommandCheckAck(pn532_packetbuffer, 3, timeout))
    {
      if (m_pn532Debug)
        cerr << __FUNCTION__ << ": No card(s) read" << endl;

      return false;  // no cards read
    }

  // wait for a card to enter the field (only possible with I2C)
  if (m_pn532Debug)
    cerr << __FUNCTION__ << ": Waiting for IRQ (indicates card presence)" << endl;

  if (!waitForReady(timeout)) {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": IRQ Timeout" << endl;

    return false;
  }
  
  // read data packet
  readData(pn532_packetbuffer, 20);

  // check some basic stuff

  /* ISO14443A card response should be in the following format:
  
     byte            Description
     -------------   ------------------------------------------
     b0..6           Frame header and preamble
     b7              Tags Found
     b8              Tag Number (only one used in this example)
     b9..10          SENS_RES
     b11             SEL_RES
     b12             NFCID Length
     b13..NFCIDLen   NFCID                                      */

  // SENS_RES   SEL_RES     Manufacturer/Card Type    NFCID Len
  // --------   -------     -----------------------   ---------
  // 00 04      08          NXP Mifare Classic 1K     4 bytes   
  // 00 02      18          NXP Mifare Classic 4K     4 bytes
  
  if (m_mifareDebug)
    cerr << __FUNCTION__ << ": Found " <<  (int)pn532_packetbuffer[7] << " tags"
         << endl;

  // only one card can be handled currently
  if (pn532_packetbuffer[7] != 1) 
    return false;
    
  uint16_t sens_res = pn532_packetbuffer[9];
  sens_res <<= 8;
  sens_res |= pn532_packetbuffer[10];

  // store these for later retrieval, they can be used to more accurately
  // ID the type of card.

  m_ATQA = sens_res;
  m_SAK = pn532_packetbuffer[11]; // SEL_RES

  if (m_mifareDebug)
    {
      fprintf(stderr, "ATQA: 0x%04x\n", m_ATQA); 
      fprintf(stderr, "SAK: 0x%02x\n", m_SAK);
    }

  /* Card appears to be Mifare Classic */
  // JET: How so?

  *uidLength = pn532_packetbuffer[12];
  if (m_mifareDebug)
    fprintf(stderr, "UID: "); 

  for (uint8_t i=0; i < pn532_packetbuffer[12]; i++) 
    {
      uid[i] = pn532_packetbuffer[13+i];
      if (m_mifareDebug)
        fprintf(stderr, "0x%02x ", uid[i]); 
    }
  if (m_mifareDebug)
    fprintf(stderr, "\n");

  return true;
}

/**************************************************************************/
/*! 
  @brief  Exchanges an APDU with the currently inlisted peer

  @param  send            Pointer to data to send
  @param  sendLength      Length of the data to send
  @param  response        Pointer to response data
  @param  responseLength  Pointer to the response data length
*/
/**************************************************************************/
bool PN532::inDataExchange(uint8_t * send, uint8_t sendLength,
                           uint8_t * response, uint8_t * responseLength)
{
  if (sendLength > PN532_PACKBUFFSIZ-2) {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": APDU length too long for packet buffer"
           << endl;

    return false;
  }
  uint8_t i;
  
  pn532_packetbuffer[0] = CMD_INDATAEXCHANGE; // 0x40
  pn532_packetbuffer[1] = m_inListedTag;
  for (i=0; i<sendLength; ++i) {
    pn532_packetbuffer[i+2] = send[i];
  }
  
  if (!sendCommandCheckAck(pn532_packetbuffer,sendLength+2,1000)) {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": Could not send ADPU" << endl;

    return false;
  }

  if (!waitForReady(1000)) {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": Response never received for ADPU..." << endl;

    return false;
  }

  readData(pn532_packetbuffer, sizeof(pn532_packetbuffer));
  
  if (pn532_packetbuffer[0] == 0 && pn532_packetbuffer[1] == 0 &&
      pn532_packetbuffer[2] == 0xff)
    {
      
      uint8_t length = pn532_packetbuffer[3];
      if (pn532_packetbuffer[4]!=(uint8_t)(~length+1))
        {
          if (m_pn532Debug)
            fprintf(stderr, "Length check invalid: 0x%02x != 0x%02x\n", length,
                    (~length)+1);

          return false;
        }
      if (pn532_packetbuffer[5]==PN532_PN532TOHOST && 
          pn532_packetbuffer[6]==RSP_INDATAEXCHANGE)
        {
          if ((pn532_packetbuffer[7] & 0x3f)!=0)
            {
              if (m_pn532Debug)
                cerr << __FUNCTION__ << ": Status code indicates an error" 
                     << endl;

              return false;
            }
          
          length -= 3;
          
          if (length > *responseLength) {
            length = *responseLength; // silent truncation...
          }
          
          for (i=0; i<length; ++i) {
            response[i] = pn532_packetbuffer[8+i];
          }
          *responseLength = length;
          
          return true;
        } 
      else {
        fprintf(stderr, "Don't know how to handle this command: 0x%02x\n",
                pn532_packetbuffer[6]);
        return false;
      } 
    } 
  else {
    cerr << __FUNCTION__ << ": Preamble missing" << endl;
    return false;
  }
}

/**************************************************************************/
/*! 
  @brief  'InLists' a passive target. PN532 acting as reader/initiator,
  peer acting as card/responder.
*/
/**************************************************************************/
bool PN532::inListPassiveTarget() 
{
  m_inListedTag = 0;

  pn532_packetbuffer[0] = CMD_INLISTPASSIVETARGET;
  pn532_packetbuffer[1] = 1;
  pn532_packetbuffer[2] = 0;
  
  if (m_pn532Debug)
    cerr << __FUNCTION__ << ": About to inList passive target" << endl;

  if (!sendCommandCheckAck(pn532_packetbuffer,3,1000)) {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": Could not send inlist message" << endl;

    return false;
  }

  if (!waitForReady(30000)) {
    return false;
  }

  readData(pn532_packetbuffer, sizeof(pn532_packetbuffer));
  
  if (pn532_packetbuffer[0] == 0 && pn532_packetbuffer[1] == 0 && 
      pn532_packetbuffer[2] == 0xff) {

    uint8_t length = pn532_packetbuffer[3];
    if (pn532_packetbuffer[4]!=(uint8_t)(~length+1)) {
      if (m_pn532Debug)
        fprintf(stderr, "Length check invalid: 0x%02x != 0x%02x\n", length,
                (~length)+1);

      return false;
    }
    if (pn532_packetbuffer[5]==PN532_PN532TOHOST && 
        pn532_packetbuffer[6]==RSP_INLISTPASSIVETARGET) {
      if (pn532_packetbuffer[7] != 1) {
        cerr << __FUNCTION__ << ": Unhandled number of tags inlisted: "
             << (int)pn532_packetbuffer[7] << endl;
        return false;
      }
      
      m_inListedTag = pn532_packetbuffer[8];
      if (m_pn532Debug)
        cerr << __FUNCTION__ << ": Tag number: " << (int)m_inListedTag << endl;
      
      return true;
    } else {
      if (m_pn532Debug)
        cerr << __FUNCTION__ << ": Unexpected response to inlist passive host"
             << endl;

      return false;
    } 
  } 
  else {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": Preamble missing" << endl;

    return false;
  }

  return true;
}


/***** Mifare Classic Functions ******/
/*  MIFARE CLASSIC DESCRIPTION
    ==========================

    Taken from: https://www.kismetwireless.net/code-old/svn/hardware/kisbee-02/firmware/drivers/rf/pn532/helpers/pn532_mifare_classic.c

    MIFARE Classic cards come in 1K and 4K varieties.  While several
    varieties of chips exist, the two main chipsets used are described
    in the following publicly accessible documents:

        MF1S503x Mifare Classic 1K data sheet:
        http://www.nxp.com/documents/data_sheet/MF1S503x.pdf
            
        MF1S70yyX MIFARE Classic 4K data sheet:
        http://www.nxp.com/documents/data_sheet/MF1S70YYX.pdf

    Mifare Classic cards typically have a a 4-byte NUID, though you may
    find cards with 7 byte IDs as well
        
    EEPROM MEMORY
    =============
    Mifare Classic cards have either 1K or 4K of EEPROM memory. Each
    memory block can be configured with different access conditions,
    with two seperate authentication keys present in each block.

    The two main Mifare Classic card types are organised as follows:

        1K Cards: 16 sectors of 4 blocks (0..15)
        4K Cards: 32 sectors of 4 blocks (0..31) and
                  8 sectors of 16 blocks (32..39)

    4 block sectors
    ===============
    Sector  Block   Bytes                                                           Description
    ------  -----   -----                                                           -----------
                    0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15

    1       3       [-------KEY A-------]   [Access Bits]   [-------KEY A-------]   Sector Trailer 1
            2       [                            Data                           ]   Data
            1       [                            Data                           ]   Data
            0       [                            Data                           ]   Data

    0       3       [-------KEY A-------]   [Access Bits]   [-------KEY A-------]   Sector Trailer 1
            2       [                            Data                           ]   Data
            1       [                            Data                           ]   Data
            0       [                     Manufacturer Data                     ]   Manufacturer Block

    Sector Trailer (Block 3)
    ------------------------
    The sector trailer block contains the two secret keys (Key A and Key B), as well
    as the access conditions for the four blocks.  It has the following structure:

        Sector Trailer Bytes
        --------------------------------------------------------------
        0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
        [       Key A       ]   [Access Bits]   [       Key B       ]

    For more information in using Keys to access the clock contents, see
    Accessing Data Blocks further below.

    Data Blocks (Blocks 0..2)
    -------------------------
    Data blocks are 16 bytes wide and, depending on the permissions set in the
    access bits, can be read from and written to. You are free to use the 16 data
    bytes in any way you wish.  You can easily store text input, store four 32-bit
    integer values, a 16 character uri, etc.

    Data Blocks as "Value Blocks"
    -----------------------------
    An alternative to storing random data in the 16 byte-wide blocks is to
    configure them as "Value Blocks".  Value blocks allow performing electronic
    purse functions (valid commands are: read, write, increment, decrement,
    restore, transfer).
    
    Each Value block contains a single signed 32-bit value, and this value is
    stored 3 times for data integrity and security reasons.  It is stored twice
    non-inverted, and once inverted.  The last 4 bytes are used for a 1-byte
    address, which is stored 4 times (twice non-inverted, and twice inverted).
    
    Data blocks configured as "Value Blocks" have the following structure:

        Value Block Bytes
        --------------------------------------------------------------
        0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
        [   Value   ]   [   ~Value  ]   [   Value   ]   [A  ~A  A   ~A]

    Manufacturer Block (Sector 0, Block 0)
    --------------------------------------
    Sector 0 is special since it contains the Manufacturer Block. This block
    contains the manufacturer data, and is read-only.  It should be avoided
    unless you know what you are doing.

    16 block sectors
    ================
    16 block sectors are identical to 4 block sectors, but with more data blocks.  The same
    structure described in the 4 block sectors above applies.

    Sector  Block   Bytes                                                           Description
    ------  -----   -----                                                           ----------
                    0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15

    32      15      [-------KEY A-------]   [Access Bits]   [-------KEY B-------]   Sector Trailer 32
            14      [                            Data                           ]   Data
            13      [                            Data                           ]   Data
            ...
            2       [                            Data                           ]   Data
            1       [                            Data                           ]   Data
            0       [                            Data                           ]   Data

    ACCESSING DATA BLOCKS
    =====================

    Before you can access the cards, you must following two steps:

    1.) You must retrieve the 7 byte UID or the 4-byte NUID of the card.
        This can be done using pn532_mifareclassic_WaitForPassiveTarget()
        below, which will return the appropriate ID.

    2.) You must authenticate the sector you wish to access according to the
        access rules defined in the Sector Trailer block for that sector.
        This can be done using pn532_mifareclassic_AuthenticateBlock(),
        passing in the appropriate key value.
          
        Most new cards have a default Key A of 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF,
        but some common values worth trying are:

            0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
            0XD3 0XF7 0XD3 0XF7 0XD3 0XF7
            0XA0 0XA1 0XA2 0XA3 0XA4 0XA5
            0XB0 0XB1 0XB2 0XB3 0XB4 0XB5
            0X4D 0X3A 0X99 0XC3 0X51 0XDD
            0X1A 0X98 0X2C 0X7E 0X45 0X9A
            0XAA 0XBB 0XCC 0XDD 0XEE 0XFF
            0X00 0X00 0X00 0X00 0X00 0X00
            0XAB 0XCD 0XEF 0X12 0X34 0X56

    3.) Once authenication has succeeded, and depending on the sector
        permissions, you can then read/write/increment/decrement the
        contents of the specific block, using one of the helper functions
        included in this module.

*/
/**************************************************************************/
/*! 
  Indicates whether the specified block number is the first block
  in the sector (block 0 relative to the current sector)
*/
/**************************************************************************/
bool PN532::mifareclassic_IsFirstBlock (uint32_t uiBlock)
{
  // Test if we are in the small or big sectors
  if (uiBlock < 128)
    return ((uiBlock) % 4 == 0);
  else
    return ((uiBlock) % 16 == 0);
}

/**************************************************************************/
/*! 
  Indicates whether the specified block number is the sector trailer
*/
/**************************************************************************/
bool PN532::mifareclassic_IsTrailerBlock (uint32_t uiBlock)
{
  // Test if we are in the small or big sectors
  if (uiBlock < 128)
    return ((uiBlock + 1) % 4 == 0);
  else
    return ((uiBlock + 1) % 16 == 0);
}

/**************************************************************************/
/*! 
  Tries to authenticate a block of memory on a MIFARE card using the
  INDATAEXCHANGE command.  See section 7.3.8 of the PN532 User Manual
  for more information on sending MIFARE and other commands.

  @param  uid           Pointer to a byte array containing the card UID
  @param  uidLen        The length (in bytes) of the card's UID (Should
  be 4 for MIFARE Classic)
  @param  blockNumber   The block number to authenticate.  (0..63 for
  1KB cards, and 0..255 for 4KB cards).
  @param  keyNumber     Which key type to use during authentication
  (0 = MIFARE_CMD_AUTH_A, 1 = MIFARE_CMD_AUTH_B)
  @param  keyData       Pointer to a byte array containing the 6 byte
  key value
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::mifareclassic_AuthenticateBlock (uint8_t * uid, uint8_t uidLen,
                                             uint32_t blockNumber,
                                             uint8_t keyNumber,
                                             uint8_t * keyData)
{
  uint8_t len;
  uint8_t i;
  
  // Hang on to the key and uid data
  memcpy (m_key, keyData, 6);
  memcpy (m_uid, uid, uidLen);
  m_uidLen = uidLen;  
  
  if (m_mifareDebug)
    {
      fprintf(stderr, "Trying to authenticate card ");
      PrintHex(m_uid, m_uidLen);
      fprintf(stderr, "Using authentication KEY %c: ", 
              keyNumber ? 'B' : 'A');
      PrintHex(m_key, 6);
    }
  
  // Prepare the authentication command //
  pn532_packetbuffer[0] = CMD_INDATAEXCHANGE;   /* Data Exchange Header */
  pn532_packetbuffer[1] = 1;                              /* Max card numbers */
  pn532_packetbuffer[2] = (keyNumber) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
  pn532_packetbuffer[3] = blockNumber;                    /* Block
                                                             Number
                                                             (1K =
                                                             0..63, 4K
                                                             =
                                                             0..255 */
  memcpy (pn532_packetbuffer+4, m_key, 6);
  for (i = 0; i < m_uidLen; i++)
    {
      pn532_packetbuffer[10+i] = m_uid[i];                /* 4 byte card ID */
    }
  
  if (! sendCommandCheckAck(pn532_packetbuffer, 10+m_uidLen))
    return false;
  
  if (!waitForReady(1000)) {
    if (m_pn532Debug)
      cerr << __FUNCTION__ << ": timeout waiting auth..." << endl;

    return false;
  }

  // Read the response packet
  readData(pn532_packetbuffer, 12);
  
  // check if the response is valid and we are authenticated???
  // for an auth success it should be bytes 5-7: 0xD5 0x41 0x00
  // Mifare auth error is technically byte 7: 0x14 but anything other
  // and 0x00 is not good
  if (pn532_packetbuffer[7] != 0x00)
    {
      if (m_pn532Debug)
        {
          fprintf(stderr, "Authentication failed: ");
          PrintHexChar(pn532_packetbuffer, 12);
        }

      return false;
    }
  
  return true;
}

/**************************************************************************/
/*! 
  Tries to read an entire 16-byte data block at the specified block
  address.

  @param  blockNumber   The block number to authenticate.  (0..63 for
  1KB cards, and 0..255 for 4KB cards).
  @param  data          Pointer to the byte array that will hold the
  retrieved data (if any)
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t * data)
{
  if (m_mifareDebug)
    cerr << __FUNCTION__ << ": Trying to read 16 bytes from block " 
         << (int)blockNumber << endl;
  
  /* Prepare the command */
  pn532_packetbuffer[0] = CMD_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 1;                      /* Card number */
  pn532_packetbuffer[2] = MIFARE_CMD_READ;        /* Mifare Read
                                                     command = 0x30 */
  pn532_packetbuffer[3] = blockNumber;            /* Block Number
                                                     (0..63 for 1K,
                                                     0..255 for 4K) */
  
  /* Send the command */
  if (! sendCommandCheckAck(pn532_packetbuffer, 4))
    {
      if (m_mifareDebug)
        cerr << __FUNCTION__ << ": Failed to receive ACK for read command" 
             << endl;

      return false;
    }
  
  /* Read the response packet */
  readData(pn532_packetbuffer, 26);
  
  /* If byte 8 isn't 0x00 we probably have an error */
  if (pn532_packetbuffer[7] != 0x00)
    {
      if (m_mifareDebug)
        {
          fprintf(stderr, "Unexpected response: ");
          PrintHexChar(pn532_packetbuffer, 26);
        }
      return false;
    }
  
  /* Copy the 16 data bytes to the output buffer        */
  /* Block content starts at byte 9 of a valid response */
  memcpy (data, pn532_packetbuffer+8, 16);
  
  /* Display data for debug if requested */
  if (m_mifareDebug)
    {
      fprintf(stderr, "Block %d: ", blockNumber);
      PrintHexChar(data, 16);
    }
  
  return true;
}

/**************************************************************************/
/*! 
  Tries to write an entire 16-byte data block at the specified block
  address.

  @param  blockNumber   The block number to authenticate.  (0..63 for
  1KB cards, and 0..255 for 4KB cards).
  @param  data          The byte array that contains the data to write.
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t * data)
{
  if (m_mifareDebug)
    fprintf(stderr, "Trying to write 16 bytes to block %d\n", blockNumber);
  
  /* Prepare the first command */
  pn532_packetbuffer[0] = CMD_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 1;                      /* Card number */
  pn532_packetbuffer[2] = MIFARE_CMD_WRITE;       /* Mifare Write
                                                     command = 0xA0 */
  pn532_packetbuffer[3] = blockNumber;            /* Block Number
                                                     (0..63 for 1K,
                                                     0..255 for 4K) */
  memcpy (pn532_packetbuffer+4, data, 16);          /* Data Payload */
  
  /* Send the command */
  if (! sendCommandCheckAck(pn532_packetbuffer, 20))
    {
      if (m_mifareDebug)
        cerr << __FUNCTION__ << ": Failed to receive ACK for write command"
             << endl;

      return false;
    }  
  usleep(10000);
  
  /* Read the response packet */
  readData(pn532_packetbuffer, 26);
  
  return true;
}

/**************************************************************************/
/*! 
  Formats a Mifare Classic card to store NDEF Records 
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::mifareclassic_FormatNDEF (void)
{
  uint8_t sectorbuffer1[16] = {0x14, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 
                               0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};
  uint8_t sectorbuffer2[16] = {0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1,
                               0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};
  uint8_t sectorbuffer3[16] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x78, 0x77,
                               0x88, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  // Note 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 must be used for key A
  // for the MAD sector in NDEF records (sector 0)

  // Write block 1 and 2 to the card
  if (!(mifareclassic_WriteDataBlock (1, sectorbuffer1)))
    return false;
  if (!(mifareclassic_WriteDataBlock (2, sectorbuffer2)))
    return false;
  // Write key A and access rights card
  if (!(mifareclassic_WriteDataBlock (3, sectorbuffer3)))
    return false;

  // Seems that everything was OK (?!)
  return true;
}

/**************************************************************************/
/*! 
  Writes an NDEF URI Record to the specified sector (1..15)
    
  Note that this function assumes that the Mifare Classic card is
  already formatted to work as an "NFC Forum Tag" and uses a MAD1
  file system.  You can use the NXP TagWriter app on Android to
  properly format cards for this.

  @param  sectorNumber  The sector that the URI record should be written
  to (can be 1..15 for a 1K card)
  @param  uriIdentifier The uri identifier code (0 = none, 0x01 = 
  "http://www.", etc.)
  @param  url           The uri text to write (max 38 characters).
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::mifareclassic_WriteNDEFURI (uint8_t sectorNumber, 
                                        NDEF_URI_T uriIdentifier,
                                        const char * url)
{
  if (!url)
    return false;

  // Figure out how long the string is
  uint8_t len = strlen(url);
  
  // Make sure we're within a 1K limit for the sector number
  if ((sectorNumber < 1) || (sectorNumber > 15))
    return false;
  
  // Make sure the URI payload is between 1 and 38 chars
  if ((len < 1) || (len > 38))
    return false;

  // Note 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 must be used for key A
  // in NDEF records
    
  // Setup the sector buffer (w/pre-formatted TLV wrapper and NDEF message)
  uint8_t sectorbuffer1[16] = {0x00, 0x00, 0x03, len+5, 0xD1, 0x01, len+1, 
                               0x55, uriIdentifier, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00, 0x00};
  uint8_t sectorbuffer2[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00};
  uint8_t sectorbuffer3[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00};
  uint8_t sectorbuffer4[16] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 
                               0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 
                               0xFF, 0xFF};
  if (len <= 6)
    {
      // Unlikely we'll get a url this short, but why not ...
      memcpy (sectorbuffer1+9, url, len);
      sectorbuffer1[len+9] = 0xFE;
    }
  else if (len == 7)
    {
      // 0xFE needs to be wrapped around to next block
      memcpy (sectorbuffer1+9, url, len);
      sectorbuffer2[0] = 0xFE;
    }
  else if ((len > 7) && (len <= 22))
    {
      // Url fits in two blocks
      memcpy (sectorbuffer1+9, url, 7);
      memcpy (sectorbuffer2, url+7, len-7);
      sectorbuffer2[len-7] = 0xFE;
    }
  else if (len == 23)
    {
      // 0xFE needs to be wrapped around to final block
      memcpy (sectorbuffer1+9, url, 7);
      memcpy (sectorbuffer2, url+7, len-7);
      sectorbuffer3[0] = 0xFE;
    }
  else
    {
      // Url fits in three blocks
      memcpy (sectorbuffer1+9, url, 7);
      memcpy (sectorbuffer2, url+7, 16);
      memcpy (sectorbuffer3, url+23, len-24);
      sectorbuffer3[len-23] = 0xFE;
    }
  
  // Now write all three blocks back to the card
  if (!(mifareclassic_WriteDataBlock (sectorNumber*4, sectorbuffer1)))
    return false;
  if (!(mifareclassic_WriteDataBlock ((sectorNumber*4)+1, sectorbuffer2)))
    return false;
  if (!(mifareclassic_WriteDataBlock ((sectorNumber*4)+2, sectorbuffer3)))
    return false;
  if (!(mifareclassic_WriteDataBlock ((sectorNumber*4)+3, sectorbuffer4)))
    return false;

  // Seems that everything was OK (?!)
  return true;
}


/***** NTAG2xx/ultralight Functions ******/

// Ultralight tags are limited to 64 pages max, with ntag2XX tags can
// have up to 231 pages.

/*  MIFARE ULTRALIGHT DESCRIPTION
    =============================

    Taken from: https://www.kismetwireless.net/code-old/svn/hardware/kisbee-02/firmware/drivers/rf/pn532/helpers/pn532_mifare_ultralight.c

    MIFARE Ultralight cards typically contain 512 bits (64 bytes) of
    memory, including 4 bytes (32-bits) of OTP (One Time Programmable)
    memory where the individual bits can be written but not erased.

        MF0ICU1 Mifare Ultralight Functional Specification:
        http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf
            

    Mifare Ultralight cards have a 7-byte UID
    
    EEPROM MEMORY
    =============
    Mifare Ultralight cards have 512 bits (64 bytes) of EEPROM memory,
    including 4 byte (32 bits) of OTP memory.  Unlike Mifare Classic cards,
    there is no authentication on a per block level, although the blocks
    can be set to "read-only" mode using Lock Bytes (described below).

    EEPROM memory is organised into 16 pages of four bytes eachs, in
    the following order

    Page   Description
    ----   ------------
    0      Serial Number (4 bytes)
    1      Serial Number (4 bytes)
    2      Byte 0:    Serial Number
           Byte 1:    Internal Memory
           Byte 2..3: lock bytes
    3      One-time programmable memory (4 bytes)
    4..15  User memory (4 bytes)

    Lock Bytes (Page 2)
    -------------------
    Bytes 2 and 3 of page 2 are referred to as "Lock Bytes".  Each
    page from 0x03 and higher can individually locked by setting the
    corresponding locking bit to "1" to prevent further write access,
    effectively making the memory read only.

    For information on the lock byte mechanism, refer to section 8.5.2 of
    the datasheet (referenced above).

    OTP Bytes (Page 3)
    ------------------
    Page 3 is the OTP memory, and by default all bits on this page are
    set to 0.  These bits can be bitwise modified using the Mifare WRITE
    command, and individual bits can be set to 1, but can not be changed
    back to 0.

    Data Pages (Pages 4..15)
    ------------------------
    Pages 4 to 15 are can be freely read from and written to, 
    provided there is no conflict with the Lock Bytes described above.

    After production, the bytes have the following default values:

    Page    Byte Values
    ----    ----------------------
            0     1     2     3
    4       0xFF  0xFF  0xFF  0xFF
    5..15   0x00  0x00  0x00  0x00

    ACCESSING DATA BLOCKS
    =====================

    Before you can access the cards, you must following two steps:

    1.) 'Connect' to a Mifare Ultralight card and retrieve the 7 byte
        UID of the card.

    2.) Memory can be read and written directly once a passive mode
        connection has been made.  No authentication is required for
        Mifare Ultralight cards.

*/


/**************************************************************************/
/*! 
  Tries to read an entire 4-byte page at the specified address.

  @param  page        The page number (0..63 in most cases)
  @param  buffer      Pointer to the byte array that will hold the
  retrieved data (if any)
*/
/**************************************************************************/
bool PN532::ntag2xx_ReadPage (uint8_t page, uint8_t * buffer)
{
  // TAG Type       PAGES   USER START    USER STOP
  // --------       -----   ----------    ---------
  // NTAG 203       42      4             39
  // NTAG 213       45      4             39
  // NTAG 215       135     4             129
  // NTAG 216       231     4             225
  
  if (page >= 231)
    {
      cerr << __FUNCTION__ << ": Page value out of range" << endl;
      return false;
    }

  if (m_mifareDebug)
    fprintf(stderr, "Reading page %d\n", page);

  /* Prepare the command */
  pn532_packetbuffer[0] = CMD_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 1;                   /* Card number */
  pn532_packetbuffer[2] = MIFARE_CMD_READ;     /* Mifare Read command = 0x30 */
  pn532_packetbuffer[3] = page;                /* Page Number (0..63
                                                  in most cases) */

  /* Send the command */
  if (! sendCommandCheckAck(pn532_packetbuffer, 4))
    {
      if (m_mifareDebug)
        cerr << __FUNCTION__ << ": Failed to receive ACK for write command"
             << endl;

      return false;
    }
  
  /* Read the response packet */
  readData(pn532_packetbuffer, 26);

  if (m_mifareDebug)
    {
      fprintf(stderr, "Received: \n");
      PrintHexChar(pn532_packetbuffer, 26);
    }

  /* If byte 8 isn't 0x00 we probably have an error */
  if (pn532_packetbuffer[7] == 0x00)
    {
      /* Copy the 4 data bytes to the output buffer         */
      /* Block content starts at byte 9 of a valid response */
      /* Note that the command actually reads 16 byte or 4  */
      /* pages at a time ... we simply discard the last 12  */
      /* bytes                                              */
      memcpy (buffer, pn532_packetbuffer+8, 4);
    }
  else
    {
      if (m_mifareDebug)
        {
          fprintf(stderr, "Unexpected response reading block: \n");
          PrintHexChar(pn532_packetbuffer, 26);
        }

      return false;
    }

  /* Display data for debug if requested */
  if (m_mifareDebug)
    {
      fprintf(stderr, "Page %d:\n", page);
      PrintHexChar(buffer, 4);
    }

  // Return OK signal
  return true;
}

/**************************************************************************/
/*! 
  Tries to write an entire 4-byte page at the specified block
  address.

  @param  page          The page number to write.  (0..63 for most cases)
  @param  data          The byte array that contains the data to write.
  Should be exactly 4 bytes long.
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::ntag2xx_WritePage (uint8_t page, uint8_t * data)
{
  // TAG Type       PAGES   USER START    USER STOP
  // --------       -----   ----------    ---------
  // NTAG 203       42      4             39
  // NTAG 213       45      4             39
  // NTAG 215       135     4             129
  // NTAG 216       231     4             225

  if ((page < 4) || (page > 225))
    {
      cerr << __FUNCTION__ << ": Page value out of range" << endl;
      return false;
    }

  if (m_mifareDebug)
    fprintf(stderr, "Trying to write 4 byte page %d\n", page);
  
  /* Prepare the first command */
  pn532_packetbuffer[0] = CMD_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 1;    /* Card number */
  pn532_packetbuffer[2] = MIFARE_ULTRALIGHT_CMD_WRITE; /* Mifare
                                                          Ultralight
                                                          Write
                                                          command =
                                                          0xA2 */
  pn532_packetbuffer[3] = page; /* Page Number (0..63 for most cases) */
  memcpy (pn532_packetbuffer+4, data, 4); /* Data Payload */

  /* Send the command */
  if (! sendCommandCheckAck(pn532_packetbuffer, 8))
    {
      if (m_mifareDebug)
        cerr << __FUNCTION__ << ": Failed to receive ACK for write command"
             << endl;

      // Return Failed Signal
      return false;
    }  
  usleep(10000);
  
  /* Read the response packet */
  readData(pn532_packetbuffer, 26);
 
  // Return OK Signal
  return true;
}

/**************************************************************************/
/*! 
  Writes an NDEF URI Record starting at the specified page (4..nn)
    
  Note that this function assumes that the NTAG2xx card is
  already formatted to work as an "NFC Forum Tag".

  @param  uriIdentifier The uri identifier code (0 = none, 0x01 = 
  "http://www.", etc.)
  @param  url           The uri text to write (null-terminated string).
  @param  dataLen       The size of the data area for overflow checks.
    
  @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool PN532::ntag2xx_WriteNDEFURI (NDEF_URI_T uriIdentifier, char * url, 
                                  uint8_t dataLen)
{
  uint8_t pageBuffer[4] = { 0, 0, 0, 0 };
  
  // Remove NDEF record overhead from the URI data (pageHeader below)
  uint8_t wrapperSize = 12;
  
  // Figure out how long the string is
  uint8_t len = strlen(url);
  
  // Make sure the URI payload will fit in dataLen (include 0xFE trailer)
  if ((len < 1) || (len+1 > (dataLen-wrapperSize)))
    return false;
  
  // Setup the record header
  // See NFCForum-TS-Type-2-Tag_1.1.pdf for details
  uint8_t pageHeader[12] =
    {
      /* NDEF Lock Control TLV (must be first and always present) */
      0x01,         /* Tag Field (0x01 = Lock Control TLV) */
      0x03,         /* Payload Length (always 3) */
      0xA0,         /* The position inside the tag of the lock bytes
                       (upper 4 = page address, lower 4 = byte
                       offset) */
      0x10,         /* Size in bits of the lock area */
      0x44,         /* Size in bytes of a page and the number of bytes
                       each lock bit can lock (4 bit + 4 bits) */
      /* NDEF Message TLV - URI Record */
      0x03,         /* Tag Field (0x03 = NDEF Message) */
      len+5,        /* Payload Length (not including 0xFE trailer) */
      0xD1,         /* NDEF Record Header (TNF=0x1:Well known record +
                       SR + ME + MB) */
      0x01,         /* Type Length for the record type indicator */
      len+1,        /* Payload len */
      0x55,         /* Record Type Indicator (0x55 or 'U' = URI Record) */
      uriIdentifier /* URI Prefix (ex. 0x01 = "http://www.") */
    };
  
  // Write 12 byte header (three pages of data starting at page 4)
  memcpy (pageBuffer, pageHeader, 4);
  if (!(ntag2xx_WritePage (4, pageBuffer)))
    return false;
  memcpy (pageBuffer, pageHeader+4, 4);
  if (!(ntag2xx_WritePage (5, pageBuffer)))
    return false;
  memcpy (pageBuffer, pageHeader+8, 4);
  if (!(ntag2xx_WritePage (6, pageBuffer)))
    return false;
  
  // Write URI (starting at page 7)
  uint8_t currentPage = 7;
  char * urlcopy = url;
  while (len)
    {
      if (len < 4)
        {
          memset(pageBuffer, 0, 4);
          memcpy(pageBuffer, urlcopy, len);
          pageBuffer[len] = 0xFE; // NDEF record footer
          if (!(ntag2xx_WritePage (currentPage, pageBuffer)))
            return false;
          // DONE!
          return true;
        }
      else if (len == 4)
        {
          memcpy(pageBuffer, urlcopy, len);
          if (!(ntag2xx_WritePage (currentPage, pageBuffer)))
            return false;
          memset(pageBuffer, 0, 4);
          pageBuffer[0] = 0xFE; // NDEF record footer
          currentPage++;
          if (!(ntag2xx_WritePage (currentPage, pageBuffer)))
            return false;
          // DONE!
          return true;
        }
      else
        {
          // More than one page of data left
          memcpy(pageBuffer, urlcopy, 4);
          if (!(ntag2xx_WritePage (currentPage, pageBuffer)))
            return false;
          currentPage++;
          urlcopy+=4;
          len-=4;
        }
    }
  
  // Seems that everything was OK (?!)
  return true;
}


/**************************************************************************/
/*! 
  @brief  Tries to read/verify the ACK packet
*/
/**************************************************************************/
bool PN532::readAck()
{
  uint8_t ackbuff[6];
  
  readData(ackbuff, 6);

  return (0 == memcmp((char *)ackbuff, (char *)pn532ack, 6));
}


/**************************************************************************/
/*! 
  @brief  Return true if the PN532 is ready with a response.
*/
/**************************************************************************/
bool PN532::isReady()
{
  // ALWAYS clear the m_irqRcvd flag if set.
  if (m_irqRcvd)
    {
      m_irqRcvd = false;
      return true;
    }

  return false;
}

/**************************************************************************/
/*! 
  @brief  Waits until the PN532 is ready.

  @param  timeout   Timeout before giving up
*/
/**************************************************************************/
bool PN532::waitForReady(uint16_t timeout)
{
  uint16_t timer = 0;
  while(!isReady())
    {
      if (timeout != 0)
        {
          timer += 10;
          if (timer > timeout)
            {
              return false;
            }
        }
      usleep(10000);
    }
  return true;
}

/**************************************************************************/
/*! 
  @brief  Reads n bytes of data from the PN532 via SPI or I2C.

  @param  buff      Pointer to the buffer where data will be written
  @param  n         Number of bytes to be read
*/
/**************************************************************************/
void PN532::readData(uint8_t* buff, uint8_t n)
{
  uint8_t buf[n + 2];
  int rv;

  memset(buf, 0, n+2);
  usleep(2000); 
  if (m_i2c.address(m_addr) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_address() failed");
      return;
    }

  rv = m_i2c.read(buf, n + 2);

  if (m_pn532Debug)
    {
      cerr << __FUNCTION__ << ": read returned " << rv << "bytes" << endl;

      fprintf(stderr, "(raw) buf (%d) = ", rv);
      PrintHex(buf, rv);
      fprintf(stderr, "\n");
    }

  for (int i=0; i<n; i++)
    buff[i] = buf[i+1];
  
  if (m_pn532Debug)
    {
      fprintf(stderr, "(returned) buf (%d) = \n", n);
      PrintHex(buff, n);
      fprintf(stderr, "\n");
    }
}

/**************************************************************************/
/*! 
  @brief  Writes a command to the PN532, automatically inserting the
  preamble and required frame details (checksum, len, etc.)

  @param  cmd       Pointer to the command buffer
  @param  cmdlen    Command length in bytes 
*/
/**************************************************************************/
void PN532::writeCommand(uint8_t* cmd, uint8_t cmdlen)
{
  // I2C command write.

  cmdlen++;

  usleep(2000);     // 2ms max in case board needs to wake up

  // command + packet wrapper
  uint8_t buf[cmdlen + 8];
  memset(buf, 0, cmdlen + 8);
  int offset = 0;

  if (m_pn532Debug)
    cerr << __FUNCTION__ << ": Sending: " << endl;

  uint8_t checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
  buf[offset++] = PN532_PREAMBLE;
  buf[offset++] = PN532_PREAMBLE;
  buf[offset++] = PN532_STARTCODE2;

  buf[offset++] = cmdlen;
  buf[offset++] = ~cmdlen + 1;
   
  buf[offset++] = PN532_HOSTTOPN532;
  checksum += PN532_HOSTTOPN532;

  for (uint8_t i=0; i<cmdlen - 1; i++) 
    {
      buf[offset++] = cmd[i];
      checksum += cmd[i];
    }
  
  buf[offset++] = ~checksum;
  buf[offset] = PN532_POSTAMBLE;

  if (m_i2c.address(m_addr) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_address() failed");
      return;
    }

  if (m_i2c.write(buf, cmdlen + 8 - 1) != mraa::SUCCESS)
    {
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": mraa_i2c_write() failed");
      return;
    }

  if (m_pn532Debug)
    {
      cerr << __FUNCTION__ << ": cmdlen + 8 = " << cmdlen + 8 
           <<", offset = " << offset << endl;

      PrintHex(buf, cmdlen + 8);
    }
} 

void PN532::dataReadyISR(void *ctx)
{
  upm::PN532 *This = (upm::PN532 *)ctx;

  // if debugging is enabled, indicate when an interrupt occurred, and
  // a previously triggered interrupt was still set.
  if (This->m_pn532Debug)
    if (This->m_irqRcvd)
      cerr << __FUNCTION__ << ": INFO: Unhandled IRQ detected." << endl;

  This->m_irqRcvd = true;
}

PN532::TAG_TYPE_T PN532::tagType()
{
  // This is really just a guess, ATQA and SAK could in theory be used
  // to refine the detection.

  if (m_uidLen == 4)
    return TAG_TYPE_MIFARE_CLASSIC;
  else if (m_uidLen == 7)
    return TAG_TYPE_NFC2;
  else
    return TAG_TYPE_UNKNOWN;
}
