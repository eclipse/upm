%include "../common_top.i"

/* BEGIN Java syntax  ------------------------------------------------------- */
#ifdef SWIGJAVA
%ignore beatISR;

JAVA_JNI_LOADLIBRARY(javaupm_ehr)
#endif
/* END Java syntax */

/* BEGIN Common SWIG syntax ------------------------------------------------- */
%{
#include "ehr.hpp"
%}
%include "ehr.hpp"
/* END Common SWIG syntax */
