#ifdef SWIGPYTHON
%module (package="upm") ecezo
#endif

%import "interfaces/interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_ecezo)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ecezo_defs.h"
#include "ecezo.hpp"
%}
%include "ecezo_defs.h"
%include "ecezo.hpp"
/* END Common SWIG syntax */
