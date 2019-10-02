#ifdef SWIGPYTHON
%module (package="upm") mma7660
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
import java.lang.Integer;
%}

%{
#include <vector>
%}

%typemap(javaout) std::vector<int> {
    return (AbstractList<Integer>)(new $&javaclassname($jnicall, true));
}
%typemap(jstype) std::vector<int> "AbstractList<Integer>"

%template(intVector) std::vector<int>;


%apply float *OUTPUT { float *ax, float *ay, float *az  };
%apply int *OUTPUT { int *x, int *y, int *z };
%ignore installISR(int , void *, void *);

JAVA_ADD_INSTALLISR_PIN(upm::MMA7660)
JAVA_JNI_LOADLIBRARY(javaupm_mma7660)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../upm_vectortypes.i"
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../upm_vectortypes.i"
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%{
#include "mma7660_regs.h"
#include "mma7660.hpp"
%}
%include "mma7660_regs.h"
%include "mma7660.hpp"
/* END Common SWIG syntax */
