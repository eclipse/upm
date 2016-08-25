%module javaupm_grovegsr
%include "../upm.i"

%{
    #include "grovegsr.hpp"
%}

%include "grovegsr.hpp"

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