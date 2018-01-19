%module javaupm_bmm150
%include "../upm.i"
%include "typemaps.i"
%include "../upm_javastdvector.i"

%ignore getMagnetometer(float *, float *, float *);

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

%{
    #include "bmm150.hpp"
    #include "bmm150_defs.h"
%}
%include "bmm150_defs.h"
%include "bmm150.hpp"

%ignore installISR (BMM150_INTERRUPT_PINS_T , int ,  mraa::Edge ,  void *, void *);

%define INTERRUPT BMM150_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::BMM150)

JAVA_JNI_LOADLIBRARY(javaupm_bmm150)
