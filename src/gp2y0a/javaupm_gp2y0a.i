%module javaupm_gp2y0a
%include "../upm.i"

%{
    #include "gp2y0a.hpp"
%}

%include "gp2y0a.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_gp2y0a");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}