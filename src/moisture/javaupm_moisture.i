%module javaupm_moisture
%include "../upm.i"

%{
    #include "moisture.hpp"
%}

%include "moisture.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_moisture");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}