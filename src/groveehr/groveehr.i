#ifdef SWIGPYTHON
%module (package="pyupm_new_interfaces") ehr
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

%ignore beatISR;

JAVA_JNI_LOADLIBRARY(javaupm_groveehr)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "groveehr.hpp"
%}
%include "groveehr.hpp"
/* END Common SWIG syntax */
