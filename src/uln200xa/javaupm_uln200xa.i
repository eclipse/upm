%module javaupm_uln200xa
%include "../upm.i"

%{
    #include "uln200xa.h"
%}

%include "uln200xa.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_uln200xa");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}