#ifdef SWIGPYTHON
%module (package="pyupm_new_interfaces") hwxpxx
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
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
