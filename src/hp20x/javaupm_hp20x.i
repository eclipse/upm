%module javaupm_hp20x
%include "../upm.i"

%{
    #include "hp20x.hpp"
%}

%include "hp20x.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hp20x");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}