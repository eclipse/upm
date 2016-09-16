%module javaupm_sht1x
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "sht1x.hpp"
%{
    #include "sht1x.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_sht1x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
