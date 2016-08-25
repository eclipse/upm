%module(directors="1") javaupm_nrf24l01
%include "../upm.i"

%feature("director") Callback;
SWIG_DIRECTOR_OWNED(Callback)

%include "arrays_java.i";
%apply signed char[] {uint8_t *};

%include "Callback.hpp"
%{
    #include "nrf24l01.hpp"
%}

%include "nrf24l01.hpp"

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