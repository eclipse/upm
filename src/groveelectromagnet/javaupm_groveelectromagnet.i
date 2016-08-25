%module javaupm_groveelectromagnet
%include "../upm.i"

%{
    #include "groveelectromagnet.hpp"
%}

%include "groveelectromagnet.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_groveelectromagnet");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}