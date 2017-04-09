%module javaupm_bmm150
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getMagnetometer(float *, float *, float *);

%include "bmm150_defs.h"
%include "bmm150.hpp"
%{
    #include "bmm150.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmm150");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
