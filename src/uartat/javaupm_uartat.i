%module javaupm_uartat
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "uartat_defs.h"
%include "uartat.hpp"
%{
    #include "uartat.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_uartat");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
