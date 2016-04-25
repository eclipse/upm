%module javaupm_biss0001
%include "../upm.i"

%{
    #include "biss0001.hpp"
%}

%include "biss0001.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_biss0001");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}