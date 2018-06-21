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

JAVA_JNI_LOADLIBRARY(javaupm_bmpx8x)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "bmpx8x_defs.h"
#include "bmpx8x.hpp"
%}
%include "bmpx8x_defs.h"
%include "bmpx8x.hpp"
/* END Common SWIG syntax */
