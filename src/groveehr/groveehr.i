#ifdef SWIGPYTHON
%module (package="upm") ehr
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
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
