%module javaupm_grovevdiv
%include "../upm.i"

%{
    #include "grovevdiv.hpp"
%}

%include "grovevdiv.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_grovevdiv");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}