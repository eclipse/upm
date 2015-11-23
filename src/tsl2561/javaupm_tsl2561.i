%module javaupm_tsl2561
%include "../upm.i"

%{
    #include "tsl2561.h"
%}

%include "tsl2561.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_tsl2561");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}