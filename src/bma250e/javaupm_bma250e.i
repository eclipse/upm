%module javaupm_bma250e
%include "../upm.i"
%include "typemaps.i"
%include "std_vector.i"

%ignore getAccelerometer(float *, float *, float *);

%template(floatVector) std::vector<float>;

%include "bma250e_defs.h"
%include "bma250e.hpp"
%{
    #include "bma250e.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bma250e");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
