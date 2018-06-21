#ifdef SWIGPYTHON
%module (package="upm") htu21d
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_ms5611)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ms5611.hpp"
%}
%include "ms5611.hpp"
/* END Common SWIG syntax */
