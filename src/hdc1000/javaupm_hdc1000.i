%module javaupm_hdc1000
%include "../upm.i"

%{
    #include "hdc1000.hpp"
%}

%include "hdc1000.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_hdc1000");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}