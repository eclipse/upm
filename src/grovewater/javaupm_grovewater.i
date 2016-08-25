%module javaupm_grovewater
%include "../upm.i"

%{
    #include "grovewater.hpp"
%}

%include "grovewater.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovewater");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}