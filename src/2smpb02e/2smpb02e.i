#ifdef SWIGPYTHON
%module (package="upm") 2smpb02e
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_2smpb02e)

#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "2smpb02e.hpp"
%}

%include "2smpb02e.hpp"
/* END Common SWIG syntax */
