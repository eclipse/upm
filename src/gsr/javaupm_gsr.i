%module javaupm_gsr
%include "../upm.i"

%{
    #include "gsr.hpp"
%}

%include "gsr.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_gsr");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}