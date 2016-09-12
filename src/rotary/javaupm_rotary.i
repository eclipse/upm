%module javaupm_rotary
%include "../upm.i"

%{
    #include "rotary.hpp"
%}

%include "rotary.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rotary");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
