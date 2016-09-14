%module javaupm_groveeldriver
%include "../upm.i"

%{
    #include "groveeldriver.hpp"
%}

%include "groveeldriver.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_groveeldriver");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}