#ifdef SWIGPYTHON
%module (package="upm") tex00
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_tex00)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "tex00.hpp"
%}
%include "tex00.hpp"
/* END Common SWIG syntax */
