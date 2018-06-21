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

JAVA_JNI_LOADLIBRARY(javaupm_abp)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "abp.hpp"
%}
%include "abp.hpp"
/* END Common SWIG syntax */
