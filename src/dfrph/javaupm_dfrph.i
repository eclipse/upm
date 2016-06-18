%module javaupm_dfrph
%include "../upm.i"

%{
    #include "dfrph.hpp"
%}

%include "dfrph.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_dfrph");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}