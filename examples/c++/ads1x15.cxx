/*
 * Author: Marc Graham <marc@m2ag.net>
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

#include "mraa.hpp"

#include <iostream>
#include <unistd.h>
#include "ads1015.hpp"
#include "ads1115.hpp"



int main()
{

     using namespace std;
     using namespace upm;
     int command;
     //Select the device you are testing here and adjust case 6 for the correct sample rates.
     //upm::ADS1015 *ads = new upm::ADS1015(1);
     upm::ADS1115 *ads = new upm::ADS1115(1, 0x49);
     float inputVoltage;
     int ans;

     do
     {

          cout << endl;
          cout << "1 - get Conversion \t" ;
          cout << "2 - get last conversion" << endl;
          cout << "3 - get Gain \t\t";
          cout << "4 - set Gain"  << endl;;
          cout << "5 - get Data Rate \t";
          cout << "6 - set Data Rate" << endl;
          cout << "7 - Set Upper Threshold \t" ;
          cout << "8 - Set Lower Threshold \t";
          cout << "9 - Display Thresholds \t";
          cout << "10 - Set Default Thresholds \t";
          cout << "11 - Set conversion ready" << endl;
          cout << "12 - get Comp Que \t" ;
          cout << "13 - set Comp Que" << endl;
          cout << "14 - get Comp Pol \t";
          cout << "15 - set Comp Pol" << endl;
          cout << "16 - get Comp mode \t";
          cout << "17 - set Comp mode " << endl;
          cout << "18 - get Comp Latch\t";
          cout << "19 - set Comp Latch " << endl;
          cout << "20 - get Continuous \t";
          cout << "21 - set Continuous \t" << endl;
          cout << "-1 - exit" << endl;
          cout << "Enter a command: ";
          cin >> command;


          switch(command)
          {
          case 2:
               cout << ads->getLastSample() << endl;
               break;
          case 3:
               cout << std::hex << ads->getGain() << endl;
               break;
          case 5:
               cout << std::hex << ads->getSPS() << endl;
               break;
          case 4:
               int gain;
               ADS1015::ADSGAIN set_gain;
               cout << "select one of the following:" << endl;
               cout << "1 -> gain 2/3 \t 2 -> gain1 \t 3 -> gain 2" << endl;
               cout << "4 -> gain 4 \t 5 -> gain 8 \t 6 -> gain 15" << endl;
               cin >> gain;
               switch(gain){
               case 1:
                    set_gain = ADS1X15::GAIN_TWOTHIRDS;
                    break;
               case 2:
                    set_gain = ADS1X15::GAIN_ONE;
                    break;
               case 3:
                    set_gain = ADS1X15::GAIN_TWO;
                    break;
               case 4:
                    set_gain = ADS1X15::GAIN_FOUR;
                    break;
               case 5:
                    set_gain = ADS1X15::GAIN_EIGHT;
                    break;
               case 6:
                    set_gain = ADS1X15::GAIN_SIXTEEN;
                    break;
               default:
                    set_gain = ADS1X15::GAIN_ONE;
               }
               ads->setGain(set_gain);
               break;
          case 6:
               int rate;
               /*ADS1015::ADSDATARATE set_rate;
               cout << "select one of the following:" << endl;
               cout << "1 -> SPS_120 \t 2 -> SPS_250 \t 3 -> SPS_490 \t 4 -> SPS_920" << endl;
               cout << "5 -> SPS_1600 \t 6 -> SPS_2400 \t 7 -> SPS_3300" << endl;
               cin >> rate;
               switch(rate){
               case 1:
                    set_rate = ADS1015::SPS_128;
                    break;
               case 2:
                    set_rate = ADS1015::SPS_250;
                    break;
               case 3:
                    set_rate = ADS1015::SPS_490;
                    break;
               case 4:
                    set_rate = ADS1015::SPS_920;
                    break;
               case 5:
                    set_rate = ADS1015::SPS_1600;
                    break;
               case 6:
                    set_rate = ADS1015::SPS_2400;
                    break;
               case 7:
                    set_rate = ADS1015::SPS_3300;
                    break;
               default:
                    set_rate = ADS1015::SPS_1600;
               } */
               ADS1115::ADSDATARATE set_rate;
               cout << "select one of the following:" << endl;
               cout << "1 -> SPS_8 \t 2 -> SPS_16 \t 3 -> SPS_32 \t 4 -> SPS_64" << endl;
               cout << "5 -> SPS_128 \t 6 -> SPS_250 \t 7 -> SPS_475 \t 8-> SPS_860" << endl;
               cin >> rate;
               switch(rate){
               case 1:
                    set_rate = ADS1115::SPS_8;
                    break;
               case 2:
                    set_rate = ADS1115::SPS_16;
                    break;
               case 3:
                    set_rate = ADS1115::SPS_32;
                    break;
               case 4:
                    set_rate = ADS1115::SPS_64;
                    break;
               case 5:
                    set_rate = ADS1115::SPS_128;
                    break;
               case 6:
                    set_rate = ADS1115::SPS_250;
                    break;
               case 7:
                    set_rate = ADS1115::SPS_475;
                    break;
               case 8:
                    set_rate = ADS1115::SPS_860;
                    break;
               default:
                    set_rate = ADS1115::SPS_128;
               }

               ads->setSPS(set_rate);
               break;
          case 1:
               int mode;
               ADS1X15::ADSMUXMODE set_mode;
               cout << "select one of the following:" << endl;
               cout << "1 -> MUX_0_1 \t 2 -> MUX_0_3 \t 3 -> MUX_1_3 \t 4 -> MUX_2_3" << endl;
               cout << "5 -> SINGLE_0 \t 6 -> SINGLE_1 \t 7 -> SINGLE_2 \t 8 -> SINGLE_3" << endl;
               cin >> mode;
               switch(mode){
               case 1:
                    set_mode = ADS1X15::DIFF_0_1;
                    break;
               case 2:
                    set_mode = ADS1X15::DIFF_0_3;
                    break;
               case 3:
                    set_mode = ADS1X15::DIFF_1_3;
                    break;
               case 4:
                    set_mode = ADS1X15::DIFF_2_3;
                    break;
               case 5:
                    set_mode = ADS1X15::SINGLE_0;
                    break;
               case 6:
                    set_mode = ADS1X15::SINGLE_1;
                    break;
               case 7:
                    set_mode = ADS1X15::SINGLE_2;
                    break;
               case 8:
                    set_mode = ADS1X15::SINGLE_3;
                    break;
               default:
                    set_mode = ADS1X15::DIFF_0_1;
                    break;
               }
               cout << ads->getSample(set_mode) << endl;
               break;
          case 7:
               cout << " enter a float value: " ;
               cin >> inputVoltage;
               ads->setThresh(ADS1115::THRESH_HIGH, inputVoltage);
               break;
          case 8:
               cout << " enter a float value: " ;
               cin >> inputVoltage;
               ads->setThresh(ADS1115::THRESH_LOW, inputVoltage);
               break;
          case 9:
               cout << "Upper " << ads->getThresh(ADS1X15::THRESH_HIGH) << endl;
               cout << "Lower " << ads->getThresh(ADS1X15::THRESH_LOW) << endl;
               break;
          case 10:
               ads->setThresh(ADS1115::THRESH_DEFAULT);
               break;
          case 11:
               ads->setThresh(ADS1015::CONVERSION_RDY);
               break;
          case 12:
               cout << ads->getCompQue() << endl;
               break;
          case 13:
               int que;
               cout << "select one of the following:" << endl;
               cout << "1 -> CQUE_1CONV \t 2 -> CQUE_2CONV \t 3 -> CQUE_3CONV \t 4 -> CQUE_NONE" << endl;
               cin >> que;
               switch(que){
               case 1:
                    ads->setCompQue(ADS1X15::CQUE_1CONV);
                    break;
               case 2:
                    ads->setCompQue(ADS1X15::CQUE_2CONV);
                    break;
               case 3:
                    ads->setCompQue(ADS1X15::CQUE_4CONV);
                    break;
               case 4:
               default:
                    ads->setCompQue(ADS1X15::CQUE_NONE);
                    break;
               }
               break;
          case 14:
               cout << ads->getCompPol() << endl;
               break;
          case 15:
               cout << "select one of the following:" << endl;
               cout << "1 -> active high \t 2 -> active low" << endl;
               cin >> ans;
               if(ans == 1) ads->setCompPol(true);
                else ads->setCompPol(false);
               break;
          case 16:
               cout << ads->getCompMode() << endl;
               break;
          case 17:
               cout << "select one of the following:" << endl;
               cout << "1 -> Window \t 2 -> Traditional (default)" << endl;
               cin >> ans;
               if(ans == 1) ads->setCompMode(true);
                else ads->setCompMode();
               break;
          case 18:
               cout << ads->getCompLatch() << endl;
               break;
          case 19:
               cout << "select one of the following:" << endl;
               cout << "1 -> Latching  \t 2 -> Non-latching (default)" << endl;
               cin >> ans;
               if(ans == 1) ads->setCompLatch(true);
                else ads->setCompLatch();
               break;
          case 20:
               cout << ads->getContinuous() << endl;
               break;
          case 21:
               cout << "select one of the following:" << endl;
               cout << "1 -> Power Down (default)  \t 2 -> Continuous" << endl;
               cin >> ans;
               if(ans == 1) ads->setContinuous(true);
                else ads->setContinuous();
               break;
          case -1:
               break;
          default:

               break;
          }

     }while (command != -1 );

     delete ads;

     return MRAA_SUCCESS;
}
