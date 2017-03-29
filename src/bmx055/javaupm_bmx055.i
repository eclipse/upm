%module javaupm_bmx055
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "../upm_vectortypes.i"

%apply int {mraa::Edge};

%ignore getAccelerometer(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%include "bmg160_defs.h"
%include "bma250e_defs.h"
%include "bmm150_defs.h"

%include "bmx055.hpp"
%{
    #include "bmx055.hpp"
%}

%include "bmc150.hpp"
%{
    #include "bmc150.hpp"
%}

%include "bmi055.hpp"
%{
    #include "bmi055.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmx055");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
