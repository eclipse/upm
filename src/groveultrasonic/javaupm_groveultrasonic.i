%module javaupm_groveultrasonic
%include "../upm.i"

%ignore signalISR;

%{
    #include "groveultrasonic.h"
%}

%include "groveultrasonic.h"

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
