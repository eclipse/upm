#ifdef SWIGPYTHON
%module (package="upm") sht1x
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_sht1x)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "sht1x_defs.h"
#include "sht1x.hpp"
%}
%include "sht1x_defs.h"
%include "sht1x.hpp"
/* END Common SWIG syntax */
