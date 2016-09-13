%module javaupm_groveehr
%include "../upm.i"

%ignore beatISR;

%{
    #include "groveehr.hpp"
%}

%include "groveehr.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_groveehr");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}