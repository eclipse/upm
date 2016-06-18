%module javaupm_grovelinefinder
%include "../upm.i"

%{
    #include "grovelinefinder.hpp"
%}

%include "grovelinefinder.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovelinefinder");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}