%module jsupm_i2clcd
%include "../upm.i"
%include "../carrays_uint8_t.i"

%include "ssd.hpp"
%include "lcd.hpp"
%{
    #include "lcd.hpp"
%}

%include "lcm1602.hpp"
%{
    #include "lcm1602.hpp"
%}

%include "jhd1313m1.hpp"
%{
    #include "jhd1313m1.hpp"
%}

%include "ssd1327.hpp"
%{
    #include "ssd1327.hpp"
%}

%include "ssd1308.hpp"
%{
    #include "ssd1308.hpp"
%}

%include "eboled.hpp"
%{
    #include "eboled.hpp"
%}

%include "sainsmartks.hpp"
%{
    #include "sainsmartks.hpp"
%}

%include "ssd1306.hpp"
%{
    #include "ssd1306.hpp"
%}
