#ifdef SWIGPYTHON
%module (package="upm") dfrec
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_dfrec)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(int, intp);
%pointer_functions(float, floatp);

%{
#include "dfrec.hpp"
%}
%include "dfrec.hpp"
/* END Common SWIG syntax */
