%module javaupm_mg811
%include "../upm.i"

%{
    #include "mg811.hpp"
%}

%include "mg811.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mg811");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}