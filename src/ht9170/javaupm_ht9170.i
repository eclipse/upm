%module javaupm_ht9170
%include "../upm.i"

%{
    #include "ht9170.hpp"
%}

%include "ht9170.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ht9170");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}