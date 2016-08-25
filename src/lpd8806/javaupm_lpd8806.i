%module javaupm_lpd8806
%include "../upm.i"

%{
    #include "lpd8806.hpp"
%}

%include "lpd8806.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lpd8806");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}