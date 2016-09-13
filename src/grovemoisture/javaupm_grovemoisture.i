%module javaupm_grovemoisture
%include "../upm.i"

%{
    #include "grovemoisture.hpp"
%}

%include "grovemoisture.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovemoisture");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}