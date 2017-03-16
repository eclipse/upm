%module javaupm_abpdrrt005pg2a5
%include "../upm.i"
%include "stdint.i"
%include "typemaps.i"

%{
    #include "abpdrrt005pg2a5.hpp"
%}
%include "abpdrrt005pg2a5.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_abpdrrt005pg2a5");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
