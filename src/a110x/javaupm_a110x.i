%module javaupm_a110x
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%{
    #include "a110x.hpp"
%}
%include "a110x.hpp"

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
