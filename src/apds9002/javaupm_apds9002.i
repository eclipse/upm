%module javaupm_apds9002
%include "../upm.i"

%{
    #include "apds9002.hpp"
%}

%include "apds9002.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_apds9002");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}