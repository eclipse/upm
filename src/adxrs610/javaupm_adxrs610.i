%module javaupm_adxrs610
%include "../upm.i"

%{
    #include "adxrs610.hpp"
%}

%include "adxrs610.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_adxrs610");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}