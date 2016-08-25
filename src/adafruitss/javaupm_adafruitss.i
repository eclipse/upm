%module javaupm_adafruitss
%include "../upm.i"

%include "adafruitss.hpp"
%{
    #include "adafruitss.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_adafruitss");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}