%module pyupm_wt5001
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"

%feature("autodoc", "3");

%{
    #include "wt5001.h"
    speed_t int_B9600 = B9600;
%}
%include "wt5001.h"
speed_t int_B9600 = B9600;
