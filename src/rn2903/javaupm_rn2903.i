%module javaupm_rn2903
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "rn2903_defs.h"
%include "rn2903.hpp"
%{
    #include "rn2903.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rn2903");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
