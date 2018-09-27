#ifdef SWIGPYTHON
%module (package="upm") mma7455
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"
%apply short *OUTPUT { short * ptrX, short * ptrY, short * ptrZ };

%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;

import java.util.AbstractList;
import java.lang.Float;
import java.lang.Short;
%}

%{
#include <vector>
%}

%typemap(javaout) std::vector<short> {
    return (AbstractList<Short>)(new $&javaclassname($jnicall, true));
}
%typemap(jstype) std::vector<short> "AbstractList<Short>"

%template(shortVector) std::vector<short>;

JAVA_JNI_LOADLIBRARY(javaupm_mma7455)
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
%{
#include "mma7455.hpp"
%}
%include "mma7455.hpp"
/* END Common SWIG syntax */
