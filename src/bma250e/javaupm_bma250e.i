%module javaupm_bma250e
%include "../upm.i"
%include "typemaps.i"
%include "../upm_javastdvector.i"

%ignore getAccelerometer(float *, float *, float *);

%typemap(javaimports) SWIGTYPE %{
import java.util.AbstractList;
import java.lang.Float;
%}

%typemap(javaout) upm::BMA250E {
    return new $&javaclassname($jnicall, true);
}
%typemap(javaout) std::vector<float> {
    return (AbstractList<Float>)(new $&javaclassname($jnicall, true));
}
%typemap(jstype) std::vector<float> "AbstractList<Float>"

%template(floatVector) std::vector<float>;

%include "bma250e_defs.h"
%include "bma250e.hpp"
%{
    #include "bma250e.hpp"
%}

JAVA_JNI_LOADLIBRARY(javaupm_bma250e)
