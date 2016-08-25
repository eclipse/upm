%module javaupm_adis16448
%include "../upm.i"

%{
    #include "adis16448.hpp"
%}

%include "adis16448.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_adis16448");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}