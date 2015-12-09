%module(directors="1") javaupm_nrf24l01
%include "../upm.i"

%feature("director") Callback;
SWIG_DIRECTOR_OWNED(Callback)

%include "arrays_java.i";
%apply signed char[] {uint8_t *};

%include "Callback.h"
%{
    #include "nrf24l01.h"
%}

%include "nrf24l01.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_nrf24l01");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}