%module javaupm_veml6070
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%{
    #include "veml6070.hpp"
%}
%include "veml6070.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_veml6070");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
