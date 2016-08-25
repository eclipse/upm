%module jsupm_hmtrp
%include "../upm.i"
%include "../carrays_uint8_t.i"
%include "../carrays_uint16_t.i"
%include "../carrays_uint32_t.i"

%{
    #include "hmtrp.hpp"
    speed_t int_B9600 = B9600;
%}

%include "hmtrp.hpp"
speed_t int_B9600 = B9600;
%array_class(char, charArray);
