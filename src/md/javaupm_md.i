%module javaupm_md
%include "../upm.i"

%{
    #include "md.hpp"
%}

%include "md.h"
%include "md.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_md");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
