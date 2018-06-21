#ifdef SWIGPYTHON
%module (package="upm") ehr
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

%ignore beatISR;

JAVA_JNI_LOADLIBRARY(javaupm_ehr)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ehr.hpp"
%}
%include "ehr.hpp"
/* END Common SWIG syntax */
