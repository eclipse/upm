%module javaupm_lm35
%include "../upm.i"

%{
    #include "lm35.hpp"
%}

%include "lm35.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lm35");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}