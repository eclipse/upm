%module javaupm_tcs37727
%include "../upm.i"

%{
    #include "tcs37727.hpp"
%}

%include "tcs37727.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_tcs37727");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}