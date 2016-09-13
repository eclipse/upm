%module javaupm_grovecollision
%include "../upm.i"

%{
    #include "grovecollision.hpp"
%}

%include "grovecollision.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovecollision");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}