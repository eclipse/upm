%module javaupm_ad8232
%include "../upm.i"

%{
    #include "ad8232.h"
%}

%include "ad8232.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ad8232");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}