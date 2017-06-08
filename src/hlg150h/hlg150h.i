%include "../common_top.i"
%include "iLightController.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%include "arrays_java.i";
%include "../java_buffer.i"

JAVA_JNI_LOADLIBRARY(javaupm_hlg150h)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "hlg150h.hpp"
%}
%include "hlg150h.hpp"
/* END Common SWIG syntax */
