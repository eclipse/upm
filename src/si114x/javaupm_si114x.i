%module javaupm_si114x
%include "../upm.i"
%include "cpointer.i"

%{
    #include "si114x.hpp"
%}

%include "si114x.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_si114x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}