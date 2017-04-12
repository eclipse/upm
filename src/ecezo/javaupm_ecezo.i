%module javaupm_ecezo
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "ecezo_defs.h"
%include "ecezo.hpp"
%{
    #include "ecezo.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ecezo");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
