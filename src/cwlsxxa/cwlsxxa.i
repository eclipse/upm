#ifdef SWIGPYTHON
%module (package="upm") cwlsxxa
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_cwlsxxa)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "cwlsxxa.hpp"
%}
%include "cwlsxxa.hpp"
/* END Common SWIG syntax */
