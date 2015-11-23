%module javaupm_grovegsr
%include "../upm.i"

%{
    #include "grovegsr.h"
%}

%include "grovegsr.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovegsr");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}