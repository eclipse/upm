%module javaupm_mcp9808
%include "../upm.i"

%{
    #include "mcp9808.h"
%}

%include "mcp9808.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_mcp9808");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}