%module javaupm_groveultrasonic
%include "../upm.i"

%ignore signalISR;

%{
    #include "groveultrasonic.hpp"
%}

%include "groveultrasonic.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_groveultrasonic");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
