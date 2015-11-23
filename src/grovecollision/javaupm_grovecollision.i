%module javaupm_grovecollision
%include "../upm.i"

%{
    #include "grovecollision.h"
%}

%include "grovecollision.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovecollision");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}