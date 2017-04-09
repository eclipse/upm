%module javaupm_tmp006
%include "../upm.i"

%{
    #include "tmp006.hpp"
%}

%include "tmp006.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_tmp006");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}