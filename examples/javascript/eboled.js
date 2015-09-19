/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Author: Tyler Gibson <tgibson@microsoft.com>
 * Copyright (c) 2015 Microsoft Corporation.
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

var lcdObj = require('jsupm_i2clcd');
var oled = new lcdObj.EBOLED();

var sample = 0;
var samples = 13;

function exit()
{
  oled = null;
  lcdObj.cleanUp();
  lcdObj = null;
  process.exit(0);
}

setInterval( function()
{
  if(sample>samples)
  {
    exit();
  }
  oled.clearScreenBuffer();
  runSample(sample++);
  oled.refresh();
}, 1500);

function runSample(sample)
{
  switch(sample) {
    case 0:
      // x/y coords are 0 based, using 1 here for padding.
      oled.setCursor(1,1);
      // nowrap = 0, wrapping = 1
      oled.setTextWrap(1);
      oled.write("HELLO WORLD! Mixed with #123 and y's, g's and q's.");
      break;
    case 1:
      oled.setCursor(12, 1);
      //multiply text size, only integers
      oled.setTextSize(3);
      oled.write("BOO!");
      oled.setTextSize(1);
      break;
    case 2:
      oled.drawRectangleFilled(0,0,48,9);
      oled.setCursor(1,1);
      // 0=Black, 1=White, 2=Xor (Toggle)
      oled.setTextColor(2);
      oled.write("Cutout");
      break;
    case 3:
      var total = Math.random()*100;
      for(var stars=0; stars<total; stars++ )
        oled.drawPixel(Math.floor(Math.random()*63), Math.floor(Math.random()*47), 1);
      break;
    case 4:
      for(var burst=0; burst<12; burst++)
        oled.drawLine(31, 24, Math.floor(Math.random()*63), Math.floor(Math.random()*47), 1);
      break;
    case 5:
      var lastPeak = 24;
      for(var peak=0; peak < 64; peak++)
      {
        var thisPeak = Math.abs(lastPeak + Math.floor(Math.random()*(-6) + Math.random()*6));
        oled.drawLine(peak, thisPeak, peak, 47, 1);
        lastPeak = thisPeak;
      }
      break;
    case 6:
      for(var y=0; y<47; y++)
      {
        oled.drawLineHorizontal(0,y+1,63,2);
        oled.refresh();
        oled.drawLineHorizontal(0,y,63,2);
      }
      break;
    case 7:
      var eqbarHeights = [ Math.floor(Math.random()*32),
                           Math.floor(Math.random()*32),
                           Math.floor(Math.random()*32),
                           Math.floor(Math.random()*32),
                           Math.floor(Math.random()*32),
                           Math.floor(Math.random()*32),
                           Math.floor(Math.random()*32) ];
      var begin = Date.now();
      while(Date.now()-begin < 2000)
      {
        oled.clearScreenBuffer();
        for(var eqbar=0; eqbar<7; eqbar++)
        {
          oled.drawRectangleFilled(eqbar*9, 49 - eqbarHeights[eqbar], 8, eqbarHeights[eqbar], 1);
          eqbarHeights[eqbar] = eqbarHeights[eqbar] + Math.random()*(-2) + Math.random()*2;
          if(eqbarHeights[eqbar]<0)
            eqbarHeights[eqbar] = 1;
        }
        oled.refresh();
      }
      oled.clear();
      break;
    case 8:
      oled.drawRoundedRectangle(8, 8, 48, 16, 4, 1);
      oled.setCursor(12, 16);
      oled.write("Cancel");
      break;
    case 9:
      oled.drawTriangle(2, 2, 52, 7, 17, 37, 1);
      break;
    case 10:
      oled.drawTriangleFilled(2, 2, 52, 7, 17, 37, 1);
      break;
    case 11:
      oled.drawCircle(32, 24, 14, 1);
      break;
    case 12:
      oled.drawCircleFilled(32, 24, 14, 1);
      break;
    case 13:
      oled.fillScreen(1);
      break;
  }
}
