%module javaupm_adafruitms1438
%include "../upm.i"

%{
    #include "adafruitms1438.hpp"
%}

%include "adafruitms1438.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_adafruitms1438");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}