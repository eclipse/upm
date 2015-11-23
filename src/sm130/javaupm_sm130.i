%module javaupm_sm130
%include "../upm.i"

%{
    #include "sm130.h"
%}

%include "sm130.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_sm130");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}