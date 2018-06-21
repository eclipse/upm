#ifdef SWIGPYTHON
%module (package="pyupm_new_interfaces") urm37
#endif

%import "interfaces/new_interfaces.i"
%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_urm37)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "urm37.hpp"
%}
%include "urm37.hpp"
/* END Common SWIG syntax */
