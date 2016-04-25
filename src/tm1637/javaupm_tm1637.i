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


%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_tm1637");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}