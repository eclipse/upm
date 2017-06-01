%module javaupm_mics6814
%include "../upm.i"

%{
    #include "mics6814.h"
%}

%include "mics6814.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mics6814");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
