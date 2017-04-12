%module javaupm_grovewfs
%include "../upm.i"

%ignore flowISR;

%{
    #include "grovewfs.hpp"
%}

%include "grovewfs.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovewfs");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}