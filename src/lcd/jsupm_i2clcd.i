%module jsupm_i2clcd
%include "../upm.i"
%include "../carrays_uint8_t.i"

%include "ssd.h"
%include "lcd.h"
%{
    #include "lcd.h"
%}

%include "lcm1602.h"
%{
    #include "lcm1602.h"
%}

%include "jhd1313m1.h"
%{
    #include "jhd1313m1.h"
%}

%include "ssd1327.h"
%{
    #include "ssd1327.h"
%}

%include "ssd1308.h"
%{
    #include "ssd1308.h"
%}

%include "eboled.h"
%{
    #include "eboled.h"
%}

%include "sainsmartks.h"
%{
    #include "sainsmartks.h"
%}

%include "ssd1306.h"
%{
    #include "ssd1306.h"
%}
