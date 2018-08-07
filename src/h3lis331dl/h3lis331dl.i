#ifdef SWIGPYTHON
%module (package="upm") h3lis331dl
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%{
#include <vector>
%}

%apply float *OUTPUT { float *aX, float *aY, float *aZ  };
%apply int *OUTPUT { int *x, int *y, int*z };

%ignore i2cContext;

%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
%}

%typemap(javaout) std::vector<int> {
    return (AbstractList<Integer>)(new $&javaclassname($jnicall, true));
}
%typemap(jstype) std::vector<int> "AbstractList<Integer>"

%template(intVector) std::vector<int>;

JAVA_JNI_LOADLIBRARY(javaupm_h3lis331dl)
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
#include "h3lis331dl.hpp"
%}
%include "h3lis331dl.hpp"
/* END Common SWIG syntax */
