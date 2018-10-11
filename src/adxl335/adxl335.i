#ifdef SWIGPYTHON
%module (package="upm") adxl335
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA

%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;

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

%apply int *OUTPUT { int *xVal, int *yVal, int *zVal };
%apply float *OUTPUT { float *xAccel, float *yAccel, float *zAccel  };

%typemap(jni) float* "jfloatArray"
%typemap(jstype) float* "float[]"
%typemap(jtype) float* "float[]"

%typemap(javaout) float* {
    return $jnicall;
}

%typemap(out) float *acceleration {
    $result = JCALL1(NewFloatArray, jenv, 3);
    JCALL4(SetFloatArrayRegion, jenv, $result, 0, 3, $1);
}

%typemap(jni) int* "jintArray"
%typemap(jstype) int* "int[]"
%typemap(jtype) int* "int[]"

%typemap(javaout) int* {
    return $jnicall;
}

%typemap(out) int *values {
    $result = JCALL1(NewIntArray, jenv, 3);
    JCALL4(SetIntArrayRegion, jenv, $result, 0, 3, (const signed int*)$1);
    delete [] $1;
}

%ignore values(int *, int *, int *);
%ignore acceleration(float *, float *, float *);

JAVA_JNI_LOADLIBRARY(javaupm_adxl335)
#endif
/* END Java syntax */

/* BEGIN Javascript syntax  ------------------------------------------------- */
#ifdef SWIGJAVASCRIPT
%include "../upm_vectortypes.i"
%pointer_functions(int, intp);
%pointer_functions(float, floatp);
#endif
/* END Javascript syntax */

/* BEGIN Python syntax  ----------------------------------------------------- */
#ifdef SWIGPYTHON
%include "../upm_vectortypes.i"
%pointer_functions(int, intp);
%pointer_functions(float, floatp);
#endif
/* END Python syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "adxl335.hpp"
%}
%include "adxl335.hpp"
/* END Common SWIG syntax */
