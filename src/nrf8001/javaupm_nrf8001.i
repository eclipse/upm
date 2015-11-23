%module javaupm_nrf8001
%include "../upm.i"

%{
    #include "nrf8001.h"
%}

%include "nrf8001.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_nrf8001");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}