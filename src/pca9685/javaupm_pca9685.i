%module javaupm_pca9685
%include "../upm.i"
%include "cpointer.i"

%{
    #include "pca9685.h"
%}

%include "pca9685.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_pca9685");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}