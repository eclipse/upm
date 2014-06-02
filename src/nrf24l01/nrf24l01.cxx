/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <iostream>
#include <unistd.h>

#include "nrf24l01.h"

using namespace upm;

NRF24l01::NRF24l01 (uint8_t cs) {
	maa_init();
	nrfInitModule (cs, 8);
}

NRF24l01::~NRF24l01 () {
	maa_result_t error = MAA_SUCCESS;
	error = maa_spi_stop(m_spi);
	if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
	error = maa_gpio_close (m_cePinCtx);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
	error = maa_gpio_close (m_csnPinCtx);
    if (error != MAA_SUCCESS) {
        maa_result_print(error);
    }
}

void 
NRF24l01::nrfInitModule (uint8_t chip_select, uint8_t chip_enable) {
	maa_result_t error = MAA_SUCCESS;

	m_csn 		= chip_select;
	m_ce 		= chip_enable;
	m_channel 	= 1;

	m_csnPinCtx = maa_gpio_init (m_csn);
	if (m_csnPinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", m_csn);
		exit (1);
	}
	
	m_cePinCtx = maa_gpio_init (m_ce);
	if (m_cePinCtx == NULL) {
        fprintf (stderr, "Are you sure that pin%d you requested is valid on your platform?", m_ce);
		exit (1);
	}
	
	error = maa_gpio_dir (m_csnPinCtx, MAA_GPIO_OUT);
    if (error != MAA_SUCCESS) {
        maa_result_print (error);
    }

	error = maa_gpio_dir (m_cePinCtx, MAA_GPIO_OUT);
    if (error != MAA_SUCCESS) {
        maa_result_print (error);
    }
	
	nrfCELow ();
	m_spi = maa_spi_init (0);
}

void 
NRF24l01::nrfConfigModule() {
	/* Set RF channel */
	nrfConfigRegister (RF_CH, m_channel);

	/* Set length of incoming payload */
	nrfConfigRegister (RX_PW_P0, m_payload);
	nrfConfigRegister (RX_PW_P1, m_payload);
	/* Set length of incoming payload for broadcast */
	nrfConfigRegister (RX_PW_P2, m_payload);
	
	/* Start receiver */
	nrfPowerUpRX ();
	nrfFlushRX ();
}

/* Clocks only one byte into the given MiRF register */
void 
NRF24l01::nrfConfigRegister(uint8_t reg, uint8_t value) {
	nrfCSOn ();
	maa_spi_write (m_spi, W_REGISTER | (REGISTER_MASK & reg));
	maa_spi_write (m_spi, value);
	nrfCSOff ();
}

void 
NRF24l01::nrfPowerUpRX() {
	m_ptx = 0;
	nrfCELow();
	nrfConfigRegister(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (1<<PRIM_RX) ) );
	nrfCEHigh();
	nrfConfigRegister(STATUS,(1 << TX_DS) | (1 << MAX_RT)); 
}

void 
NRF24l01::nrfFlushRX() {
	nrfCSOn ();
	maa_spi_write (m_spi, FLUSH_RX);
	nrfCSOff ();
}

/* Sets the receiving address */
void 
NRF24l01::nrfSetRXaddr(uint8_t * addr) {
	nrfCELow();
	nrfWriteRegister(RX_ADDR_P1, addr, mirf_ADDR_LEN);
	nrfCEHigh();
}

/* Sets the transmitting address */
void 
NRF24l01::nrfSetTXaddr(uint8_t * addr)
{
	/* RX_ADDR_P0 must be set to the sending addr for auto ack to work. */
	nrfWriteRegister (RX_ADDR_P0, addr, mirf_ADDR_LEN);
	nrfWriteRegister (TX_ADDR, addr, mirf_ADDR_LEN);
}

/* The broadcast address should be 0xFFFFF */
void 
NRF24l01::nrfSetBroadcastAddr (uint8_t * addr) {
	nrfCELow ();
	nrfWriteRegister (RX_ADDR_P2, addr, mirf_ADDR_LEN);
	nrfCEHigh ();
}

void 
NRF24l01::nrfSetPayload (uint8_t load) {
	m_payload = load;
}

void 
NRF24l01::nrfWriteRegister(uint8_t reg, uint8_t * value, uint8_t len) 
{
	nrfCSOn ();
	maa_spi_write (m_spi, W_REGISTER | (REGISTER_MASK & reg));
	nrfTransmitSync(value, len);
	nrfCSOff ();
}

void 
NRF24l01::nrfTransmitSync(uint8_t *dataout, uint8_t len){
	uint8_t i;
	for(i = 0; i < len; i++) {
		maa_spi_write (m_spi, dataout[i]);
	}
}

/* Checks if data is available for reading */
bool 
NRF24l01::nrfDataReady() {
	uint8_t status = nrfGetStatus();
	if ( status & (1 << RX_DR) ) {
		return 1;
	}
	
	return !nrfRXFifoEmpty();
}

uint8_t 
NRF24l01::nrfGetStatus () {
	uint8_t rv;
	nrfReadRegister (STATUS, &rv, 1);
	return rv;
}

/* Reads an array of bytes from the given start position in the MiRF registers. */
void 
NRF24l01::nrfReadRegister (uint8_t reg, uint8_t * value, uint8_t len)
{
	nrfCSOn ();
	maa_spi_write (m_spi, R_REGISTER | (REGISTER_MASK & reg));
	nrfTransferSync (value, value, len);
	nrfCSOff ();
}

void 
NRF24l01::nrfTransferSync (uint8_t *dataout,uint8_t *datain,uint8_t len) {
	uint8_t i;
	for(i = 0;i < len;i++) {
		datain[i] = maa_spi_write (m_spi, dataout[i]);
	}
}

bool 
NRF24l01::nrfRXFifoEmpty () {
	uint8_t fifo_status;
	nrfReadRegister (FIFO_STATUS, &fifo_status, sizeof(fifo_status));
	return (fifo_status & (1 << RX_EMPTY));
}

/* Reads payload bytes into data array */
void 
NRF24l01::nrfGetData (uint8_t * data) 
{
	nrfCSOn ();
	/* Send cmd to read rx payload */
	maa_spi_write (m_spi, R_RX_PAYLOAD);
	/* Read payload */
	nrfTransferSync(data, data, m_payload);
	nrfCSOff ();
	nrfConfigRegister(STATUS, (1<<RX_DR));
}

/* Sends a data package to the default address. Be sure to send the correct
 * amount of bytes as configured as payload on the receiver. */
void 
NRF24l01::nrfSend(uint8_t * value) {
	uint8_t status;
	status = nrfGetStatus();

	while (m_ptx) {
		status = nrfGetStatus();

		if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
			m_ptx = 0;
			break;
		}
	} // Wait until last paket is send

	nrfCELow();
	nrfPowerUpTX();							// Set to transmitter mode , Power up
	nrfCSOn ();
	maa_spi_write (m_spi, FLUSH_TX);		// Write cmd to flush tx fifo
	nrfCSOff ();
	
	nrfCSOn ();
	maa_spi_write (m_spi, W_TX_PAYLOAD); 	// Write cmd to write payload
	nrfTransmitSync(value, m_payload);		// Write payload
	nrfCSOff ();
	nrfCEHigh();							// Start transmission
}

void 
NRF24l01::nrfSend () {
	nrfSend (m_txBuffer);
}

bool 
NRF24l01::nrfIsSending () {
	uint8_t status;
	if (m_ptx)	{ // Sending mode.
		status = nrfGetStatus();
		/* if sending successful (TX_DS) or max retries exceded (MAX_RT). */
		if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
			nrfPowerUpRX();
			return false; 
		}
		return true;
	}
	return false;
}

void 
NRF24l01::nrfPowerUpTX () {
	m_ptx = 1;
	nrfConfigRegister (CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (0<<PRIM_RX) ) );
}

void 
NRF24l01::nrfPowerDown () {
	nrfCELow ();
	nrfConfigRegister (CONFIG, mirf_CONFIG);
}

maa_result_t 
NRF24l01::nrfCEHigh () {
	return maa_gpio_write (m_cePinCtx, HIGH);
}

maa_result_t 
NRF24l01::nrfCELow () {
	return maa_gpio_write (m_cePinCtx, LOW);
}

maa_result_t 
NRF24l01::nrfCSOn () {
	return maa_gpio_write (m_csnPinCtx, LOW);
}

maa_result_t 
NRF24l01::nrfCSOff () {
	return maa_gpio_write (m_csnPinCtx, HIGH);
}

void 
NRF24l01::nrfListenForChannel() {
	if(!nrfIsSending() && nrfDataReady()) {
		nrfGetData(m_rxBuffer);
		dataRecievedHandler(); /* let know that data arrived */
	}
}

