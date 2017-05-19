%module javaupm_lsm6ds3h
%include "../upm.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%ignore getAccelerometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%include "lsm6ds3h_defs.h"
%include "lsm6ds3h.hpp"
%{
    #include "lsm6ds3h.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_lsm6ds3h");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
