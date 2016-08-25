%module javaupm_micsv89
%include "../upm.i"

%{
    #include "micsv89.hpp"
%}

%include "micsv89.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_micsv89");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}