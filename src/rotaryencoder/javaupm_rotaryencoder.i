%module javaupm_rotaryencoder
%include "../upm.i"

%ignore signalAISR;

%{
    #include "rotaryencoder.hpp"
%}

%include "rotaryencoder.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rotaryencoder");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}