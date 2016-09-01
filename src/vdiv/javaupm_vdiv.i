%module javaupm_vdiv
%include "../upm.i"

%{
    #include "vdiv.hpp"
%}

%include "vdiv.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_vdiv");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}