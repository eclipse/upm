%module javaupm_loudness
%include "../upm.i"

%{
    #include "loudness.hpp"
%}

%include "loudness.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_loudness");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}