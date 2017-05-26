%module javaupm_bmx055
%include "../upm.i"
%include "cpointer.i"
%include "typemaps.i"
%include "../upm_javastdvector.i"

%apply int {mraa::Edge};

%ignore getAccelerometer(float *, float *, float *);
%ignore getMagnetometer(float *, float *, float *);
%ignore getGyroscope(float *, float *, float *);

%typemap(javaimports) SWIGTYPE %{
import java.util.AbstractList;
import java.lang.Float;
%}

%typemap(javaout) SWIGTYPE {
    return new $&javaclassname($jnicall, true);
}
%typemap(javaout) std::vector<float> {
    return (AbstractList<Float>)(new $&javaclassname($jnicall, true));
}
%typemap(jstype) std::vector<float> "AbstractList<Float>"

%template(floatVector) std::vector<float>;

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
