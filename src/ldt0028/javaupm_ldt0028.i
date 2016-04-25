%module javaupm_ldt0028
%include "../upm.i"

%{
    #include "ldt0028.hpp"
%}

%include "ldt0028.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ldt0028");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}