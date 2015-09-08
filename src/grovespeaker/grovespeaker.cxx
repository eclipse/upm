/*
 * Author: Zion Orent <sorent@ics.com>
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
#include <string>
#include <stdexcept>

#include "grovespeaker.h"

using namespace upm;

GroveSpeaker::GroveSpeaker(int pin)
{
  if ( !(m_gpio = mraa_gpio_init(pin)) )
    throw std::invalid_argument(std::string(__FUNCTION__) +
                                ": mraa_gpio_init() failed, invalid pin?");

    mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);
	m_note_list['a'] = storeNote(1136, 1073, 568, 536, 284, 268);
	m_note_list['b'] = storeNote(1012, 0, 506, 0, 253, 0);
	m_note_list['c'] = storeNote(1911, 1804, 956, 902, 478, 451);
	m_note_list['d'] = storeNote(1703, 1607, 851, 804, 426, 402);
	m_note_list['e'] = storeNote(1517, 0, 758, 0, 379, 0);
	m_note_list['f'] = storeNote(1432, 1351, 716, 676, 358, 338);
	m_note_list['g'] = storeNote(1276, 1204, 638, 602, 319, 301);
}

GroveSpeaker::~GroveSpeaker()
{
    mraa_gpio_close(m_gpio);
}

NoteData GroveSpeaker::storeNote(int noteDelayLow, int noteDelayLowSharp, 
                                 int noteDelayMed, int noteDelayMedSharp, 
                                 int noteDelayHigh, int noteDelayHighSharp)
{
	NoteData note;
	note.delayTimeLow = noteDelayLow;
	note.delayTimeLowSharp = noteDelayLowSharp;
	note.delayTimeMed = noteDelayMed;
	note.delayTimeMedSharp = noteDelayMedSharp;
	note.delayTimeHigh = noteDelayHigh;
	note.delayTimeHighSharp = noteDelayHighSharp;
	return note;
}

void GroveSpeaker::playAll()
{
	playSound('c', false, "low");
	usleep(200000);
	playSound('d', false, "low");
	usleep(200000);
	playSound('e', false, "low");
	usleep(200000);
	playSound('f', false, "low");
	usleep(200000);
	playSound('g', false, "low");
	usleep(500000);
	playSound('a', false, "low");
	usleep(500000);
	playSound('b', false, "low");
	usleep(500000);
}

void GroveSpeaker::playSound(char letter, bool sharp, std::string vocalWeight)
{
	std::map<char, NoteData>::iterator it = m_note_list.find(letter);
	if(it == m_note_list.end())
	{
		std::cout << "The key " << letter << " doesn't exist." << std::endl;
		return;
	}
	NoteData nd = it->second;
	int delayTime;
	if (sharp)
	{
		if (vocalWeight.compare("low") == 0)
			delayTime = nd.delayTimeLowSharp;
		else if (vocalWeight.compare("med") == 0)
			delayTime = nd.delayTimeMedSharp;
		else if (vocalWeight.compare("high") == 0)
			delayTime = nd.delayTimeHighSharp;
		else
		{
			std::cout << "Correct voice weight values are low, med, or high" 
			          << std::endl;
			return;
		}
	}
	else
	{
		if (vocalWeight.compare("low") == 0)
			delayTime = nd.delayTimeLow;
		else if (vocalWeight.compare("med") == 0)
			delayTime = nd.delayTimeMed;
		else if (vocalWeight.compare("high") == 0)
			delayTime = nd.delayTimeHigh;
		else
		{
			std::cout << "Correct voice weight values are low, med, or high"
			          << std::endl;
			return;
		}
	}
	// If delayTime is zero, that means you tried to choose a sharp note 
	// for a note that has no sharp
	if (sharp && !delayTime)
	{
		std::cout << "The key " << letter << " doesn't have a sharp note."
		          << std::endl;
		return;
	}
	sound(delayTime);
}

void GroveSpeaker::sound(int note_delay)
{
	mraa_result_t error = MRAA_SUCCESS;
	for (int i = 0; i < 100; i++)   
 	{
		error = mraa_gpio_write (m_gpio, HIGH);
		usleep(note_delay);
		error = mraa_gpio_write (m_gpio, LOW);
		usleep(note_delay);
	}
	if (error != MRAA_SUCCESS)
		mraa_result_print(error);
}

