%module javaupm_zfm20
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"
%include "arrays_java.i";
%include "../java_buffer.i";

%apply uint16_t *OUTPUT { uint16_t *id, uint16_t *score };

%{
    #include "zfm20.hpp"
    speed_t int_B57600 = B57600;
%}


%include "zfm20.hpp"
speed_t int_B57600 = B57600;

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_zfm20");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}