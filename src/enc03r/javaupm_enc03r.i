%module javaupm_enc03r
%include "../upm.i"

%{
    #include "enc03r.hpp"
%}

%include "enc03r.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_enc03r");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}