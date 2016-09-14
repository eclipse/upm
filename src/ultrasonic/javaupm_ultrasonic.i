%module javaupm_ultrasonic
%include "../upm.i"

%ignore signalISR;

%{
    #include "ultrasonic.hpp"
%}

%include "ultrasonic.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ultrasonic");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
