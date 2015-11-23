%module javaupm_cjq4435
%include "../upm.i"

%{
    #include "cjq4435.h"
%}

%include "cjq4435.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_cjq4435");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}