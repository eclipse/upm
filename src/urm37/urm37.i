#ifdef SWIGPYTHON
%module (package="upm") urm37
#endif

%import "interfaces/interfaces.i"
%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
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
