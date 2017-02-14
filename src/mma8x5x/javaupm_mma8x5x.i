%module javaupm_mma8x5x
%include "../upm.i"

%{
    #include "mma8x5x.hpp"
%}

%include "mma8x5x.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mma8x5x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}