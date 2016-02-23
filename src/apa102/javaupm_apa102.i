%module javaupm_apa102

%include "../upm.i"
%include "typemaps.i"

%{
    #include "apa102.h"
%}

%include "apa102.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_apa102");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
