%module javaupm_bmg160
%include "../upm.i"
%include "typemaps.i"
%include "std_vector.i"

%ignore getGyroscope(float *, float *, float *);

%template(floatVector) std::vector<float>;

%include "bmg160_defs.h"
%include "bmg160.hpp"
%{
    #include "bmg160.hpp"
%}

%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("javaupm_bmg160");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
