%module javaupm_grovespeaker
%include "../upm.i"

%{
    #include "grovespeaker.h"
%}

%include "grovespeaker.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovespeaker");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}