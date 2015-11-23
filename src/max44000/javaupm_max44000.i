%module javaupm_max44000
%include "../upm.i"

%{
    #include "max44000.h"
%}

%include "max44000.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_max44000");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}