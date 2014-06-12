%module pyupm_i2clcd

%feature("autodoc", "3");

%include "i2clcd.h"
%{
    #include "i2clcd.h"
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
