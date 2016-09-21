/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <string.h>

#ifdef JAVACALLBACK
#undef JAVACALLBACK
#endif

#include "sx1276.hpp"

using namespace upm;
using namespace std;

//
// FSK bandwidth to register definition structs
//
typedef struct
{
  uint32_t bandwidth;
  uint8_t  RegValue;
} FskBandwidth_t;

static const FskBandwidth_t FskBandwidths[] =
{       
  { 2600  , 0x17 },   
  { 3100  , 0x0F },
  { 3900  , 0x07 },
  { 5200  , 0x16 },
  { 6300  , 0x0E },
  { 7800  , 0x06 },
  { 10400 , 0x15 },
  { 12500 , 0x0D },
  { 15600 , 0x05 },
  { 20800 , 0x14 },
  { 25000 , 0x0C },
  { 31300 , 0x04 },
  { 41700 , 0x13 },
  { 50000 , 0x0B },
  { 62500 , 0x03 },
  { 83333 , 0x12 },
  { 100000, 0x0A },
  { 125000, 0x02 },
  { 166700, 0x11 },
  { 200000, 0x09 },
  { 250000, 0x01 },
  { 300000, 0x00 }, // Invalid Badwidth
};


SX1276::SX1276(uint8_t chipRev, int bus, int cs, int resetPin, int dio0, 
               int dio1, int dio2, int dio3, int dio4, int dio5) :
  m_spi(bus), m_gpioCS(cs), m_gpioReset(resetPin), m_gpioDIO0(dio0),
  m_gpioDIO1(dio1), m_gpioDIO2(dio2), m_gpioDIO3(dio3), m_gpioDIO4(dio4),
  m_gpioDIO5(dio5)
{
  m_spi.mode(mraa::SPI_MODE0);
  m_spi.frequency(10000000); // 10Mhz, if supported

  m_gpioCS.dir(mraa::DIR_OUT);
  m_gpioCS.useMmap(true);
  csOff();

  m_gpioReset.dir(mraa::DIR_IN);

  // 10ms for POR
  usleep(10000);

  // setup the interrupt handlers.  All 6 of them.
  m_gpioDIO0.dir(mraa::DIR_IN);
  if (m_gpioDIO0.isr(mraa::EDGE_RISING, onDio0Irq, this))
    throw std::runtime_error(string(__FUNCTION__) +
                             ": Gpio.isr(dio0) failed");

  m_gpioDIO1.dir(mraa::DIR_IN);
  if (m_gpioDIO1.isr(mraa::EDGE_RISING, onDio1Irq, this))
    throw std::runtime_error(string(__FUNCTION__) +
                             ": Gpio.isr(dio1) failed");

  m_gpioDIO2.dir(mraa::DIR_IN);
  if (m_gpioDIO2.isr(mraa::EDGE_RISING, onDio2Irq, this))
    throw std::runtime_error(string(__FUNCTION__) +
                             ": Gpio.isr(dio2) failed");

  m_gpioDIO3.dir(mraa::DIR_IN);
  if (m_gpioDIO3.isr(mraa::EDGE_RISING, onDio3Irq, this))
    throw std::runtime_error(string(__FUNCTION__) +
                             ": Gpio.isr(dio3) failed");

  m_gpioDIO4.dir(mraa::DIR_IN);
  if (m_gpioDIO4.isr(mraa::EDGE_RISING, onDio4Irq, this))
    throw std::runtime_error(string(__FUNCTION__) +
                             ": Gpio.isr(dio4) failed");

  // this one isn't as vital, so no need to fail if this one can't be
  // setup.
  m_gpioDIO5.dir(mraa::DIR_IN);
  if (m_gpioDIO5.isr(mraa::EDGE_RISING, onDio5Irq, this))
    cerr << __FUNCTION__ << ": Gpio.isr(dio5) failed" << endl;

  initClock();
  m_radioEvent = REVENT_DONE;
  m_settings.state = STATE_IDLE;
  memset(m_rxBuffer, 0, FIFO_SIZE);
  m_rxSNR = 0;
  m_rxRSSI = 0;

  // check the chip revision (to make sure we can read the regs properly)
  uint8_t cRev = getChipVersion();
  if (cRev != chipRev)
    {
      std::ostringstream str;
      std::ostringstream str2;

      str << hex << (int)cRev << dec;
      str2 << hex << (int)chipRev << dec;
      throw std::runtime_error(string(__FUNCTION__) +
                               ": Incorrect Chip Revision. Expected 0x" +
                               str2.str() + ", got 0x" + str.str());
    }

  pthread_mutexattr_t mutexAttrib;
  pthread_mutexattr_init(&mutexAttrib);
  //  pthread_mutexattr_settype(&mutexAttrib, PTHREAD_MUTEX_RECURSIVE);
  
  if (pthread_mutex_init(&m_intrLock, &mutexAttrib))
    {
      pthread_mutexattr_destroy(&mutexAttrib);
      throw std::runtime_error(std::string(__FUNCTION__) +
                               ": pthread_mutex_init(intrLock) failed");
    }

  pthread_mutexattr_destroy(&mutexAttrib);

  init();
}

SX1276::~SX1276()
{
  pthread_mutex_destroy(&m_intrLock);
}

uint8_t SX1276::readReg(uint8_t reg)
{
  uint8_t pkt[2] = {static_cast<uint8_t>(reg & 0x7f), 0};

  csOn();
  if (m_spi.transfer(pkt, pkt, 2))
   {
      csOff();
      throw std::runtime_error(string(__FUNCTION__) +
                               ": Spi.transfer() failed");
      return 0;
    }
  csOff();

  return pkt[1];
}

bool SX1276::writeReg(uint8_t reg, uint8_t val)
{
  uint8_t pkt[2] = {static_cast<uint8_t>(reg | m_writeMode), val};

  csOn();
  if (m_spi.transfer(pkt, NULL, 2))
    {
      csOff();
      throw std::runtime_error(string(__FUNCTION__) +
                               ": Spi.transfer() failed");
      return false;
    }
  csOff();

  return true;
}

void SX1276::readFifo(uint8_t *buffer, int len)
{
  // can't read more than 256 bytes
  if (len > FIFO_SIZE)
    {
      throw std::length_error(string(__FUNCTION__) +
                              ": cannot read more than 256 bytes from FIFO");
      return;
    }

  uint8_t pkt = 0;

  csOn();
  if (m_spi.transfer(&pkt, NULL, 1))
    {
      csOff();
      throw std::runtime_error(string(__FUNCTION__) +
                               ": Spi.transfer(0) failed");
      return;
    }

  if (m_spi.transfer(NULL, buffer, len))
    {
      csOff();
      throw std::runtime_error(string(__FUNCTION__) +
                               ": Spi.transfer(buf) failed");
      return;
    }
  csOff();
}

void SX1276::writeFifo(uint8_t *buffer, int len)
{
  // can't write more than 256 bytes
  if (len > FIFO_SIZE)
    {
      throw std::length_error(string(__FUNCTION__) +
                              ": cannot write more than 256 bytes to FIFO");
      return;
    }

  uint8_t pkt = (0 | m_writeMode);

  csOn();
  if (m_spi.transfer(&pkt, NULL, 1))
    {
      csOff();
      throw std::runtime_error(string(__FUNCTION__) +
                               ": Spi.transfer(0) failed");
      return;
    }

  if (m_spi.transfer(buffer, NULL, len))
    {
      csOff();
      throw std::runtime_error(string(__FUNCTION__) +
                               ": Spi.transfer(buf) failed");
      return;
    }
  csOff();
}


uint8_t SX1276::getChipVersion()
{
  return readReg(COM_RegVersion);
}


void SX1276::reset()
{
  m_gpioReset.dir(mraa::DIR_OUT);
  usleep(1000); // 1ms
  m_gpioReset.dir(mraa::DIR_IN);
  usleep(10000); // 10ms
}


void SX1276::init()
{
  typedef struct
  {
    RADIO_MODEM_T  Modem;
    uint8_t        Addr;
    uint8_t        Value;
  } radioRegisters_t;
  
  // some initial setup
  static const radioRegisters_t radioRegsInit[] = {
    { MODEM_FSK , COM_RegLna             , 0x23 },
    { MODEM_FSK , FSK_RegRxConfig        , 0x1E },
    { MODEM_FSK , FSK_RegRssiConfg       , 0xD2 },
    { MODEM_FSK , FSK_RegPreambleDetect  , 0xAA },
    { MODEM_FSK , FSK_RegOsc             , 0x07 },
    { MODEM_FSK , FSK_RegSyncConfig      , 0x12 },
    { MODEM_FSK , FSK_RegSyncValue1      , 0xC1 },
    { MODEM_FSK , FSK_RegSyncValue2      , 0x94 },
    { MODEM_FSK , FSK_RegSyncValue3      , 0xC1 },
    { MODEM_FSK , FSK_RegPacketConfig1   , 0xD8 },
    { MODEM_FSK , FSK_RegFifoThresh      , 0x8F },
    { MODEM_FSK , FSK_RegImageCal        , 0x02 },
    { MODEM_FSK , COM_RegDioMapping1     , 0x00 },
    { MODEM_FSK , COM_RegDioMapping2     , 0x30 },
    { MODEM_LORA, LOR_RegMaxPayloadLength, 0x40 }
  };

  reset();

  rxChainCalibration();

  setOpMode(MODE_Sleep);

  for (int i = 0; i < sizeof(radioRegsInit) / sizeof(radioRegisters_t); i++ )
    {
      setModem(radioRegsInit[i].Modem);
      writeReg(radioRegsInit[i].Addr, radioRegsInit[i].Value);
    }
  
  setModem(MODEM_FSK);
  m_settings.state = STATE_IDLE;
}

void SX1276::rxChainCalibration()
{
  uint8_t regPaConfigInitVal;
  uint32_t initialFreq;
  uint8_t reg;

  // this function should only be called in init() (after reset()), as
  // the device is configured for FSK mode, LF at that time.
  
  // Save context
  regPaConfigInitVal = readReg(COM_RegPaConfig);
  initialFreq = (uint32_t) ( ((double)
                               (((uint32_t)readReg(COM_RegFrfMsb) << 16) |
                                 ((uint32_t)readReg(COM_RegFrfMid) << 8) |
                                 ((uint32_t)readReg(COM_RegFrfLsb)) ) )
    * FXOSC_STEP);
  
  // Cut the PA just in case, RFO output, power = -1 dBm
  writeReg(COM_RegPaConfig, 0x00);
  
  // Launch Rx chain calibration for LF band
  reg = readReg(FSK_RegImageCal);
  writeReg(FSK_RegImageCal, reg | IMAGECAL_ImageCalStart); 

  // spin until complete
  while (readReg(FSK_RegImageCal) & IMAGECAL_ImageCalRunning)
    usleep(1);

  //  cerr << __FUNCTION__ << ": Imagecal LF complete" << endl;
  
  // Set a Frequency in HF band
  setChannel(868000000);
  
  // Launch Rx chain calibration for HF band 
  reg = readReg(FSK_RegImageCal);
  writeReg(FSK_RegImageCal, reg | IMAGECAL_ImageCalStart); 

  // spin until complete
  while (readReg(FSK_RegImageCal) & IMAGECAL_ImageCalRunning)
    usleep(1);
  
  //  cerr << __FUNCTION__ << ": Imagecal HF complete" << endl;
  
  // Restore context
  writeReg(COM_RegPaConfig, regPaConfigInitVal);
  setChannel(initialFreq);
}

void SX1276::setChannel(uint32_t freq)
{
  m_settings.channel = freq;

  freq = ( uint32_t )( ( double )freq / FXOSC_STEP );

  writeReg(COM_RegFrfMsb, ( uint8_t )( ( freq >> 16 ) & 0xff ) );
  writeReg(COM_RegFrfMid, ( uint8_t )( ( freq >> 8 ) & 0xff ) );
  writeReg(COM_RegFrfLsb, ( uint8_t )( freq & 0xff ) );
}

void SX1276::setOpMode(MODE_T opMode)
{
  static uint8_t opModePrev = MODE_Standby;
  
  if(opMode != opModePrev)
    {
      opModePrev = opMode;

      uint8_t reg = readReg(COM_RegOpMode) & 
        ~(_OPMODE_Mode_MASK << _OPMODE_Mode_SHIFT);
      
      writeReg(COM_RegOpMode, (reg | (opMode << _OPMODE_Mode_SHIFT)) );
    }
}

void SX1276::setModem(RADIO_MODEM_T modem)
{
  if (m_settings.modem == modem )
    {
      return;
    }
  
  m_settings.modem = modem;

  uint8_t reg = 0;

  switch (m_settings.modem)
    {
    default:
    case MODEM_FSK:
      setOpMode(MODE_Sleep);

      // turn off lora
      reg = (readReg(COM_RegOpMode) & ~OPMODE_LongRangeMode);
      writeReg(COM_RegOpMode, reg);
      
      writeReg(COM_RegDioMapping1, 0x00);
      writeReg(COM_RegDioMapping2, 0x30); // DIO5=ModeReady

      break;

    case MODEM_LORA:
      setOpMode(MODE_Sleep);
      // turn lora on
      reg = (readReg(COM_RegOpMode) | OPMODE_LongRangeMode);
      writeReg(COM_RegOpMode, reg);
      
      writeReg(COM_RegDioMapping1, 0x00);
      writeReg(COM_RegDioMapping2, 0x00);

      break;
    }
}

bool SX1276::isChannelFree(RADIO_MODEM_T modem, uint32_t freq, 
                           int16_t rssiThresh)
{
  int16_t rssi = 0;
  
  setModem(modem);
  
  setChannel(freq);
  
  setOpMode(MODE_FSK_RxMode);
  
  usleep(1000);
  
  rssi = getRSSI(modem);
  
  setSleep();
  
  if (rssi > rssiThresh)
    {
      return false;
    }

  return true;
}

int16_t SX1276::getRSSI(RADIO_MODEM_T modem)
{
  int16_t rssi = 0;
  
  switch (modem)
    {
    case MODEM_FSK:
      // devide by 2
      rssi = -(readReg(FSK_RegRssiValue) >> 1);

      break;

    case MODEM_LORA:
      {
        uint8_t reg = readReg(LOR_RegRssiValue);
        if (m_settings.channel > RF_MID_BAND_THRESH )
          {
            rssi = LOR_RSSI_OFFSET_HF + reg;
          }
        else
          {
            rssi = LOR_RSSI_OFFSET_LF + reg;
          }
      }
      break;

    default:
      rssi = -1;

      break;
    }

  return rssi;
}

void SX1276::setSleep()
{
  setOpMode(MODE_Sleep);
  m_settings.state = STATE_IDLE;
}

void SX1276::setStandby()
{
  setOpMode(MODE_Standby);
  m_settings.state = STATE_IDLE;
}

uint8_t SX1276::lookupFSKBandWidth(uint32_t bw)
{
  // This function looks up values in the fsk_bw_lookup_table and
  // returns the approprite register value to use for either the
  // FSK_RxBw or the FSK_RegAfcBw registers

  // See Table 40 in the datasheet

  for (int i=0; i<(sizeof(FskBandwidths)/sizeof(FskBandwidth_t)) - 1; i++)
    {
      if ( (bw >= FskBandwidths[i].bandwidth) && 
           (bw < FskBandwidths[i + 1].bandwidth) )
        {
          return FskBandwidths[i].RegValue;
        }
    }
  
  // shouldn't happen, but the universe is vast and indifferent...
  throw std::range_error(std::string(__FUNCTION__) +
                         ": Unable to find bandwidth in lookup table. "
                         "Bandwidth must be between 2600 and 250000 for FSK");

  return 0;
}

SX1276::RADIO_EVENT_T SX1276::sendStr(string buffer, int timeout)
{
  if (buffer.size() > (FIFO_SIZE - 1))
    throw std::range_error(string(__FUNCTION__) +
                           ": buffer size must be less than 256");

  // for LORA/FSK modem, there seems to be a 64 byte requirement,
  // (LOR_RegRxNbBytes on the receiver) never seems to be anything
  // other than 64. Same seems to go for the FSK modem.  So, if the
  // packet is less than 64, pad it out to 64 bytes.  This requires
  // investigation.
  while (buffer.size() < 64)
    buffer.push_back(0);

  return send((uint8_t *)buffer.c_str(), buffer.size(), timeout);
}

SX1276::RADIO_EVENT_T SX1276::send(uint8_t *buffer, uint8_t size, 
                                   int txTimeout)
{
  switch (m_settings.modem)
    {
    case MODEM_FSK:
      {
        m_settings.fskPacketHandler.NbBytes = 0;
        m_settings.fskPacketHandler.Size = size;

        if (m_settings.fskSettings.FixLen == false)
          {
            writeFifo((uint8_t *)&size, 1);
          }
        else
          {
            writeReg(FSK_RegPayloadLength, size );
          }

        if ( (size > 0) && (size <= 64) )
          {
            m_settings.fskPacketHandler.ChunkSize = size;
          }
        else
          {
            m_settings.fskPacketHandler.ChunkSize = 32;
          }

        // Write payload buffer
        writeFifo(buffer, m_settings.fskPacketHandler.ChunkSize);
        m_settings.fskPacketHandler.NbBytes += 
          m_settings.fskPacketHandler.ChunkSize;
      }

      break;

    case MODEM_LORA:
      {
        if (m_settings.loraSettings.IqInverted == true)
          {
            uint8_t reg = readReg(LOR_RegInvertIQ);

            reg &= ~(INVERTIQ_InvertIQTxOff | INVERTIQ_InvertIQRx);
            writeReg(LOR_RegInvertIQ, reg);

            // warning, hardcoded undocumented magic number into
            // undocumented register
            writeReg(LOR_RegInvertIQ2, 0x19);
          }
        else
          {
            uint8_t reg = readReg(LOR_RegInvertIQ);
            reg &= ~(INVERTIQ_InvertIQTxOff | INVERTIQ_InvertIQRx);
            reg |= INVERTIQ_InvertIQTxOff; // 'active' off.
            writeReg(LOR_RegInvertIQ, reg);

            // warning, hardcoded undocumented magic number into
            // undocumented register
            writeReg(LOR_RegInvertIQ2, 0x1d);
          }

        m_settings.loraPacketHandler.Size = size;
        //        cerr << "PAYLOAD SIZE " << (int)size << endl;

        // Initializes the payload size
        writeReg(LOR_RegPayloadLength, size);

        // Full buffer used for Tx
        writeReg(LOR_RegFifoTxBaseAddr, 0);
        writeReg(LOR_RegFifoAddrPtr, 0 );

        // FIFO operations can not take place in Sleep mode
        if ((readReg(COM_RegOpMode) & _OPMODE_Mode_MASK) == MODE_Sleep)
          {
            setStandby();
            usleep(1000); // 1ms
          }

        // Write payload buffer
        writeFifo(buffer, size);
      }

      break;
    }

  return setTx(txTimeout);
}


void SX1276::setRxConfig(RADIO_MODEM_T modem, uint32_t bandwidth,
                         uint32_t datarate, uint8_t coderate,
                         uint32_t bandwidthAfc, uint16_t preambleLen,
                         uint16_t symbTimeout, bool fixLen,
                         uint8_t payloadLen,
                         bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                         bool iqInverted, bool rxContinuous)
{
  setModem( modem );
  
  uint8_t reg;
  
  switch (modem)
    {
    case MODEM_FSK:
      {
        m_settings.fskSettings.Bandwidth = bandwidth;
        m_settings.fskSettings.Datarate = datarate;
        m_settings.fskSettings.BandwidthAfc = bandwidthAfc;
        m_settings.fskSettings.FixLen = fixLen;
        m_settings.fskSettings.PayloadLen = payloadLen;
        m_settings.fskSettings.CrcOn = crcOn;
        m_settings.fskSettings.IqInverted = iqInverted;
        m_settings.fskSettings.RxContinuous = rxContinuous;
        m_settings.fskSettings.PreambleLen = preambleLen;
            
        datarate = (uint16_t)(FXOSC_FREQ / (double)datarate);
        writeReg(FSK_RegBitrateMsb, (uint8_t)(datarate >> 8));
        writeReg(FSK_RegBitrateLsb, (uint8_t)(datarate & 0xff));
        
        writeReg(FSK_RegRxBw, lookupFSKBandWidth(bandwidth));
        writeReg(FSK_RegAfcBw, lookupFSKBandWidth(bandwidthAfc));
        
        writeReg(FSK_RegPreambleMsb, 
                 (uint8_t)((preambleLen >> 8) & 0xff));
        writeReg(FSK_RegPreambleLsb, (uint8_t)(preambleLen & 0xff));
            
        if (fixLen)
          {
            writeReg(FSK_RegPayloadLength, payloadLen);
          }
            
        reg = readReg(FSK_RegPacketConfig1);
        reg &= ~(PACKETCONFIG1_CrcOn | PACKETCONFIG1_PacketFormat);

        if (!fixLen)
          reg |= PACKETCONFIG1_PacketFormat; // variable len
        
        if (crcOn)
          reg |= PACKETCONFIG1_CrcOn;

        writeReg(FSK_RegPacketConfig1, reg);
      }

      break;

    case MODEM_LORA:
      {
        // convert the supplied (legal) LORA bandwidths into something
        // the chip can handle.
        switch (bandwidth) 
          {
          case 125000:
            bandwidth = BW_125;
            break;
            
          case 250000:
            bandwidth = BW_250;
            break;
            
          case 500000:
            bandwidth = BW_500;
            break;
            
          default:
            throw std::runtime_error(std::string(__FUNCTION__) +
                                     ": LORA bandwidth must be 125000, 250000, "
                                     "or 500000");
          }

        m_settings.loraSettings.Bandwidth = bandwidth;
        m_settings.loraSettings.Datarate = datarate;
        m_settings.loraSettings.Coderate = coderate;
        m_settings.loraSettings.FixLen = fixLen;
        m_settings.loraSettings.PayloadLen = payloadLen;
        m_settings.loraSettings.CrcOn = crcOn;
        m_settings.loraSettings.FreqHopOn = freqHopOn;
        m_settings.loraSettings.HopPeriod = hopPeriod;
        m_settings.loraSettings.IqInverted = iqInverted;
        m_settings.loraSettings.RxContinuous = rxContinuous;

        // datarate is really LORA SPREADING_FACTOR_*
        if (datarate > 12)
          {
            datarate = 12;
          }
        else if (datarate < 6)
          {
            datarate = 6;
          }
        
        if ( ((bandwidth == BW_125) && ((datarate == 11) || 
                                        (datarate == 12))) ||
             ((bandwidth == BW_250) && (datarate == 12)) )
          {
            m_settings.loraSettings.LowDatarateOptimize = true;
          }
        else
          {
            m_settings.loraSettings.LowDatarateOptimize = false;
          }

        reg = readReg(LOR_RegModemConfig1);
        reg &= ~((_MODEMCONFIG1_CodingRate_MASK << 
                  _MODEMCONFIG1_CodingRate_SHIFT) |
                 (_MODEMCONFIG1_Bw_MASK << _MODEMCONFIG1_Bw_SHIFT) |
                 MODEMCONFIG1_ImplicitHeaderModeOn);

        if (fixLen)
          reg |= MODEMCONFIG1_ImplicitHeaderModeOn;

        reg |= ((bandwidth & _MODEMCONFIG1_Bw_MASK) << _MODEMCONFIG1_Bw_SHIFT);
        reg |= ((coderate & _MODEMCONFIG1_CodingRate_MASK) << 
                _MODEMCONFIG1_CodingRate_SHIFT);

        writeReg(LOR_RegModemConfig1, reg);

        reg = readReg(LOR_RegModemConfig2);
        reg &= ~((_MODEMCONFIG2_SpreadingFactor_MASK << 
                  _MODEMCONFIG2_SpreadingFactor_SHIFT) |
                 MODEMCONFIG2_RxPayloadCrcOn |
                 (_MODEMCONFIG2_SymbTimeoutMsb_MASK <<
                  _MODEMCONFIG2_SymbTimeoutMsb_SHIFT));

        if (crcOn)
          reg |= MODEMCONFIG2_RxPayloadCrcOn;

        reg |= ((datarate & _MODEMCONFIG2_SpreadingFactor_MASK) << 
                _MODEMCONFIG2_SpreadingFactor_SHIFT);

        // mask symbTimeOut (MSB) for safety
        reg |= ( ((symbTimeout >> 8) & _MODEMCONFIG2_SymbTimeoutMsb_MASK) << 
                 _MODEMCONFIG2_SymbTimeoutMsb_SHIFT);
        writeReg(LOR_RegModemConfig2, reg);

        reg = readReg(LOR_RegModemConfig3);
        
        reg &= ~MODEMCONFIG3_LowDataRateOptimize;

        if (m_settings.loraSettings.LowDatarateOptimize)
          reg |= MODEMCONFIG3_LowDataRateOptimize;

        writeReg(LOR_RegModemConfig3, reg);

        writeReg(LOR_RegSymbTimeoutLsb, (uint8_t)(symbTimeout & 0xff));

            
        writeReg(LOR_RegPreambleMsb, (uint8_t)((preambleLen >> 8) & 0xff));
        writeReg(LOR_RegPreambleLsb, (uint8_t)(preambleLen & 0xff));

        if (fixLen == 1)
          writeReg(LOR_RegPayloadLength, payloadLen);

        // The datasheet says this is only valid in FSK mode, but
        // Semtech code indicates it is only available in LORA
        // mode... So which is it?
        
        // Lets assume for now that the code is correct, as there
        // is a HopPeriod register for LoRa, and no such registers
        // exist for FSK.
        if (m_settings.loraSettings.FreqHopOn)
          {
            reg = readReg(LOR_RegPllHop);
            reg &= ~PLLHOP_FastHopOn;
            reg |= PLLHOP_FastHopOn;
            writeReg(LOR_RegPllHop, reg);

            writeReg(LOR_RegHopPeriod, m_settings.loraSettings.HopPeriod);
          }
        else
          {
            reg = readReg(LOR_RegPllHop);
            reg &= ~PLLHOP_FastHopOn;
            writeReg(LOR_RegPllHop, reg);
          }

        // errata checks - writing magic numbers into undocumented,
        // reserved registers :) The Semtech code was broken in this
        // logic.
        if ( (bandwidth == BW_500) && 
             (m_settings.channel > RF_MID_BAND_THRESH) )
          {
            // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz
            // Bandwidth (HF)
            writeReg(LOR_Reserved36, 0x02);
            writeReg(LOR_Reserved3a, 0x64);
          }
        else if (bandwidth == BW_500 && 
                 (m_settings.channel >= 410000000))
          {
            // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz
            // Bandwidth (LF above 410Mhz)
            writeReg(LOR_Reserved36, 0x02);
            writeReg(LOR_Reserved3a, 0x7f);
          }
        else
          {
            // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz
            // Bandwidth (everything else)
            writeReg(LOR_Reserved36, 0x03);
          }
                 
        // datarate is really LORA spreading factor
        if (datarate == 6)
          {
            // datarate == SPREADINGFACTOR_64
            reg = readReg(LOR_RegDetectOptimize);
            reg &= ~(_DETECTOPTIMIZE_DetectionOptimize_MASK << 
                     _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            reg |= (DETECTIONOPTIMIZE_SF6 << 
                    _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            writeReg(LOR_RegDetectOptimize, reg);

            // see page 27 in the datasheet
            writeReg(LOR_RegDetectionThreshold, LOR_DetectionThreshold_SF6);
          }
        else
          {
            reg = readReg(LOR_RegDetectOptimize);
            reg &= ~(_DETECTOPTIMIZE_DetectionOptimize_MASK << 
                     _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            reg |= (DETECTIONOPTIMIZE_SF7_SF12 << 
                    _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            writeReg(LOR_RegDetectOptimize, reg);

            // see page 27 in the datasheet
            writeReg(LOR_RegDetectionThreshold, 
                     LOR_DetectionThreshold_SF7_SF12);
          }
      }
      break;
    }
}

void SX1276::setTxConfig(RADIO_MODEM_T modem, int8_t power, 
                         uint32_t fdev, 
                         uint32_t bandwidth, uint32_t datarate,
                         uint8_t coderate, uint16_t preambleLen,
                         bool fixLen, bool crcOn, bool freqHopOn, 
                         uint8_t hopPeriod, bool iqInverted)
{
  uint8_t paConfig = 0;
  uint8_t paDac = 0;
  
  setModem(modem);
    
  paConfig = readReg(COM_RegPaConfig);
  paDac = readReg(COM_RegPaDac);

  uint8_t paSelect = 0x00; // default, +14dBm
  if (m_settings.channel < RF_MID_BAND_THRESH)
    paSelect = PACONFIG_PaSelect; // PA_BOOST, +20dBm
  
  paConfig &= ~PACONFIG_PaSelect;
  paConfig |= paSelect;
  paConfig &= ~(_PACONFIG_MaxPower_MASK << _PACONFIG_MaxPower_SHIFT);
  paConfig |= (7 << _PACONFIG_MaxPower_SHIFT); // PACONFIG_MaxPower = 7

  if ((paConfig & PACONFIG_PaSelect))
    {
      if (power > 17)
        {
          paDac &= ~(_PADAC_PaDac_MASK << _PADAC_PaDac_SHIFT);
          paDac |= (PADAC_BOOST << _PADAC_PaDac_SHIFT);
        }
      else
        {
          paDac &= ~(_PADAC_PaDac_MASK << _PADAC_PaDac_SHIFT);
          paDac |= (PADAC_DEFAULT << _PADAC_PaDac_SHIFT);
        }

      if ((paDac & PADAC_BOOST) == PADAC_BOOST)
        {
          if (power < 5)
            {
              power = 5;
            }
          if (power > 20)
            {
              power = 20;
            }
          paConfig = ~(_PACONFIG_OutputPower_MASK & _PACONFIG_OutputPower_SHIFT);
          paConfig |= ( ((uint8_t)(power - 5) & 
                         _PACONFIG_OutputPower_MASK) << 
                        _PACONFIG_OutputPower_SHIFT );
        }
      else
        {
          if (power < 2)
            {
              power = 2;
            }
          if (power > 17)
            {
              power = 17;
            }
          
          paConfig = ~(_PACONFIG_OutputPower_MASK & _PACONFIG_OutputPower_SHIFT);
          paConfig |= ( ((uint8_t)(power - 2) & 
                         _PACONFIG_OutputPower_MASK) << 
                        _PACONFIG_OutputPower_SHIFT );
        }
    }
  else
    {
      if (power < -1)
        {
          power = -1;
        }
      if (power > 14)
        {
          power = 14;
        }

      paConfig = ~(_PACONFIG_OutputPower_MASK & _PACONFIG_OutputPower_SHIFT);
      paConfig |= ( ((uint8_t)(power + 1) & 
                    _PACONFIG_OutputPower_MASK) << 
                    _PACONFIG_OutputPower_SHIFT );
    }
  writeReg(COM_RegPaConfig, paConfig);
  writeReg(COM_RegPaDac, paDac);

  uint8_t reg;

  switch (modem)
    {
    case MODEM_FSK:
      {
        m_settings.fskSettings.Power = power;
        m_settings.fskSettings.Fdev = fdev;
        m_settings.fskSettings.Bandwidth = bandwidth;
        m_settings.fskSettings.Datarate = datarate;
        m_settings.fskSettings.PreambleLen = preambleLen;
        m_settings.fskSettings.FixLen = fixLen;
        m_settings.fskSettings.CrcOn = crcOn;
        m_settings.fskSettings.IqInverted = iqInverted;
            
        fdev = (uint16_t)((double)fdev / FXOSC_STEP);
        writeReg(FSK_RegFdevMsb, (uint8_t)(fdev >> 8));
        writeReg(FSK_RegFdevLsb, (uint8_t)(fdev & 0xFF));

        datarate = (uint16_t)(FXOSC_FREQ / (double)datarate);
        writeReg(FSK_RegBitrateMsb, (uint8_t)(datarate >> 8));
        writeReg(FSK_RegBitrateLsb, (uint8_t)(datarate & 0xff));
        
        writeReg(FSK_RegPreambleMsb, (uint8_t)(preambleLen >> 8));
        writeReg(FSK_RegPreambleLsb, (uint8_t)(preambleLen & 0xff));
        
        reg = readReg(FSK_RegPacketConfig1);
        reg &= ~(PACKETCONFIG1_CrcOn | PACKETCONFIG1_PacketFormat);

        if (!fixLen)
          reg |= PACKETCONFIG1_PacketFormat; // variable len

        if (crcOn)
          reg |= PACKETCONFIG1_CrcOn;

        writeReg(FSK_RegPacketConfig1, reg);
      }
      break;
      
    case MODEM_LORA:
      {
        m_settings.loraSettings.Power = power;

        // we convert bandwidth into appropriate BW_* constants for LORA
        switch (bandwidth) {
        case 125000:
          bandwidth = BW_125;
          break;

        case 250000:
          bandwidth = BW_250;
          break;

        case 500000:
          bandwidth = BW_500;
          break;

        default:
          throw std::runtime_error(std::string(__FUNCTION__) +
                                   ": LORA bandwidth must be 125000, 250000, "
                                   "or 500000");
        }


        m_settings.loraSettings.Bandwidth = bandwidth;
        m_settings.loraSettings.Datarate = datarate;
        m_settings.loraSettings.Coderate = coderate;
        m_settings.loraSettings.PreambleLen = preambleLen;
        m_settings.loraSettings.FixLen = fixLen;
        m_settings.loraSettings.FreqHopOn = freqHopOn;
        m_settings.loraSettings.HopPeriod = hopPeriod;
        m_settings.loraSettings.CrcOn = crcOn;
        m_settings.loraSettings.IqInverted = iqInverted;
        
        // datarate is really SPREADINGFACTOR_* for LoRa
        if (datarate > 12)
          {
            datarate = 12;
          }
        else if (datarate < 6)
          {
            datarate = 6;
          }
        
        if ( ((bandwidth == BW_125) && ((datarate == 11) || 
                                        (datarate == 12))) ||
             ((bandwidth == BW_250) && (datarate == 12)) )
          {
            m_settings.loraSettings.LowDatarateOptimize = true;
          }
        else
          {
            m_settings.loraSettings.LowDatarateOptimize = false;
          }
        

        // datasheet says this is only valid in FSK mode, but Semtech
        // code indicates it is only available in LORA mode... So
        // which is it?
        
        // Lets assume for now that the code is correct, as there
        // is a HopPeriod register for LoRa, and no such registers
        // exist for FSK.
        if (m_settings.loraSettings.FreqHopOn == true)
          {
            reg = readReg(LOR_RegPllHop);
            reg &= ~PLLHOP_FastHopOn;
            reg |= PLLHOP_FastHopOn;
            writeReg(LOR_RegPllHop, reg);

            writeReg(LOR_RegHopPeriod, m_settings.loraSettings.HopPeriod);
          }
        else
          {
            reg = readReg(LOR_RegPllHop);
            reg &= ~PLLHOP_FastHopOn;
            writeReg(LOR_RegPllHop, reg);
          }
        
        reg = readReg(LOR_RegModemConfig1);
        reg &= ~((_MODEMCONFIG1_CodingRate_MASK << 
                  _MODEMCONFIG1_CodingRate_SHIFT) |
                 (_MODEMCONFIG1_Bw_MASK << _MODEMCONFIG1_Bw_SHIFT) |
                 MODEMCONFIG1_ImplicitHeaderModeOn);

        if (fixLen)
          reg |= MODEMCONFIG1_ImplicitHeaderModeOn;

        reg |= ((bandwidth & _MODEMCONFIG1_Bw_MASK) << _MODEMCONFIG1_Bw_SHIFT);
        reg |= ((coderate & _MODEMCONFIG1_CodingRate_MASK) << 
                _MODEMCONFIG1_CodingRate_SHIFT);

        writeReg(LOR_RegModemConfig1, reg);

        reg = readReg(LOR_RegModemConfig2);
        reg &= ~((_MODEMCONFIG2_SpreadingFactor_MASK << 
                  _MODEMCONFIG2_SpreadingFactor_SHIFT) |
                 MODEMCONFIG2_RxPayloadCrcOn);

        if (crcOn)
          reg |= MODEMCONFIG2_RxPayloadCrcOn;

        reg |= ((datarate & _MODEMCONFIG2_SpreadingFactor_MASK) << 
                _MODEMCONFIG2_SpreadingFactor_SHIFT);
        writeReg(LOR_RegModemConfig2, reg);

        reg = readReg(LOR_RegModemConfig3);
        
        reg &= ~MODEMCONFIG3_LowDataRateOptimize;

        if (m_settings.loraSettings.LowDatarateOptimize)
          reg |= MODEMCONFIG3_LowDataRateOptimize;

        writeReg(LOR_RegModemConfig3, reg);

        writeReg(LOR_RegPreambleMsb, (uint8_t)((preambleLen >> 8) & 0xff));
        writeReg(LOR_RegPreambleLsb, (uint8_t)(preambleLen & 0xff));
        
        // datarate is SPREADINGFACTOR_*
        if (datarate == 6)
          {
            reg = readReg(LOR_RegDetectOptimize);
            reg &= ~(_DETECTOPTIMIZE_DetectionOptimize_MASK << 
                     _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            reg |= (DETECTIONOPTIMIZE_SF6 << 
                    _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            writeReg(LOR_RegDetectOptimize, reg);

            // see page 27 in the datasheet
            writeReg(LOR_RegDetectionThreshold, LOR_DetectionThreshold_SF6);

          }
        else
          {
            reg = readReg(LOR_RegDetectOptimize);
            reg &= ~(_DETECTOPTIMIZE_DetectionOptimize_MASK << 
                     _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            reg |= (DETECTIONOPTIMIZE_SF7_SF12 << 
                    _DETECTOPTIMIZE_DetectionOptimize_SHIFT);

            writeReg(LOR_RegDetectOptimize, reg);

            // see page 27 in the datasheet
            writeReg(LOR_RegDetectionThreshold, 
                     LOR_DetectionThreshold_SF7_SF12);
          }
      }

      break;
    }
}

SX1276::RADIO_EVENT_T SX1276::setTx(int timeout)
{
  uint8_t reg = 0;

  switch (m_settings.modem)
    {
    case MODEM_FSK:
      {
        // DIO0=PacketSent
        // DIO1=FifoLevel
        // DIO2=FifoFull
        // DIO3=FifoEmpty
        // DIO4=LowBat
        // DIO5=ModeReady

        reg = readReg(COM_RegDioMapping1);
        reg &= ~( (DOIMAPPING1_Dio0Mapping_MASK << 
                   DOIMAPPING1_Dio0Mapping_SHIFT) |
                  (DOIMAPPING1_Dio2Mapping_MASK << 
                   DOIMAPPING1_Dio2Mapping_SHIFT) );

        writeReg(COM_RegDioMapping1, reg);


        reg = readReg(COM_RegDioMapping2);
        reg &= ~( (DOIMAPPING2_Dio4Mapping_MASK << 
                   DOIMAPPING2_Dio4Mapping_SHIFT) |
                  (DOIMAPPING2_Dio5Mapping_MASK << 
                   DOIMAPPING2_Dio5Mapping_SHIFT) );

        writeReg(COM_RegDioMapping2, reg);

        m_settings.fskPacketHandler.FifoThresh = 
          (readReg(FSK_RegFifoThresh) & 
           (_FIFOTHRESH_FifoThreshold_MASK << _FIFOTHRESH_FifoThreshold_SHIFT));
      }

      break;

    case MODEM_LORA:
      {
        if (m_settings.loraSettings.FreqHopOn == true )
          {
            // mask out all except TxDone and FhssChangeChannel
            writeReg(LOR_RegIrqFlagsMask, 
                     LOR_IRQFLAG_RxTimeout |
                     LOR_IRQFLAG_RxDone |
                     LOR_IRQFLAG_PayloadCrcError |
                     LOR_IRQFLAG_ValidHeader |
                     // LOR_IRQFLAG_TxDone |
                     LOR_IRQFLAG_CadDone |
                     // LOR_IRQFLAG_FhssChangeChannel |
                     LOR_IRQFLAG_CadDetected);
            
            // DIO0=TxDone, DIO2=FhssChangeChannel
            reg = readReg(COM_RegDioMapping1);
            reg &= ~( (DOIMAPPING1_Dio0Mapping_MASK << 
                       DOIMAPPING1_Dio0Mapping_SHIFT) |
                      (DOIMAPPING1_Dio2Mapping_MASK << 
                       DOIMAPPING1_Dio2Mapping_SHIFT) );
            reg |= ( (DIOMAPPING_01 << DOIMAPPING1_Dio0Mapping_SHIFT) |
                     (DIOMAPPING_00 << DOIMAPPING1_Dio2Mapping_SHIFT) );
            writeReg(COM_RegDioMapping1, reg);
          }
        else
          {
            // mask out all except TxDone
            writeReg(LOR_RegIrqFlagsMask, 
                     LOR_IRQFLAG_RxTimeout |
                     LOR_IRQFLAG_RxDone |
                     LOR_IRQFLAG_PayloadCrcError |
                     LOR_IRQFLAG_ValidHeader |
                     // LOR_IRQFLAG_TxDone |
                     LOR_IRQFLAG_CadDone |
                     LOR_IRQFLAG_FhssChangeChannel |
                     LOR_IRQFLAG_CadDetected);

            // DIO0=TxDone
            reg = readReg(COM_RegDioMapping1);
            reg &= ~( (DOIMAPPING1_Dio0Mapping_MASK << 
                       DOIMAPPING1_Dio0Mapping_SHIFT) );
            reg |= (DIOMAPPING_01 << DOIMAPPING1_Dio0Mapping_SHIFT);
            writeReg(COM_RegDioMapping1, reg);
          }
      }
      break;
    }

  m_settings.state = STATE_TX_RUNNING;
  m_radioEvent = REVENT_EXEC;

  setOpMode(MODE_TxMode);

  initClock();
  while ((getMillis() < timeout) && m_radioEvent == REVENT_EXEC)
    usleep(100);

  if (m_radioEvent == REVENT_EXEC)
    {
      // timeout
      m_radioEvent = REVENT_TIMEOUT;
    }

  return m_radioEvent;
}

SX1276::RADIO_EVENT_T SX1276::setRx(uint32_t timeout)
{
  bool rxContinuous = false;
  uint8_t reg = 0;

  switch (m_settings.modem)
    {
    case MODEM_FSK:
      {
        rxContinuous = m_settings.fskSettings.RxContinuous;
            
        // DIO0=PayloadReady
        // DIO1=FifoLevel
        // DIO2=SyncAddr
        // DIO3=FifoEmpty
        // DIO4=Preamble
        // DIO5=ModeReady
        reg = readReg(COM_RegDioMapping1);
        reg &= ~( (DOIMAPPING1_Dio0Mapping_MASK << 
                   DOIMAPPING1_Dio0Mapping_SHIFT) |
                  (DOIMAPPING1_Dio2Mapping_MASK << 
                   DOIMAPPING1_Dio2Mapping_SHIFT) );
        reg |= ( (DIOMAPPING_00 << DOIMAPPING1_Dio0Mapping_SHIFT) |
                 (DIOMAPPING_11 << DOIMAPPING1_Dio2Mapping_SHIFT) );
        writeReg(COM_RegDioMapping1, reg);

            
        reg = readReg(COM_RegDioMapping2);
        reg &= ~( (DOIMAPPING2_Dio4Mapping_MASK << 
                   DOIMAPPING2_Dio4Mapping_SHIFT) |
                  (DOIMAPPING2_Dio5Mapping_MASK << 
                   DOIMAPPING2_Dio5Mapping_SHIFT) );
        reg |= (DIOMAPPING_11 << DOIMAPPING2_Dio4Mapping_SHIFT) |
          DOIMAPPING2_MapPreambleDetect;

        writeReg(COM_RegDioMapping2, reg);

        m_settings.fskPacketHandler.FifoThresh = 
          (readReg(FSK_RegFifoThresh) & _FIFOTHRESH_FifoThreshold_MASK);
            
        m_settings.fskPacketHandler.PreambleDetected = false;
        m_settings.fskPacketHandler.SyncWordDetected = false;
        m_settings.fskPacketHandler.NbBytes = 0;
        m_settings.fskPacketHandler.Size = 0;
      }

      break;

    case MODEM_LORA:
      {
        // The datasheet does not mention anything other than an
        // InvertIQ bit (0x40) in RegInvertIQ register (0x33).  Here,
        // we seem to have two bits in RegInvertIQ (existing one for
        // RX), and a 'new' one for TXOff (0x01).  In addition,
        // INVERTIQ2 (0x3b) does not exist in the datasheet, it is
        // marked as reserved. We will assume that the datasheet is
        // out of date.

        if (m_settings.loraSettings.IqInverted == true)
          {
            reg = readReg(LOR_RegInvertIQ);
            reg &= ~(INVERTIQ_InvertIQTxOff | INVERTIQ_InvertIQRx);
            reg |= INVERTIQ_InvertIQRx;
            writeReg(LOR_RegInvertIQ, reg);

            // warning, hardcoded undocumented magic number into
            // undocumented register
            writeReg(LOR_RegInvertIQ2, 0x19);
          }
        else
          {
            reg = readReg(LOR_RegInvertIQ);
            reg &= ~(INVERTIQ_InvertIQTxOff | INVERTIQ_InvertIQRx);
            reg |= INVERTIQ_InvertIQTxOff; // 'active' off.
            writeReg(LOR_RegInvertIQ, reg);

            // warning, hardcoded undocumented magic number into
            // undocumented register
            writeReg(LOR_RegInvertIQ2, 0x1d);
          }         

        // ERRATA 2.3 - Receiver Spurious Reception of a LoRa Signal
        if (m_settings.loraSettings.Bandwidth < 9)
          {
            reg = readReg(LOR_RegDetectOptimize);
            reg &= 0x7f; // clear undocumented bit 7
            writeReg(LOR_RegDetectOptimize, reg);

            // warning, writing magic numbers into undocumented
            // registers
            switch (m_settings.loraSettings.Bandwidth)
              {
              case 0: // 7.8 kHz
                writeReg(LOR_Reserved2f, 0x48);
                setChannel(m_settings.channel + 7.81e3);
                break;
              case 1: // 10.4 kHz
                writeReg(LOR_Reserved2f, 0x44);
                setChannel(m_settings.channel + 10.42e3);
                break;
              case 2: // 15.6 kHz
                writeReg(LOR_Reserved2f, 0x44);
                setChannel(m_settings.channel + 15.62e3);
                break;
              case 3: // 20.8 kHz
                writeReg(LOR_Reserved2f, 0x44);
                setChannel(m_settings.channel + 20.83e3);
                break;
              case 4: // 31.2 kHz
                writeReg(LOR_Reserved2f, 0x44);
                setChannel(m_settings.channel + 31.25e3);
                break;
              case 5: // 41.4 kHz
                writeReg(LOR_Reserved2f, 0x44);
                setChannel(m_settings.channel + 41.67e3);
                break;
              case 6: // 62.5 kHz
                writeReg(LOR_Reserved2f, 0x40);
                break;
              case 7: // 125 kHz
                writeReg(LOR_Reserved2f, 0x40);
                break;
              case 8: // 250 kHz
                writeReg(LOR_Reserved2f, 0x40);
                break;
              }
          }
        else
          {
            reg = readReg(LOR_RegDetectOptimize);
            reg |= 0x80; // set undocumented bit 7
            writeReg(LOR_RegDetectOptimize, reg);
          }

        rxContinuous = m_settings.loraSettings.RxContinuous;
            
        if (m_settings.loraSettings.FreqHopOn == true)
          {
            // mask out all except RxDone, RxTimeout, PayloadCrCError,
            // and FhssChangeChannel
            writeReg(LOR_RegIrqFlagsMask, 
                     // LOR_IRQFLAG_RxTimeout |
                     // LOR_IRQFLAG_RxDone |
                     // LOR_IRQFLAG_PayloadCrcError |
                     LOR_IRQFLAG_ValidHeader |
                     LOR_IRQFLAG_TxDone |
                     LOR_IRQFLAG_CadDone |
                     // LOR_IRQFLAG_FhssChangeChannel |
                     LOR_IRQFLAG_CadDetected);

            // DIO0=RxDone, DIO2=FhssChangeChannel
            reg = readReg(COM_RegDioMapping1);
            reg &= ~( (DOIMAPPING1_Dio0Mapping_MASK << 
                       DOIMAPPING1_Dio0Mapping_SHIFT) |
                      (DOIMAPPING1_Dio2Mapping_MASK << 
                       DOIMAPPING1_Dio2Mapping_SHIFT) );
            reg |= ( (DIOMAPPING_00 << DOIMAPPING1_Dio0Mapping_SHIFT) |
                     (DIOMAPPING_00 << DOIMAPPING1_Dio2Mapping_SHIFT) );
            writeReg(COM_RegDioMapping1, reg);
          }
        else
          {
            // mask out all except RxDone, RxTimeout, and PayloadCrCError
            writeReg(LOR_RegIrqFlagsMask, 
                     // LOR_IRQFLAG_RxTimeout |
                     // LOR_IRQFLAG_RxDone |
                     // LOR_IRQFLAG_PayloadCrcError |
                     LOR_IRQFLAG_ValidHeader |
                     LOR_IRQFLAG_TxDone |
                     LOR_IRQFLAG_CadDone |
                     LOR_IRQFLAG_FhssChangeChannel |
                     LOR_IRQFLAG_CadDetected);

            // DIO0=RxDone
            reg = readReg(COM_RegDioMapping1);
            reg &= ~(DOIMAPPING1_Dio0Mapping_MASK << 
                     DOIMAPPING1_Dio0Mapping_SHIFT);
            reg |= (DIOMAPPING_00 << DOIMAPPING1_Dio0Mapping_SHIFT);
            writeReg(COM_RegDioMapping1, reg);
          }

        writeReg(LOR_RegFifoRxBaseAddr, 0);
        writeReg(LOR_RegFifoAddrPtr, 0);
      }

      break;

    }

  memset(m_rxBuffer, 0, FIFO_SIZE);

  m_settings.state = STATE_RX_RUNNING;
  m_radioEvent = REVENT_EXEC;

  if (m_settings.modem == MODEM_FSK)
    {
      setOpMode(MODE_FSK_RxMode);
        
      if (rxContinuous == false)
        {
          // timer..?
#if 0
          TimerSetValue( &RxTimeoutSyncWord, ( 8.0 * ( m_settings.fsk.PreambleLen +
                                                       ( ( SX1276Read( REG_SYNCCONFIG ) &
                                                           ~RF_SYNCCONFIG_SYNCSIZE_MASK ) +
                                                         1.0 ) + 10.0 ) /
                                               ( double )m_settings.fsk.Datarate ) * 1e6 );
          TimerStart( &RxTimeoutSyncWord );
#endif
        }
    }
  else
    {
      // LoRa

      if (rxContinuous == true)
        {
          setOpMode(MODE_LOR_RxContinuous);
        }
      else
        {
          setOpMode(MODE_LOR_RxSingle);
        }
    }

  initClock();
  while ((getMillis() < timeout) && m_radioEvent == REVENT_EXEC)
    usleep(100);

  if (m_radioEvent == REVENT_EXEC)
    {
      // timeout
      m_radioEvent = REVENT_TIMEOUT;
    }

  return m_radioEvent;
}


void SX1276::startCAD()
{
  switch (m_settings.modem)
    {
    case MODEM_LORA:
      {
        // mask out all except CadDone and CadDetected
        writeReg(LOR_RegIrqFlagsMask, 
                 LOR_IRQFLAG_RxTimeout |
                 LOR_IRQFLAG_RxDone |
                 LOR_IRQFLAG_PayloadCrcError |
                 LOR_IRQFLAG_ValidHeader |
                 LOR_IRQFLAG_TxDone |
                 // LOR_IRQFLAG_CadDone |
                 LOR_IRQFLAG_FhssChangeChannel //|
                 // LOR_IRQFLAG_CadDetected
                 );

        // DIO3=CADDone

        uint8_t reg;
        reg = readReg(COM_RegDioMapping1);
        reg &= ~(DOIMAPPING1_Dio3Mapping_MASK << 
                 DOIMAPPING1_Dio3Mapping_SHIFT);
        reg |= (DIOMAPPING_00 << DOIMAPPING1_Dio3Mapping_SHIFT);
        writeReg(COM_RegDioMapping1, reg);
        
        m_settings.state = STATE_CAD;
        setOpMode(MODE_LOR_CAD);
      }

      break;

    case MODEM_FSK:
    default:
      break;
    }
}

void SX1276::setMaxPayloadLength(RADIO_MODEM_T modem, uint8_t max)
{
  setModem(modem);
  
  switch (modem)
    {
    case MODEM_FSK:
      if (m_settings.fskSettings.FixLen == false)
        {
          writeReg(FSK_RegPayloadLength, max);
        }

      break;

    case MODEM_LORA:
      writeReg(LOR_RegMaxPayloadLength, max);

      break;
    }
}


void SX1276::onDio0Irq(void *ctx)
{
  upm::SX1276 *This = (upm::SX1276 *)ctx;

  This->lockIntrs();

  volatile uint8_t irqFlags = 0;

  //  cerr << __FUNCTION__ << ": Enter" << endl;

  switch (This->m_settings.state)
    {
    case STATE_RX_RUNNING:

      // RxDone interrupt
      switch (This->m_settings.modem)
        {
        case MODEM_FSK:

          if (This->m_settings.fskSettings.CrcOn == true )
            {
              irqFlags = This->readReg(FSK_RegIrqFlags2);

              if (!(irqFlags & IRQFLAGS2_CrcOk))
                {
                  // Clear Irqs
                  This->writeReg(FSK_RegIrqFlags1,
                                 IRQFLAGS1_Rssi | 
                                 IRQFLAGS1_PreambleDetect |
                                 IRQFLAGS1_SyncAddressMatch);
                  This->writeReg(FSK_RegIrqFlags2, IRQFLAGS2_FifoOverrun);

                  if (This->m_settings.fskSettings.RxContinuous == false )
                    {
                      This->m_settings.state = STATE_IDLE;
                    }
                  else
                    {
                      // Continuous mode restart Rx chain
                      This->writeReg(FSK_RegRxConfig,
                                     This->readReg(FSK_RegRxConfig) | 
                                     RXCONFIG_RestartRxWithoutPllLock);
                    }

                  // RxError radio event
                  //                  cerr << __FUNCTION__ << ": RxError crc/sync timeout" << endl;
                  This->m_radioEvent = REVENT_ERROR;

                  This->m_settings.fskPacketHandler.PreambleDetected = false;
                  This->m_settings.fskPacketHandler.SyncWordDetected = false;
                  This->m_settings.fskPacketHandler.NbBytes = 0;
                  This->m_settings.fskPacketHandler.Size = 0;
                  break;
                }
            }

          // Read received packet size
          if ( (This->m_settings.fskPacketHandler.Size == 0) && 
               (This->m_settings.fskPacketHandler.NbBytes == 0) )
            {
              if (This->m_settings.fskSettings.FixLen == false )
                {
                  This->readFifo((uint8_t*)&(This->m_settings.fskPacketHandler.Size),
                                 1);
                }
              else
                {
                  This->m_settings.fskPacketHandler.Size = 
                    This->readReg(FSK_RegPayloadLength);
                }

              This->readFifo(This->m_rxBuffer + 
                             This->m_settings.fskPacketHandler.NbBytes, 
                             This->m_settings.fskPacketHandler.Size - 
                             This->m_settings.fskPacketHandler.NbBytes);

              This->m_settings.fskPacketHandler.NbBytes += 
                (This->m_settings.fskPacketHandler.Size - 
                 This->m_settings.fskPacketHandler.NbBytes);
            }
          else
            {
              This->readFifo(This->m_rxBuffer + 
                             This->m_settings.fskPacketHandler.NbBytes, 
                             (This->m_settings.fskPacketHandler.Size - 
                              This->m_settings.fskPacketHandler.NbBytes));

              This->m_settings.fskPacketHandler.NbBytes += 
                (This->m_settings.fskPacketHandler.Size - 
                 This->m_settings.fskPacketHandler.NbBytes);
            }

          if (This->m_settings.fskSettings.RxContinuous == false)
            {
              This->m_settings.state = STATE_IDLE;
            }
          else
            {
              // Continuous mode restart Rx chain
              This->writeReg(FSK_RegRxConfig,
                             This->readReg(FSK_RegRxConfig) | 
                             RXCONFIG_RestartRxWithoutPllLock);
            }

          // RxDone radio event
          This->m_rxRSSI = This->m_settings.fskPacketHandler.RssiValue;
          This->m_rxLen = This->m_settings.fskPacketHandler.Size;
          This->m_radioEvent = REVENT_DONE;
          // cerr << __FUNCTION__ << ": FSK RxDone" << endl;
          // fprintf(stderr, "### %s: RX(%d): %s\n", 
          //         __FUNCTION__, 
          //         This->m_settings.fskPacketHandler.Size,
          //         This->m_rxBuffer);

          This->m_settings.fskPacketHandler.PreambleDetected = false;
          This->m_settings.fskPacketHandler.SyncWordDetected = false;
          This->m_settings.fskPacketHandler.NbBytes = 0;
          This->m_settings.fskPacketHandler.Size = 0;

          break;

        case MODEM_LORA:
          {
            int8_t snr = 0;

            // Clear Irq
            This->writeReg(LOR_RegIrqFlags, LOR_IRQFLAG_RxDone);

            irqFlags = This->readReg(LOR_RegIrqFlags);

            // cerr << "LORA PayloadCRC on = " 
            //      <<  hex << (int)This->readReg(LOR_RegHopChannel) << dec << endl;
            if (irqFlags & LOR_IRQFLAG_PayloadCrcError)
              {
                // Clear Irq
                This->writeReg(LOR_RegIrqFlags, LOR_IRQFLAG_PayloadCrcError);
                if (This->m_settings.loraSettings.RxContinuous == false)
                  {
                    This->m_settings.state = STATE_IDLE;
                  }
                // RxError radio event
                // cerr << __FUNCTION__ << ": RxError (payload crc error)" << endl;
                This->m_radioEvent = REVENT_ERROR;

                break;
              }

            This->m_settings.loraPacketHandler.SnrValue = 
              This->readReg(LOR_RegPktSnrValue);

            if (This->m_settings.loraPacketHandler.SnrValue & 0x80)
              {
                // The SNR sign bit is 1
                // Invert and divide by 4
                snr = ( (~(This->m_settings.loraPacketHandler.SnrValue) + 1 ) & 
                        0xff) >> 2;
                snr = -snr;
              }
            else
              {
                // Divide by 4
                snr = (This->m_settings.loraPacketHandler.SnrValue & 0xff) >> 2;
              }

            int16_t rssi = This->readReg(LOR_RegPktRssiValue);

            if (snr < 0)
              {
                if (This->m_settings.channel > RF_MID_BAND_THRESH)
                  {
                    This->m_settings.loraPacketHandler.RssiValue =
                      LOR_RSSI_OFFSET_HF + rssi + ( rssi >> 4 ) + snr;
                  }
                else
                  {
                    This->m_settings.loraPacketHandler.RssiValue =
                      LOR_RSSI_OFFSET_LF + rssi + ( rssi >> 4 ) + snr;
                  }
              }
            else
              {
                if (This->m_settings.channel > RF_MID_BAND_THRESH)
                  {
                    This->m_settings.loraPacketHandler.RssiValue =
                      LOR_RSSI_OFFSET_HF + rssi + (rssi >> 4);
                  }
                else
                  {
                    This->m_settings.loraPacketHandler.RssiValue =
                      LOR_RSSI_OFFSET_LF + rssi + (rssi >> 4);
                  }
              }

            This->m_settings.loraPacketHandler.Size = 
              This->readReg(LOR_RegRxNbBytes);

            // cerr << "LORA HANDLER SIZE = " 
            //      <<  (int)This->m_settings.loraPacketHandler.Size << endl;

            // cerr << "LORA MAXPAYLOAD = " 
            //      <<  (int)This->readReg(LOR_RegMaxPayloadLength) << endl;

            This->readFifo(This->m_rxBuffer, 
                           This->m_settings.loraPacketHandler.Size);

            if (This->m_settings.loraSettings.RxContinuous == false)
              {
                This->m_settings.state = STATE_IDLE;
              }

            // RxDone radio event
            
            // The returned size (from LOR_RegRxNbBytes) is always 64
            // bytes regardless of the packet size I sent.  Something
            // is wrong here.
            //            cerr << __FUNCTION__ << ": RxDone (LORA)" << endl;
            This->m_rxRSSI = (int)rssi;
            This->m_rxSNR = (int)snr;
            This->m_rxLen = This->m_settings.loraPacketHandler.Size;
            This->m_radioEvent = REVENT_DONE;
            // if (This->m_settings.state == STATE_RX_RUNNING)
            //   fprintf(stderr, "### %s: snr = %d rssi = %d RX(%d): %s\n", 
            //           __FUNCTION__, 
            //           (int)snr, (int)rssi,
            //           This->m_settings.loraPacketHandler.Size,
            //           This->m_rxBuffer);
            // else
            //   fprintf(stderr, "### %s: snr = %d rssi = %d RX: INV BUFFER (crc)\n", __FUNCTION__, 
            //         (int)snr, (int)rssi);

          }

          break;

        default:
          break;

        }

      break;

    case STATE_TX_RUNNING:

      // TxDone interrupt
      switch (This->m_settings.modem)
        {
        case MODEM_LORA:
          // Clear Irq
          This->writeReg(LOR_RegIrqFlags, LOR_IRQFLAG_TxDone);
          // fprintf(stderr, "%s: LORA IrqFlags = %02x\n", __FUNCTION__,
          //         This->readReg(LOR_RegIrqFlags));
          // Intentional fall through
   
        case MODEM_FSK:
        default:
          This->m_settings.state = STATE_IDLE;

          // TxDone radio event
          This->m_radioEvent = REVENT_DONE;
          //          cerr << __FUNCTION__ << ": TxDone" << endl;

          break;
        }
      break;

    default:
      break;
    }

  This->unlockIntrs();
}


void SX1276::onDio1Irq(void *ctx)
{
  upm::SX1276 *This = (upm::SX1276 *)ctx;

  This->lockIntrs();
  //  cerr << __FUNCTION__ << ": Enter" << endl;

  switch (This->m_settings.state)
    {                
    case STATE_RX_RUNNING:

      switch (This->m_settings.modem)
        {
        case MODEM_FSK:
          // FifoLevel interrupt
          // Read received packet size
          if ( (This->m_settings.fskPacketHandler.Size == 0 ) && 
               (This->m_settings.fskPacketHandler.NbBytes == 0) )
            {
              if (This->m_settings.fskSettings.FixLen == false)
                {
                  This->readFifo((uint8_t*)&(This->m_settings.fskPacketHandler.Size),
                                 1);
                }
              else
                {
                  This->m_settings.fskPacketHandler.Size = 
                    This->readReg(FSK_RegPayloadLength);
                }
            }
          
          if ( (This->m_settings.fskPacketHandler.Size - 
                This->m_settings.fskPacketHandler.NbBytes) > 
               This->m_settings.fskPacketHandler.FifoThresh)
            {
              This->readFifo((This->m_rxBuffer + 
                              This->m_settings.fskPacketHandler.NbBytes), 
                             This->m_settings.fskPacketHandler.FifoThresh);
              This->m_settings.fskPacketHandler.NbBytes += 
                This->m_settings.fskPacketHandler.FifoThresh;
            }
          else
            {
              This->readFifo((This->m_rxBuffer +
                              This->m_settings.fskPacketHandler.NbBytes),
                             This->m_settings.fskPacketHandler.Size - 
                             This->m_settings.fskPacketHandler.NbBytes);
              This->m_settings.fskPacketHandler.NbBytes += 
                (This->m_settings.fskPacketHandler.Size - 
                 This->m_settings.fskPacketHandler.NbBytes);
            }

          break;

        case MODEM_LORA:
          // Sync time out
          This->m_settings.state = STATE_IDLE;
          // RxError (LORA timeout) radio events
          //          cerr << __FUNCTION__ << ": RxTimeout (LORA)" << endl;
          This->m_radioEvent = REVENT_TIMEOUT;

          break;

        default:
          break;
        }

      break;

    case STATE_TX_RUNNING:

      switch (This->m_settings.modem )
        {
        case MODEM_FSK:
          // FifoLevel interrupt
          if ( (This->m_settings.fskPacketHandler.Size - 
                This->m_settings.fskPacketHandler.NbBytes) > 
               This->m_settings.fskPacketHandler.ChunkSize)
            {
              This->writeFifo((This->m_rxBuffer + 
                               This->m_settings.fskPacketHandler.NbBytes), 
                              This->m_settings.fskPacketHandler.ChunkSize);
              This->m_settings.fskPacketHandler.NbBytes += 
                This->m_settings.fskPacketHandler.ChunkSize;
            }
          else 
            {
              // Write the last chunk of data
              This->writeFifo((This->m_rxBuffer +
                               This->m_settings.fskPacketHandler.NbBytes),
                              This->m_settings.fskPacketHandler.Size - 
                              This->m_settings.fskPacketHandler.NbBytes);
              This->m_settings.fskPacketHandler.NbBytes += 
                (This->m_settings.fskPacketHandler.Size - 
                 This->m_settings.fskPacketHandler.NbBytes);
            }
          break;

        case MODEM_LORA:
          break;

        default:
          break;
        }
      break;      

    default:
      break;
    }

  This->unlockIntrs();
}

void SX1276::onDio2Irq(void *ctx)
{
  upm::SX1276 *This = (upm::SX1276 *)ctx;

  This->lockIntrs();
  //  cerr << __FUNCTION__ << ": Enter" << endl;

  switch (This->m_settings.state)
    {
    case STATE_RX_RUNNING:

      switch (This->m_settings.modem)
        {
        case MODEM_FSK:
          if ( (This->m_settings.fskPacketHandler.PreambleDetected == true ) && 
              (This->m_settings.fskPacketHandler.SyncWordDetected == false) )
            {
              This->m_settings.fskPacketHandler.SyncWordDetected = true;

              This->m_settings.fskPacketHandler.RssiValue = 
                -(This->readReg(FSK_RegRssiValue) >> 1 );

              This->m_settings.fskPacketHandler.AfcValue = 
                (int32_t)(double)( ((uint16_t)This->readReg(FSK_RegAfcMsb) << 8 ) |
                                   (uint16_t)This->readReg(FSK_RegAfcLsb) ) *
                FXOSC_STEP;
              This->m_settings.fskPacketHandler.RxGain = 
                (This->readReg(COM_RegLna) >> 5) & 0x07;
            }

          break;

        case MODEM_LORA:
          if (This->m_settings.loraSettings.FreqHopOn == true)
            {
              // Clear Irq
              This->writeReg(LOR_RegIrqFlags, LOR_IRQFLAG_FhssChangeChannel);

              // Fhss radio event (unsupported currently)
              // FhssChangeChannel( (readReg( LOR_RegHopChannel) & 
              // ~_HOPCHANNEL_FhssPresentChannel_MASK) );
              //cerr << __FUNCTION__ << ": Fhss Change Channel (LORA, RX running)" << endl;
            }
          break;

        default:
          break;
        }
      break;

    case STATE_TX_RUNNING:
      switch (This->m_settings.modem)
        {
        case MODEM_FSK:
          break;
        case MODEM_LORA:
          if (This->m_settings.loraSettings.FreqHopOn == true)
            {
              // Clear Irq
              This->writeReg(LOR_RegIrqFlags, LOR_IRQFLAG_FhssChangeChannel);
                    
              // Fhss radio event (unsupported currently)
              // FhssChangeChannel( (readReg( LOR_RegHopChannel) & 
              // ~_HOPCHANNEL_FhssPresentChannel_MASK) );
              //cerr << __FUNCTION__ << ": Fhss Change Channel (LORA, TX running)" << endl;
            }
          break;

        default:
          break;
        }
      break;      

    default:
      break;
    }

  This->unlockIntrs();
}

void SX1276::onDio3Irq(void *ctx)
{
  upm::SX1276 *This = (upm::SX1276 *)ctx;

  This->lockIntrs();
  //  cerr << __FUNCTION__ << ": Enter" << endl;
  switch (This->m_settings.modem)
    {
    case MODEM_FSK:
      break;

    case MODEM_LORA:
      if (This->readReg(LOR_RegIrqFlags) & LOR_IRQFLAG_CadDetected)
        {
          // Clear Irq
          This->writeReg(LOR_RegIrqFlags, 
                         (LOR_IRQFLAG_CadDetected | LOR_IRQFLAG_CadDone));

          // CADDetected radio event (true)
          // cerr << __FUNCTION__ << ": CadDetected (LORA)" << endl;

        }
      else
        {
          // Clear Irq
          This->writeReg(LOR_RegIrqFlags, LOR_IRQFLAG_CadDone);
          // CADDetected radio event (false)
          //cerr << __FUNCTION__ << ": CadDone (LORA)" << endl;
        }

      break;

    default:
      break;
    }

  This->unlockIntrs();
}

void SX1276::onDio4Irq(void *ctx)
{
  upm::SX1276 *This = (upm::SX1276 *)ctx;

  This->lockIntrs();
  //  cerr << __FUNCTION__ << ": Enter" << endl;
  switch (This->m_settings.modem)
    {
    case MODEM_FSK:
      {
        if (This->m_settings.fskPacketHandler.PreambleDetected == false)
          {
            This->m_settings.fskPacketHandler.PreambleDetected = true;
          }
      }

      break;

    case MODEM_LORA:
      break;

    default:
      break;
    }

  This->unlockIntrs();
}

void SX1276::onDio5Irq(void *ctx)
{
  upm::SX1276 *This = (upm::SX1276 *)ctx;

  This->lockIntrs();
  //  cerr << __FUNCTION__ << ": Enter" << endl;
  switch (This->m_settings.modem)
    {
    case MODEM_FSK:
      break;
    case MODEM_LORA:
      //      fprintf(stderr, "%s: LORA IrqFlags = %02x\n", __FUNCTION__,
      //              This->readReg(LOR_RegIrqFlags));
      break;
    default:
      break;
    }

  This->unlockIntrs();
}

void SX1276::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t SX1276::getMillis()
{
  struct timeval elapsed, now;
  uint32_t elapse;

  // get current time
  gettimeofday(&now, NULL);

  // compute the delta since m_startTime
  if( (elapsed.tv_usec = now.tv_usec - m_startTime.tv_usec) < 0 ) 
    {
      elapsed.tv_usec += 1000000;
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec - 1;
    } 
  else 
    {
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec;
    }

  elapse = (uint32_t)((elapsed.tv_sec * 1000) + (elapsed.tv_usec / 1000));

  // never return 0
  if (elapse == 0)
    elapse = 1;

  return elapse;
}


