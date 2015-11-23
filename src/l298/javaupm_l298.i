%module javaupm_l298
%include "../upm.i"

%{
    #include "l298.h"
%}

%include "l298.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_l298");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}