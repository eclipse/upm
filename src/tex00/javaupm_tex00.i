%module javaupm_tex00
%include "../upm.i"
%include "std_string.i"

%include "tex00.h"
%{
    #include "tex00.h"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_tex00");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
