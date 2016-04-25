//! [Interesting]
%module javaupm_max31855
%include "../upm.i"

%{
    #include "max31855.hpp"
%}

%include "max31855.hpp"
//! [Interesting]

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_max31855");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}