%module javaupm_grovescam
%include "../upm.i"
%include "../java_buffer.i"

%{
    #include "grovescam.h"
%}

%include "grovescam.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovescam");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}