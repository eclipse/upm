#include "mraa.hpp"
#include <iostream>
#include <unistd.h>

#include "ssd1351.hpp"

#define BLACK       0x0000
#define WHITE       0xFFFF
#define INTEL_BLUE  0x0BF8

int main(int argc, char **argv)
{
    // Define colors (16-bit RGB on 5/6/5 bits)
    int colors[] = {0x0000, 0x000F, 0x03E0, 0x03EF,
                    0x7800, 0x780F, 0x7BE0, 0xC618,
                    0x7BEF, 0x001F, 0x07E0, 0x07FF,
                    0xF800, 0xF81F, 0xFFE0, 0xFFFF};
//! [Interesting]
    // Initialize display with pins
    // oc = 0, dc = 1, r  = 2, si = 11, cl = 13
    upm::SSD1351* display = new upm::SSD1351(0, 1, 2);

    // Test lines pixel by pixel
    for(int i = 0; i < SSD1351HEIGHT; i++) {
        for(int j = 0; j < SSD1351WIDTH; j++) {
            display->drawPixel(i, j, colors[i/8]);
        }
    }
    display->refresh();
    sleep(5);

    // Test rectangles
    for(int i = 0; i < SSD1351HEIGHT/32; i++) {
        for (int j = 0; j < SSD1351WIDTH/32; j++) {
            display->fillRect(i * 32, j * 32, 32, 32, colors[i * 4 + j]);
        }
    }
    display->refresh();
    sleep(5);

    // Test circles
    display->fillScreen(0x2104);
    for(int i = 0; i < SSD1351HEIGHT/32; i++) {
        for (int j = 0; j < SSD1351WIDTH/32; j++) {
            display->drawCircle(i * 32 + 15, j * 32 + 15, 15, colors[i * 4 + j]);
        }
    }
    display->refresh();
    sleep(5);

    // Test Text
    display->fillScreen(INTEL_BLUE);
    display->setTextColor(WHITE, INTEL_BLUE);
    display->setTextSize(4);
    display->setCursor(7, 30);
    display->print("Intel");
    display->setCursor(5, 70);
    display->print("IoTDK");
    display->refresh();
//! [Interesting]
}
