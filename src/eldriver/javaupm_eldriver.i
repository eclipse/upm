%module javaupm_eldriver
%include "../upm.i"

%{
    #include "eldriver.hpp"
%}

%include "eldriver.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_eldriver");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}