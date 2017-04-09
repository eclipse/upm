%module javaupm_ims
%include "../upm.i"

%{
    #include "ims.hpp"
%}

%include "ims_defs.h"
%include "ims.hpp"

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_ims");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
