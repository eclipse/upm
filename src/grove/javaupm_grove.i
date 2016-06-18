%module (docstring="Basic Grove sensors") javaupm_grove

%include "../upm.i"

%apply int {mraa::Edge}

%{
    #include "grove.hpp"
%}

%include "grove.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grove");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}

