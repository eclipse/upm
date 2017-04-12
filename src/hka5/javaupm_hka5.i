%module javaupm_hka5
%include "../upm.i"
%include "std_string.i"
%include "stdint.i"
%include "typemaps.i"

%include "hka5.hpp"
%{
    #include "hka5.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hka5");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
