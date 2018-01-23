%module javaupm_bmg160
%include "../upm.i"
%include "typemaps.i"
%include "../upm_javastdvector.i"

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

%{
    #include "bmg160.hpp"
    #include "bmg160_defs.h"
%}
%include "bmg160_defs.h"
%include "bmg160.hpp"


%ignore installISR (BMG160_INTERRUPT_PINS_T , int   mraa::Edge ,  void *, void *);

%define INTERRUPT BMG160_INTERRUPT_PINS_T
%enddef
JAVA_ADD_INSTALLISR_INTERRUPT(upm::BMG160)

JAVA_JNI_LOADLIBRARY(javaupm_bmg160)
