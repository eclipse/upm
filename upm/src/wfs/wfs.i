%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore flowISR;

JAVA_JNI_LOADLIBRARY(javaupm_wfs)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "wfs.hpp"
%}
%include "wfs.hpp"
/* END Common SWIG syntax */
