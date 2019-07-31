/*
 * Author: Kiveisha Yevgeniy
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * Author: Mike McCauley
 * Copyright (c) 2011 Mike McCauley
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstring>
#include <cmath>
#include <sys/time.h>
#include "rf22.hpp"

using namespace upm;

// These are indexed by the values of ModemConfigChoice
// Canned modem configurations generated with 
// http://www.hoperf.com/upload/rf/RF22B%2023B%2031B%2042B%2043B%20Register%20Settings_RevB1-v5.xls
// Stored in flash (program) memory to save SRAM
static const RF22::ModemConfig MODEM_CONFIG_TABLE[] =
{
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x00, 0x08 }, // Unmodulated carrier
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x33, 0x08 }, // FSK, PN9 random modulation, 2, 5

    // All the following enable FIFO with reg 71
    //  1c,   1f,   20,   21,   22,   23,   24,   25,   2c,   2d,   2e,   58,   69,   6e,   6f,   70,   71,   72
    // FSK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x22, 0x08 }, // 2, 5
    { 0x1b, 0x03, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x22, 0x3a }, // 2.4, 36
    { 0x1d, 0x03, 0xa1, 0x20, 0x4e, 0xa5, 0x00, 0x13, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x22, 0x48 }, // 4.8, 45
    { 0x1e, 0x03, 0xd0, 0x00, 0x9d, 0x49, 0x00, 0x45, 0x40, 0x0a, 0x20, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x22, 0x48 }, // 9.6, 45
    { 0x2b, 0x03, 0x34, 0x02, 0x75, 0x25, 0x07, 0xff, 0x40, 0x0a, 0x1b, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x22, 0x0f }, // 19.2, 9.6
    { 0x02, 0x03, 0x68, 0x01, 0x3a, 0x93, 0x04, 0xd5, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x22, 0x1f }, // 38.4, 19.6
    { 0x06, 0x03, 0x45, 0x01, 0xd7, 0xdc, 0x07, 0x6e, 0x40, 0x0a, 0x2d, 0x80, 0x60, 0x0e, 0xbf, 0x0c, 0x22, 0x2e }, // 57.6. 28.8
    { 0x8a, 0x03, 0x60, 0x01, 0x55, 0x55, 0x02, 0xad, 0x40, 0x0a, 0x50, 0x80, 0x60, 0x20, 0x00, 0x0c, 0x22, 0xc8 }, // 125, 125

    // GFSK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
    // These differ from FSK only in register 71, for the modulation type
    { 0x2b, 0x03, 0xf4, 0x20, 0x41, 0x89, 0x00, 0x36, 0x40, 0x0a, 0x1d, 0x80, 0x60, 0x10, 0x62, 0x2c, 0x23, 0x08 }, // 2, 5
    { 0x1b, 0x03, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x23, 0x3a }, // 2.4, 36
    { 0x1d, 0x03, 0xa1, 0x20, 0x4e, 0xa5, 0x00, 0x13, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x23, 0x48 }, // 4.8, 45
    { 0x1e, 0x03, 0xd0, 0x00, 0x9d, 0x49, 0x00, 0x45, 0x40, 0x0a, 0x20, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x23, 0x48 }, // 9.6, 45
    { 0x2b, 0x03, 0x34, 0x02, 0x75, 0x25, 0x07, 0xff, 0x40, 0x0a, 0x1b, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x23, 0x0f }, // 19.2, 9.6
    { 0x02, 0x03, 0x68, 0x01, 0x3a, 0x93, 0x04, 0xd5, 0x40, 0x0a, 0x1e, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x23, 0x1f }, // 38.4, 19.6
    { 0x06, 0x03, 0x45, 0x01, 0xd7, 0xdc, 0x07, 0x6e, 0x40, 0x0a, 0x2d, 0x80, 0x60, 0x0e, 0xbf, 0x0c, 0x23, 0x2e }, // 57.6. 28.8
    { 0x8a, 0x03, 0x60, 0x01, 0x55, 0x55, 0x02, 0xad, 0x40, 0x0a, 0x50, 0x80, 0x60, 0x20, 0x00, 0x0c, 0x23, 0xc8 }, // 125, 125

    // OOK, No Manchester, Max Rb err <1%, Xtal Tol 20ppm
    { 0x51, 0x03, 0x68, 0x00, 0x3a, 0x93, 0x01, 0x3d, 0x2c, 0x11, 0x28, 0x80, 0x60, 0x09, 0xd5, 0x2c, 0x21, 0x08 }, // 1.2, 75
    { 0xc8, 0x03, 0x39, 0x20, 0x68, 0xdc, 0x00, 0x6b, 0x2a, 0x08, 0x2a, 0x80, 0x60, 0x13, 0xa9, 0x2c, 0x21, 0x08 }, // 2.4, 335
    { 0xc8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x29, 0x04, 0x29, 0x80, 0x60, 0x27, 0x52, 0x2c, 0x21, 0x08 }, // 4.8, 335
    { 0xb8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x82, 0x29, 0x80, 0x60, 0x4e, 0xa5, 0x2c, 0x21, 0x08 }, // 9.6, 335
    { 0xa8, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x41, 0x29, 0x80, 0x60, 0x9d, 0x49, 0x2c, 0x21, 0x08 }, // 19.2, 335
    { 0x98, 0x03, 0x9c, 0x00, 0xd1, 0xb7, 0x00, 0xd4, 0x28, 0x20, 0x29, 0x80, 0x60, 0x09, 0xd5, 0x0c, 0x21, 0x08 }, // 38.4, 335
    { 0x98, 0x03, 0x96, 0x00, 0xda, 0x74, 0x00, 0xdc, 0x28, 0x1f, 0x29, 0x80, 0x60, 0x0a, 0x3d, 0x0c, 0x21, 0x08 }, // 40, 335

};

RF22::RF22(int spiBus, int slaveSelectPin, int interruptPin)
{
    _idleMode = RF22_XTON; // Default idle state is READY mode
    _mode = RF22_MODE_IDLE; // We start up in idle mode
    _rxGood = 0;
    _rxBad = 0;
    _txGood = 0;
    
    //Initialize the SPI bus and pins, MRAA will log any failures here
    // start the SPI library:
    // Note the RF22 wants mode 0, MSB first and default to 1 Mbps
    _spi = mraa_spi_init(spiBus);
    mraa_spi_mode (_spi, MRAA_SPI_MODE0);
    mraa_spi_lsbmode(_spi, 0);
    mraa_spi_frequency(_spi, 1000000); // 1Mhz

    _cs = mraa_gpio_init(slaveSelectPin);
    mraa_gpio_dir(_cs, MRAA_GPIO_OUT);    
    
    _irq = mraa_gpio_init(interruptPin);
    mraa_gpio_dir(_irq, MRAA_GPIO_IN);
    mraa_gpio_isr(_irq, MRAA_GPIO_EDGE_FALLING, &isr, (void*)this);
}

RF22::~RF22()
{
    mraa_spi_stop(_spi);
    mraa_gpio_close(_cs);
    mraa_gpio_close(_irq);
}

uint8_t RF22::init()
{
    // Wait for RF22 POR (up to 16msec)
    usleep (16);

    // Initialise the slave select pin    
    mraa_gpio_write(_cs, 0x1);

    usleep (100);
    
    // Software reset the device
    reset();

    // Get the device type and check it
    // This also tests whether we are really connected to a device
    _deviceType = spiRead(RF22_REG_00_DEVICE_TYPE);
    if (   _deviceType != RF22_DEVICE_TYPE_RX_TRX
        && _deviceType != RF22_DEVICE_TYPE_TX)
    return 0;

    clearTxBuf();
    clearRxBuf();
  
    // Most of these are the POR default
    spiWrite(RF22_REG_7D_TX_FIFO_CONTROL2, RF22_TXFFAEM_THRESHOLD);
    spiWrite(RF22_REG_7E_RX_FIFO_CONTROL,  RF22_RXFFAFULL_THRESHOLD);
    spiWrite(RF22_REG_30_DATA_ACCESS_CONTROL, RF22_ENPACRX | RF22_ENPACTX | RF22_ENCRC | RF22_CRC_CRC_16_IBM);
    // Configure the message headers
    // Here we set up the standard packet format for use by the RF22 library
    // 8 nibbles preamble
    // 2 SYNC words 2d, d4
    // Header length 4 (to, from, id, flags)
    // 1 octet of data length (0 to 255)
    // 0 to 255 octets data
    // 2 CRC octets as CRC16(IBM), computed on the header, length and data
    // On reception the to address is check for validity against RF22_REG_3F_CHECK_HEADER3
    // or the broadcast address of 0xff
    // If no changes are made after this, the transmitted
    // to address will be 0xff, the from address will be 0xff
    // and all such messages will be accepted. This permits the out-of the box
    // RF22 config to act as an unaddresed, unreliable datagram service
    spiWrite(RF22_REG_32_HEADER_CONTROL1, RF22_BCEN_HEADER3 | RF22_HDCH_HEADER3);
    spiWrite(RF22_REG_33_HEADER_CONTROL2, RF22_HDLEN_4 | RF22_SYNCLEN_2);
    setPreambleLength(8);
    uint8_t syncwords[] = { 0x2d, 0xd4 };
    setSyncWords(syncwords, sizeof(syncwords));
    setPromiscuous(0); 
    // Check the TO header against RF22_DEFAULT_NODE_ADDRESS
    spiWrite(RF22_REG_3F_CHECK_HEADER3, RF22_DEFAULT_NODE_ADDRESS);
    // Set the default transmit header values
    setHeaderTo(RF22_DEFAULT_NODE_ADDRESS);
    setHeaderFrom(RF22_DEFAULT_NODE_ADDRESS);
    setHeaderId(0);
    setHeaderFlags(0);

    // Ensure the antenna can be switched automatically according to transmit and receive
    // This assumes GPIO0(out) is connected to TX_ANT(in) to enable tx antenna during transmit
    // This assumes GPIO1(out) is connected to RX_ANT(in) to enable rx antenna during receive
    spiWrite (RF22_REG_0B_GPIO_CONFIGURATION0, 0x12) ; // TX state
    spiWrite (RF22_REG_0C_GPIO_CONFIGURATION1, 0x15) ; // RX state

    // Enable interrupts
    spiWrite(RF22_REG_05_INTERRUPT_ENABLE1, RF22_ENTXFFAEM | RF22_ENRXFFAFULL | RF22_ENPKSENT | RF22_ENPKVALID | RF22_ENCRCERROR | RF22_ENFFERR);
    spiWrite(RF22_REG_06_INTERRUPT_ENABLE2, RF22_ENPREAVAL);

    // Set some defaults. An innocuous ISM frequency, and reasonable pull-in
    setFrequency(434.0, 0.05);
//    setFrequency(900.0);
    // Some slow, reliable default speed and modulation
    setModemConfig(FSK_Rb2_4Fd36);
//    setModemConfig(FSK_Rb125Fd125);
    // Minimum power
    setTxPower(RF22_TXPOW_8DBM);
//    setTxPower(RF22_TXPOW_17DBM);

    return 1;
}

// C++ level interrupt handler for this instance
void RF22::handleInterrupt()
{
    uint8_t _lastInterruptFlags[2] = {};
    // Read the interrupt flags which clears the interrupt
    spiBurstRead(RF22_REG_03_INTERRUPT_STATUS1, _lastInterruptFlags, 2);

    if (_lastInterruptFlags[0] & RF22_IFFERROR)
    { 
        resetFifos(); // Clears the interrupt
        if (_mode == RF22_MODE_TX)
            restartTransmit();
        else if (_mode == RF22_MODE_RX)
            clearRxBuf();
    }
    // Caution, any delay here may cause a FF underflow or overflow
    if (_lastInterruptFlags[0] & RF22_ITXFFAEM)
    {
        // See if more data has to be loaded into the Tx FIFO 
        sendNextFragment();
    }
    if (_lastInterruptFlags[0] & RF22_IRXFFAFULL)
    {
        // Caution, any delay here may cause a FF overflow
        // Read some data from the Rx FIFO
        readNextFragment(); 
    }
    if (_lastInterruptFlags[0] & RF22_IEXT)
    {
        // This is not enabled by the base code, but users may want to enable it
        handleExternalInterrupt();
    }
    if (_lastInterruptFlags[1] & RF22_IWUT)
    {
        // This is not enabled by the base code, but users may want to enable it
        handleWakeupTimerInterrupt();
    }
    if (_lastInterruptFlags[0] & RF22_IPKSENT)
    {
        _txGood++; 
        // Transmission does not automatically clear the tx buffer.
        // Could retransmit if we wanted
        // RF22 transitions automatically to Idle
        _mode = RF22_MODE_IDLE;
    }
    if (_lastInterruptFlags[0] & RF22_IPKVALID)
    {
        uint8_t len = spiRead(RF22_REG_4B_RECEIVED_PACKET_LENGTH);

        // May have already read one or more fragments
        // Get any remaining unread octets, based on the expected length
        // First make sure we dont overflow the buffer in the case of a stupid length
        // or partial bad receives
        if (   len >  RF22_MAX_MESSAGE_LEN
            || len < _bufLen)
        {
            _rxBad++;
            _mode = RF22_MODE_IDLE;
            clearRxBuf();
            return; // Hmmm receiver buffer overflow. 
        }

        spiBurstRead(RF22_REG_7F_FIFO_ACCESS, _buf + _bufLen, len - _bufLen);
        _rxGood++;
        _bufLen = len;
        _mode = RF22_MODE_IDLE;
        _rxBufValid = true;
    }
    if (_lastInterruptFlags[0] & RF22_ICRCERROR)
    {
        _rxBad++;
        clearRxBuf();
        resetRxFifo();
        _mode = RF22_MODE_IDLE;
        setModeRx(); // Keep trying
    }
    if (_lastInterruptFlags[1] & RF22_IPREAVAL)
    {
        _lastRssi = spiRead(RF22_REG_26_RSSI);
        clearRxBuf();
    }
}

void RF22::isr(void* args)
{
    RF22* This = (RF22*)(args);
    This->handleInterrupt();
}

void RF22::reset()
{
    spiWrite(RF22_REG_07_OPERATING_MODE1, RF22_SWRES);
    // Wait for it to settle
    usleep(100); // SWReset time is nominally 100usec
}

uint8_t RF22::spiRead(uint8_t reg)
{
    uint8_t data = 0;
    spiBurstRead (reg, &data, 1);
    return data;
}

void RF22::spiWrite(uint8_t reg, uint8_t val)
{
    spiBurstWrite (reg, &val, 1);
}

void RF22::spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len)
{
    uint8_t *request = new uint8_t[len + 1]();
    uint8_t *response = new uint8_t[len + 1]();

    request[0] = reg & ~RF22_SPI_WRITE_MASK;
    memcpy (&request[1], dest, len);

    mraa_gpio_write(_cs, 0x1);
    mraa_gpio_write(_cs, 0x0);
    usleep(100);
    mraa_spi_transfer_buf(_spi, request, response, len + 1);
    usleep(100);
    mraa_gpio_write(_cs, 0x1);

    memcpy (dest, &response[1], len);

    delete[] request;
    delete[] response;
}

void RF22::spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len)
{
    uint8_t *request = new uint8_t[len + 1]();
    uint8_t *response = new uint8_t[len + 1]();

    request[0] = reg | RF22_SPI_WRITE_MASK;
    memcpy (&request[1], src, len);

    mraa_gpio_write(_cs, 0x1);
    mraa_gpio_write(_cs, 0x0);
    usleep(100);
    mraa_spi_transfer_buf(_spi, request, response, len + 1);
    usleep(100);
    mraa_gpio_write(_cs, 0x1);

    delete[] request;
    delete[] response;
}

uint8_t RF22::statusRead()
{
    return spiRead(RF22_REG_02_DEVICE_STATUS);
}

uint8_t RF22::adcRead(uint8_t adcsel,
                      uint8_t adcref ,
                      uint8_t adcgain, 
                      uint8_t adcoffs)
{
    uint8_t configuration = adcsel | adcref | (adcgain & RF22_ADCGAIN);
    spiWrite(RF22_REG_0F_ADC_CONFIGURATION, configuration | RF22_ADCSTART);
    spiWrite(RF22_REG_10_ADC_SENSOR_AMP_OFFSET, adcoffs);

    // Conversion time is nominally 305usec
    // Wait for the DONE bit
    while (!(spiRead(RF22_REG_0F_ADC_CONFIGURATION) & RF22_ADCDONE))
    ;
    // Return the value  
    return spiRead(RF22_REG_11_ADC_VALUE);
}

uint8_t RF22::temperatureRead(uint8_t tsrange, uint8_t tvoffs)
{
    spiWrite(RF22_REG_12_TEMPERATURE_SENSOR_CALIBRATION, tsrange | RF22_ENTSOFFS);
    spiWrite(RF22_REG_13_TEMPERATURE_VALUE_OFFSET, tvoffs);
    return adcRead(RF22_ADCSEL_INTERNAL_TEMPERATURE_SENSOR | RF22_ADCREF_BANDGAP_VOLTAGE); 
}

uint16_t RF22::wutRead()
{
    uint8_t buf[2] = {};
    spiBurstRead(RF22_REG_17_WAKEUP_TIMER_VALUE1, buf, 2);
    return ((uint16_t)buf[0] << 8) | buf[1]; // Dont rely on byte order
}

// RFM-22 doc appears to be wrong: WUT for wtm = 10000, r, = 0, d = 0 is about 1 sec
void RF22::setWutPeriod(uint16_t wtm, uint8_t wtr, uint8_t wtd)
{
    uint8_t period[3];

    period[0] = ((wtr & 0xf) << 2) | (wtd & 0x3);
    period[1] = wtm >> 8;
    period[2] = wtm & 0xff;
    spiBurstWrite(RF22_REG_14_WAKEUP_TIMER_PERIOD1, period, sizeof(period));
}

// Returns true if center + (fhch * fhs) is within limits
// Caution, different versions of the RF22 support different max freq
// so YMMV
uint8_t RF22::setFrequency(float center, float afcPullInRange)
{
    uint8_t fbsel = RF22_SBSEL;
    uint8_t afclimiter;
    if (center < 240.0 || center > 960.0) // 930.0 for early silicon
    return false;
    if (center >= 480.0)
    {
        if (afcPullInRange < 0.0 || afcPullInRange > 0.318750)
            return false;
        center /= 2;
        fbsel |= RF22_HBSEL;
        afclimiter = afcPullInRange * 1000000.0 / 1250.0;
    }
    else
    {
        if (afcPullInRange < 0.0 || afcPullInRange > 0.159375)
            return false;
        afclimiter = afcPullInRange * 1000000.0 / 625.0;
    }
    center /= 10.0;
    float integerPart = floor(center);
    float fractionalPart = center - integerPart;

    uint8_t fb = (uint8_t)integerPart - 24; // Range 0 to 23
    fbsel |= fb;
    uint16_t fc = fractionalPart * 64000;
    spiWrite(RF22_REG_73_FREQUENCY_OFFSET1, 0);  // REVISIT
    spiWrite(RF22_REG_74_FREQUENCY_OFFSET2, 0);
    spiWrite(RF22_REG_75_FREQUENCY_BAND_SELECT, fbsel);
    spiWrite(RF22_REG_76_NOMINAL_CARRIER_FREQUENCY1, fc >> 8);
    spiWrite(RF22_REG_77_NOMINAL_CARRIER_FREQUENCY0, fc & 0xff);
    spiWrite(RF22_REG_2A_AFC_LIMITER, afclimiter);
    return !(statusRead() & RF22_FREQERR);
}

// Step size in 10kHz increments
// Returns true if centre + (fhch * fhs) is within limits
uint8_t RF22::setFHStepSize(uint8_t fhs)
{
    spiWrite(RF22_REG_7A_FREQUENCY_HOPPING_STEP_SIZE, fhs);
    return !(statusRead() & RF22_FREQERR);
}

// Adds fhch * fhs to centre frequency
// Returns true if centre + (fhch * fhs) is within limits
uint8_t RF22::setFHChannel(uint8_t fhch)
{
    spiWrite(RF22_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT, fhch);
    return !(statusRead() & RF22_FREQERR);
}

uint8_t RF22::rssiRead()
{
    return spiRead(RF22_REG_26_RSSI);
}

uint8_t RF22::ezmacStatusRead()
{
    return spiRead(RF22_REG_31_EZMAC_STATUS);
}

void RF22::setMode(uint8_t mode)
{
    spiWrite(RF22_REG_07_OPERATING_MODE1, mode);
}

void RF22::setModeIdle()
{
    if (_mode != RF22_MODE_IDLE)
    {
        setMode(_idleMode);
        _mode = RF22_MODE_IDLE;
    }
}

void RF22::setModeRx()
{
    if (_mode != RF22_MODE_RX)
    {
        setMode(_idleMode | RF22_RXON);
        _mode = RF22_MODE_RX;
    }
}

void RF22::setModeTx()
{
    if (_mode != RF22_MODE_TX)
    {
        setMode(_idleMode | RF22_TXON);
        _mode = RF22_MODE_TX;
        // Hmmm, if you dont clear the RX FIFO here, then it appears that going
        // to transmit mode in the middle of a receive can corrupt the
        // RX FIFO
        resetRxFifo();
        clearRxBuf();
    }
}

uint8_t  RF22::mode()
{
    return _mode;
}

void RF22::setTxPower(uint8_t power)
{
    spiWrite(RF22_REG_6D_TX_POWER, power);
}

// Sets registers from a canned modem configuration structure
void RF22::setModemRegisters(const ModemConfig* config)
{
    spiWrite(RF22_REG_1C_IF_FILTER_BANDWIDTH,                    config->reg_1c);
    spiWrite(RF22_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE,      config->reg_1f);
    spiBurstWrite(RF22_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE, &config->reg_20, 6);
    spiBurstWrite(RF22_REG_2C_OOK_COUNTER_VALUE_1,              &config->reg_2c, 3);
    spiWrite(RF22_REG_58_CHARGE_PUMP_CURRENT_TRIMMING,           config->reg_58);
    spiWrite(RF22_REG_69_AGC_OVERRIDE1,                          config->reg_69);
    spiBurstWrite(RF22_REG_6E_TX_DATA_RATE1,                    &config->reg_6e, 5);
}

// Set one of the canned FSK Modem configs
// Returns true if its a valid choice
uint8_t RF22::setModemConfig(ModemConfigChoice index)
{
    if (index > (sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
        return false;

    RF22::ModemConfig cfg;
    // memcpy_P(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(RF22::ModemConfig)); // !!!!!!!!!!!!!!!!!!! MIGHT CAUSE ISSUES
    memcpy(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(RF22::ModemConfig));
    setModemRegisters(&cfg);

    return true;
}

// REVISIT: top bit is in Header Control 2 0x33
void RF22::setPreambleLength(uint8_t nibbles)
{
    spiWrite(RF22_REG_34_PREAMBLE_LENGTH, nibbles);
}

// Caution doesnt set sync word len in Header Control 2 0x33
void RF22::setSyncWords(const uint8_t* syncWords, uint8_t len)
{
    spiBurstWrite(RF22_REG_36_SYNC_WORD3, syncWords, len);
}

void RF22::clearRxBuf()
{
    _bufLen = 0;
    _rxBufValid = false;
}

uint8_t RF22::available()
{
    if (!_rxBufValid)
    setModeRx(); // Make sure we are receiving
    return _rxBufValid;
}

// Blocks until a valid message is received
void RF22::waitAvailable()
{
    while (!available())
    ;
}

// Blocks until a valid message is received or timeout expires
// Return true if there is a message available
bool RF22::waitAvailableTimeout(unsigned long timeout)
{
    unsigned long endtime = getTimestamp() + timeout;
    unsigned long currenttime = getTimestamp();
    while (currenttime < endtime) {
        currenttime = getTimestamp();
        if (available()) {
            return true;
        }
    }
        
    return false;
}

void RF22::waitPacketSent()
{
    while (_mode == RF22_MODE_TX)
    ; // Wait for any previous transmit to finish
}

// Diagnostic help
void RF22::printBuffer(const char* prompt, const uint8_t* buf, uint8_t len)
{
}

uint8_t RF22::recv(uint8_t* buf, uint8_t* len)
{
    if (!available())
        return false;

    if (*len > _bufLen)
        *len = _bufLen;
    memcpy(buf, _buf, *len);
    clearRxBuf();
    return true;
}

void RF22::clearTxBuf()
{
    _bufLen = 0;
    _txBufSentIndex = 0;
}

void RF22::startTransmit()
{
    sendNextFragment(); // Actually the first fragment
    spiWrite(RF22_REG_3E_PACKET_LENGTH, _bufLen); // Total length that will be sent
    setModeTx(); // Start the transmitter, turns off the receiver
}

// Restart the transmission of a packet that had a problem
void RF22::restartTransmit()
{
    _mode = RF22_MODE_IDLE;
    _txBufSentIndex = 0;
    startTransmit();
}

uint8_t RF22::send(const uint8_t* data, uint8_t len)
{
    waitPacketSent();

    if (!fillTxBuf(data, len))
        return false;
    startTransmit();

    return true;
}

uint8_t RF22::fillTxBuf(const uint8_t* data, uint8_t len)
{
    clearTxBuf();
    if (!len)
    return false; 
    return appendTxBuf(data, len);
}

uint8_t RF22::appendTxBuf(const uint8_t* data, uint8_t len)
{
    if (((uint16_t)_bufLen + len) > RF22_MAX_MESSAGE_LEN)
    return false;

    memcpy(_buf + _bufLen, data, len);
    _bufLen += len;

    return true;
}

// Assumption: there is currently <= RF22_TXFFAEM_THRESHOLD bytes in the Tx FIFO
void RF22::sendNextFragment()
{
    if (_txBufSentIndex < _bufLen)
    {
    // Some left to send?
    uint8_t len = _bufLen - _txBufSentIndex;
    // But dont send too much
    if (len > (RF22_FIFO_SIZE - RF22_TXFFAEM_THRESHOLD - 1))
        len = (RF22_FIFO_SIZE - RF22_TXFFAEM_THRESHOLD - 1);
    spiBurstWrite(RF22_REG_7F_FIFO_ACCESS, _buf + _txBufSentIndex, len);
    _txBufSentIndex += len;
    }
}

// Assumption: there are at least RF22_RXFFAFULL_THRESHOLD in the RX FIFO
// That means it should only be called after a RXFFAFULL interrupt
void RF22::readNextFragment()
{
    if (((uint16_t)_bufLen + RF22_RXFFAFULL_THRESHOLD) > RF22_MAX_MESSAGE_LEN)
    return; // Hmmm receiver overflow. Should never occur

    // Read the RF22_RXFFAFULL_THRESHOLD octets that should be there
    spiBurstRead(RF22_REG_7F_FIFO_ACCESS, _buf + _bufLen, RF22_RXFFAFULL_THRESHOLD);
    _bufLen += RF22_RXFFAFULL_THRESHOLD;
}

// Clear the FIFOs
void RF22::resetFifos()
{
    spiWrite(RF22_REG_08_OPERATING_MODE2, RF22_FFCLRRX | RF22_FFCLRTX);
    spiWrite(RF22_REG_08_OPERATING_MODE2, 0);
}

// Clear the Rx FIFO
void RF22::resetRxFifo()
{
    spiWrite(RF22_REG_08_OPERATING_MODE2, RF22_FFCLRRX);
    spiWrite(RF22_REG_08_OPERATING_MODE2, 0);
}

// CLear the TX FIFO
void RF22::resetTxFifo()
{
    spiWrite(RF22_REG_08_OPERATING_MODE2, RF22_FFCLRTX);
    spiWrite(RF22_REG_08_OPERATING_MODE2, 0);
}

// Default implmentation does nothing. Override if you wish
void RF22::handleExternalInterrupt()
{
}

// Default implmentation does nothing. Override if you wish
void RF22::handleWakeupTimerInterrupt()
{
}

void RF22::setHeaderTo(uint8_t to)
{
    spiWrite(RF22_REG_3A_TRANSMIT_HEADER3, to);
}

void RF22::setHeaderFrom(uint8_t from)
{
    spiWrite(RF22_REG_3B_TRANSMIT_HEADER2, from);
}

void RF22::setHeaderId(uint8_t id)
{
    spiWrite(RF22_REG_3C_TRANSMIT_HEADER1, id);
}

void RF22::setHeaderFlags(uint8_t flags)
{
    spiWrite(RF22_REG_3D_TRANSMIT_HEADER0, flags);
}

uint8_t RF22::headerTo()
{
    return spiRead(RF22_REG_47_RECEIVED_HEADER3);
}

uint8_t RF22::headerFrom()
{
    return spiRead(RF22_REG_48_RECEIVED_HEADER2);
}

uint8_t RF22::headerId()
{
    return spiRead(RF22_REG_49_RECEIVED_HEADER1);
}

uint8_t RF22::headerFlags()
{
    return spiRead(RF22_REG_4A_RECEIVED_HEADER0);
}

uint8_t RF22::lastRssi()
{
    return _lastRssi;
}

void RF22::setPromiscuous(uint8_t promiscuous)
{
    spiWrite(RF22_REG_43_HEADER_ENABLE3, promiscuous ? 0x00 : 0xff);
}

uint64_t 
RF22::getTimestamp () {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(1000000 * tv.tv_sec + tv.tv_usec);
}
