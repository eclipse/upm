%module javaupm_hx711
%include "../upm.i"

%{
    #include "hx711.hpp"
%}

%include "hx711.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hx711");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}