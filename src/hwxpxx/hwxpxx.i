#ifdef SWIGPYTHON
%module (package="upm") hwxpxx
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_hwxpxx)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "hwxpxx.hpp"
%}
%include "hwxpxx.hpp"
/* END Common SWIG syntax */
