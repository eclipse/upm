%module javaupm_wfs
%include "../upm.i"

%ignore flowISR;

%{
    #include "wfs.hpp"
%}

%include "wfs.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_wfs");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
