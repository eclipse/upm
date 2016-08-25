%module javaupm_groveemg
%include "../upm.i"

%{
    #include "groveemg.hpp"
%}

%include "groveemg.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_groveemg");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}