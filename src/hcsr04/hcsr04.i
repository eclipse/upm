#ifdef SWIGPYTHON
%module (package="upm") hcsr04
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_hcsr04)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "hcsr04.hpp"
%}
%include "hcsr04.h"
%include "hcsr04.hpp"
/* END Common SWIG syntax */
