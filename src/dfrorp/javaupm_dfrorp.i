%module javaupm_dfrorp
%include "../upm.i"
%include "std_string.i"
%include "cpointer.i"
%include "typemaps.i"

%include "dfrorp.hpp"
%{
    #include "dfrorp.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_dfrorp");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
