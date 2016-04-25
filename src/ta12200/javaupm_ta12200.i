%module javaupm_ta12200
%include "../upm.i"

%{
    #include "ta12200.hpp"
%}

%include "ta12200.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ta12200");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}