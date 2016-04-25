%module javaupm_cjq4435
%include "../upm.i"

%{
    #include "cjq4435.hpp"
%}

%include "cjq4435.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_cjq4435");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}