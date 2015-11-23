%module javaupm_hp20x
%include "../upm.i"

%{
    #include "hp20x.h"
%}

%include "hp20x.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hp20x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}