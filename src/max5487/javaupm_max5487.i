%module javaupm_max5487
%include "../upm.i"

%{
    #include "max5487.hpp"
%}

%include "max5487.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_max5487");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}