%module javaupm_bmpx8x
%include "../upm.i"

%include "../interfaces/javaupm_iTemperatureSensor.i"
%include "../interfaces/javaupm_iPressureSensor.i"

%{
    #include "bmpx8x.hpp"
%}

%include "bmpx8x_defs.h"
%include "bmpx8x.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmpx8x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
