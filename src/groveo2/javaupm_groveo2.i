%module javaupm_groveo2
%include "../upm.i"

%{
    #include "groveo2.hpp"
%}

%include "groveo2.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_groveo2");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}