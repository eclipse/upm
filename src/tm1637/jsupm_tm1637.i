%module jsupm_tm1637
%include "../upm.i"
%include "../carrays_uint8_t.i"

%varargs(4, int digit = 0) write;

%rename("writeArray")  write(uint8_t *digits);
%rename("writeString") write(std::string digits);

%{
    #include "tm1637.hpp"
%}

%include "tm1637.hpp"

