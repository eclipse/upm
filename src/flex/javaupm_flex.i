%module javaupm_flex
%include "../upm.i"

%{
    #include "flex.h"
%}

%include "flex.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_flex");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}