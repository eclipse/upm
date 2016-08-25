%module javaupm_maxsonarez
%include "../upm.i"

%{
    #include "maxsonarez.hpp"
%}

%include "maxsonarez.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_maxsonarez");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}