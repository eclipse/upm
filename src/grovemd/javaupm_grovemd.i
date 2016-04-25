%module javaupm_grovemd
%include "../upm.i"

%{
    #include "grovemd.hpp"
%}

%include "grovemd.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovemd");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}