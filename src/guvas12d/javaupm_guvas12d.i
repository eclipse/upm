%module javaupm_guvas12d
%include "../upm.i"

%{
    #include "guvas12d.h"
%}

%include "guvas12d.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_guvas12d");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}