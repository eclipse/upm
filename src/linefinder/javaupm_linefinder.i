%module javaupm_linefinder
%include "../upm.i"

%{
    #include "linefinder.hpp"
%}

%include "linefinder.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_linefinder");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}