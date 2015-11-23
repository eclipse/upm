%module javaupm_tcs3414cs
%include "../upm.i"

%{
    #include "tcs3414cs.h"
%}

%include "tcs3414cs.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_tcs3414cs");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}