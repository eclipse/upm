%module javaupm_t3311
%include "../upm.i"
%include "typemaps.i"

%{
    #include "t3311.h"
%}

%include "t3311.h"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_t3311");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
