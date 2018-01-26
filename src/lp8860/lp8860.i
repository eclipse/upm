%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%typemap(javaimports) SWIGTYPE %{import upm_interfaces.*;%}
%import "../interfaces/javaupm_iLightController.i"

JAVA_JNI_LOADLIBRARY(javaupm_lp8860)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "lp8860.hpp"
%}
%include "lp8860.hpp"
/* END Common SWIG syntax */
