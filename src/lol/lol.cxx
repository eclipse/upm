/*
 * Author: Alexander Komarov <alexander.komarov@intel.com>
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
#include <unistd.h>
#include <stdlib.h>
#include <functional>
#include <string.h>
#include "lol.h"

using namespace upm;

static mraa_gpio_context *m_Ctx;
static unsigned char *buffer;

static mraa_gpio_context c1, c2, c3, c4;

static int charlie_pairs [12][22] = {
{3,124, 4,110, 5,96,  6,82,  7,68, 8,54, 9,40, 10,26, 11,12, -1,-1, -1,-1},
{3,122, 4,108, 5,94,  6,80,  7,66, 8,52, 9,38, 10,24, 11,10, -1,-1, -1,-1},
{3,120, 4,106, 5,92,  6,78,  7,64, 8,50, 9,36, 10,22, 11,8,  -1,-1, -1,-1},
{0,125, 1,123, 2,121, 4,98,  5,84, 6,70, 7,56, 8,42,  9,28,  10,14, 11,0},
{0,111, 1,109, 2,107, 3,112, 5,85, 6,71, 7,57, 8,43, 9,29,   10,15, 11,1},
{0,97,  1,95,  2,93,  3,113, 4,99, 6,72, 7,58, 8,44, 9,30,   10,16, 11,2},
{0,83,  1,81,  2,79,  3,114, 4,100,5,86, 7,59, 8,45, 9,31,   10,17, 11,3},
{0,69,  1,67,  2,65,  3,115, 4,101,5,87, 6,73, 8,46, 9,32,   10,18, 11,4},
{0,55,  1,53,  2,51,  3,116, 4,102,5,88, 6,74, 7,60, 9,33,   10,19, 11,5},
{0,41,  1,39,  2,37,  3,117, 4,103,5,89, 6,75, 7,61, 8,47,   10,20, 11,6},
{0,27,  1,25,  2,23,  3,118, 4,104,5,90, 6,76, 7,62, 8,48,   9,34,  11,7},
{0,13,  1,11,  2,9,   3,119, 4,105,5,91, 6,77, 7,63, 8,49,   9,35,  10,21}
};

void clear_gpio(int gpio)
{
    mraa_gpio_mode(m_Ctx[gpio], MRAA_GPIO_HIZ);
    mraa_gpio_dir(m_Ctx[gpio], MRAA_GPIO_IN);
}

void clear_prev_cycle(int cycle)
{
    int i;

    // What is prev cycle?
    cycle--;
    if (cycle == -1)
        cycle = 11;

    // Disable all "1"'s
    for (i = 0; i < 11; i++) {
        if (charlie_pairs[cycle][i*2] == -1)
            break;
        if(buffer[charlie_pairs[cycle][i*2 + 1]])
            clear_gpio(charlie_pairs[cycle][i*2]);
    }

// Disable "0"
    clear_gpio(cycle);
}

void set_strong_one(int gpio)
{
    mraa_gpio_dir(m_Ctx[gpio], MRAA_GPIO_OUT);
    mraa_gpio_mode(m_Ctx[gpio], MRAA_GPIO_STRONG);
    mraa_gpio_write(m_Ctx[gpio], 1);
}

void set_strong_zero(int gpio)
{
    mraa_gpio_dir(m_Ctx[gpio], MRAA_GPIO_OUT);
    mraa_gpio_mode(m_Ctx[gpio], MRAA_GPIO_STRONG);
    mraa_gpio_write(m_Ctx[gpio], 0);
}


static void clear_gpios()
{
    int i;
    for (i = 0; i < 12; i++)
        clear_gpio(i);
}

void *do_draw(void *arg)
{
    clear_gpios();

    while (1) {
        int i, cur;
        uint8_t cycle = 0;
        // 12 Cycles of Matrix
        for (cycle = 0; cycle < 12; cycle++)
        {
            if (cycle == 12) cycle = 0;

            clear_prev_cycle(cycle);
            // set strong/0 on current cycle line
            set_strong_zero(cycle);

            // draw ones from framebuffer
            for (i = 0; i < 11; i++) {
                cur = charlie_pairs[cycle][i*2];
                if (cur == -1) break;

                if (buffer[charlie_pairs[cycle][i*2 + 1]]) {
                    set_strong_one(cur);
//  printf("cycle %d %d %d %d\n", cycle, i, charlie_pairs[cycle][i*2 + 1],
//           buffer[charlie_pairs[cycle][i*2 + 1]]);
                }
            }
        }

    }
}

LoL::LoL() {
    int i = 0;
    mraa_result_t error;
    for (i = 0; i < 12; i++)
      {
        if ( !(m_LoLCtx[i] = mraa_gpio_init(i+2)) ) 
          {
            throw std::invalid_argument(std::string(__FUNCTION__) +
                                        ": mraa_gpio_init() failed, invalid pin?");
            return;
          }

      }

    memset(framebuffer, 0, LOL_X*LOL_Y);

    // I am optimistic and stupid - thread creation
    // always works in my world
    buffer = (unsigned char*)framebuffer;
    m_Ctx = m_LoLCtx;
    pthread_create (&drawer_thread, NULL, do_draw, NULL);
}

LoL::~LoL() {
    int i = 0;
    mraa_result_t error;
    for (i = 0; i < 12; i++)
        mraa_gpio_close(m_LoLCtx[i]);
}

unsigned char* LoL::getFramebuffer() {
    return framebuffer;
}

unsigned char LoL::setPixel(int x, int y, unsigned char pixel)
{
    if (x < 0 || y < 0 || x >= LOL_X || y >= LOL_Y)
        return -1;

    framebuffer[x + LOL_X*y] = (pixel == 0) ? 0 : 1;
    return 0;
}

unsigned char LoL::getPixel(int x, int y)
{
    if (x < 0 || y < 0 || x >= LOL_X || y >= LOL_Y)
        return -1;

    return (framebuffer[x + LOL_X*y] == 0) ? 0 : 1;
}

