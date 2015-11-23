%module javaupm_grovevdiv
%include "../upm.i"

%{
    #include "grovevdiv.h"
%}

%include "grovevdiv.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovevdiv");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}