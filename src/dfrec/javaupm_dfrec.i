%module javaupm_dfrec
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"
%include "typemaps.i"

%include "dfrec.hpp"
%{
    #include "dfrec.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_dfrec");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
