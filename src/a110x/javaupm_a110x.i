%module javaupm_a110x
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%{
    #include "a110x.h"
%}
%include "a110x.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_a110x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
