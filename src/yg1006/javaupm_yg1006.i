%module javaupm_yg1006
%include "../upm.i"

%{
    #include "yg1006.hpp"
%}

%include "yg1006.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_yg1006");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}