%module javaupm_bmpx8x
%include "../upm.i"

%{
    #include "bmpx8x.h"
%}

%include "bmpx8x.h"

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