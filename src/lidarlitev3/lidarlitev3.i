#ifdef SWIGPYTHON
%module (package="upm") lidarlitev3
#endif

%import "interfaces/new_interfaces.i"

%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{
import upm_new_interfaces.*;
%}

JAVA_JNI_LOADLIBRARY(javaupm_lidarlitev3)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "lidarlitev3.hpp"
%}
%include "lidarlitev3.hpp"
/* END Common SWIG syntax */
