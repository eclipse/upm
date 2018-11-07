%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore beatISR;

JAVA_JNI_LOADLIBRARY(javaupm_groveehr)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "groveehr.hpp"
%}
%include "groveehr.hpp"
/* END Common SWIG syntax */
