%module javaupm_i2clcd
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "arrays_java.i";
        
%apply unsigned char[] {uint8_t *};
%apply uint8_t *OUTPUT { uint8_t *data };

%include "ssd.h"
%{
    #include "ssd.h"
%}

%include "lcd.h"
%{
    #include "lcd.h"
%}

%include "jhd1313m1.h"
%{
    #include "jhd1313m1.h"
%}

%include "lcm1602.h"
%{
    #include "lcm1602.h"
%}

%include "ssd1327.h"
%{
    #include "ssd1327.h"
%}

%include "ssd1308.h"
%{
    #include "ssd1308.h"
%}

%include "sainsmartks.h"
%{
    #include "sainsmartks.h"
%}

%include "eboled.h"
%{
    #include "eboled.h"
%}

