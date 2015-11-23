%module javaupm_stepmotor
%include "../upm.i"

%{
    #include "stepmotor.h"
%}

%include "stepmotor.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_stepmotor");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}