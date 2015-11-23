%module javaupm_groveemg
%include "../upm.i"

%{
    #include "groveemg.h"
%}

%include "groveemg.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_groveemg");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}