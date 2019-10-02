#ifdef SWIGPYTHON
%module (package="upm") htu21d
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

%include "arrays_java.i"
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_ms5803)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%pointer_functions(float, floatp);

%{
#include "ms5803_defs.h"
#include "ms5803.hpp"
%}
%include "ms5803_defs.h"
%include "ms5803.hpp"
/* END Common SWIG syntax */
