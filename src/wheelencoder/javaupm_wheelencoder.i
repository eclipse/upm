%module javaupm_wheelencoder
%include "../upm.i"

%{
    #include "wheelencoder.hpp"
%}

%include "wheelencoder.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_wheelencoder");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}