%module javaupm_adafruitss
%include "../upm.i"

%include "adafruitss.h"
%{
    #include "adafruitss.h"
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