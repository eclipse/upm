%module pyupm_tm1637
%include "../upm.i"
%include "../carrays_uint8_t.i"

%varargs(4, int digit = 0) write;

%{
    #include "tm1637.h"
%}
%include "tm1637.h"
