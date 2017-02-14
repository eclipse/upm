%module javaupm_mag3110
%include "../upm.i"

%{
    #include "mag3110.hpp"
%}

%include "mag3110.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mag3110");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}