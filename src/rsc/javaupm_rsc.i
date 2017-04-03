%module javaupm_rsc
%include "../upm.i"

%{
    #include "rsc.hpp"
%}

%include "rsc.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_rsc");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
