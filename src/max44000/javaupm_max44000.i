%module javaupm_max44000
%include "../upm.i"

%{
    #include "max44000.hpp"
%}

%include "max44000.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_max44000");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}