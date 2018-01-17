%module javaupm_tm1637
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%varargs(4, int digit = 0) write;

%rename("writeArray")  write(uint8_t *digits);
%rename("writeString") write(std::string digits);

%apply uint8_t *INPUT { uint8_t *digits }

%{
    #include "tm1637.hpp"
%}

%include "tm1637.hpp"


JAVA_JNI_LOADLIBRARY(javaupm_tm1637)