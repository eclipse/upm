%module javaupm_max31723
%include "../upm.i"

%{
    #include "max31723.hpp"
%}

%include "max31723.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_max31723");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}