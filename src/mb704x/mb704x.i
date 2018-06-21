#ifdef SWIGPYTHON
%module (package="pyupm_new_interfaces") mb704x
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_mb704x)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "mb704x.hpp"
%}
%include "mb704x.hpp"
/* END Common SWIG syntax */
